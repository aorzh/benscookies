package org.drupal.project.computing;

import org.apache.commons.cli.*;
import org.apache.commons.lang3.ArrayUtils;
import org.apache.commons.lang3.StringUtils;

import java.util.*;
import java.util.concurrent.*;
import java.util.logging.Level;
import java.util.logging.Logger;

/**
 * This is the application class.
 *
 * <b>Additional thoughts:</b>
 *
 * It'll be nice to have "site" and "config" as "final". But that means they have to be set in constructors,
 * and launchFromShell() would have to be static factory method, and then it'll increase complexity.
 *
 * This design combines DApplication and DLauncher. Pro: simple. Con: Can't use other launcher easily.
 */
abstract public class DApplication {

    /**
     * Various running mode of the Druplet
     */
    public static enum RunningMode {
        /**
         * Only execute the next available command in the queue, and then exit.
         */
        FIRST,

        /**
         * Retrieve a list of available commands, execute them in serial, and then exit.
         */
        SEQUENTIAL,

        /**
         * Retrieves a list of commands, execute them in parallel, and then exit.
         */
        PARALLEL,

        /**
         * Continuously retrieve new commands from the queue, execute them in parallel (serial just means thread=1), and exit only when told to.
         */
        DAEMON,

        /**
         * Run application from shell, rather than reading commands from {computing_record} table.
         */
        PUSH
    }


    protected Logger logger = DUtils.getInstance().getPackageLogger();

    /**
     * Configuration for this application
     */
    protected DConfig config = new DConfig();

    /**
     * Drupal site for this application
     */
    protected DSite drupalSite;

    /**
     * Running mode of this DApplication. Not supposed to be accessible by sub-classes.
     */
    private RunningMode runningMode = RunningMode.SEQUENTIAL;

    /**
     * Extra options when running in "push" mode.
     */
    protected String[] pushOptions;

    /**
     * acceptable command registry in the format of:
     * Class name => the class object.
     */
    protected Map<String, Class<? extends DCommand>> commandRegistry = new HashMap<String, Class<? extends DCommand>>();


    /**
     * Default constructor. There are no other constructors.
     * In your sub-class, you can set this.config in this constructor.
     */
    public DApplication() {
        buildCommandRegistry();
        // additional settings of this.config.
    }

    public DSite getDrupalSite() {
        return drupalSite;
    }


    /**
     * Launches the DApplication from shell.
     * users of the class should call this function.
     * @param args arguments from main()
     */
    public void launchFromShell(String[] args) {
        // build command parser
        Options options = getShellOptions();
        CommandLineParser parser = new PosixParser();
        CommandLine command = null;

        try {
            command = parser.parse(options, args);
        } catch (ParseException exp) {
            logger.severe("Cannot parse parameters. Error message: " + exp.getMessage());
            printHelpMessage();
            System.exit(-1);
        }

        // handle --debug option
        if (command.hasOption('d')) {
            logger.setLevel(Level.FINEST);
        }

        // handle --drush option
        if (command.hasOption('s')) {
            config.setProperty("drushExec", command.getOptionValue('s'));
        }

        // handle --run option
        if (command.hasOption('r')) {
            try {
                runningMode = RunningMode.valueOf(command.getOptionValue('r').toUpperCase());
                if (runningMode == RunningMode.PUSH) {
                    pushOptions = command.getArgs();
                }
            } catch (Exception e) {
                logger.severe("Cannot recognize running mode. Use -h to see the options available");
                System.exit(-1);
            }
        }

        // handle the executables. mutually exclusive options in the OptionGroup
        if (command.hasOption('h')) {
            printHelpMessage();
        }
        else if (command.hasOption('t')) {
            initDrupalSite();
            if (drupalSite.checkConnection()) {
                logger.info("Check drupal site connection successfully.");
            } else {
                logger.warning("Cannot connect to drupal site.");
            }
        }
        else {
            launch();
        }
    }


    /**
     * In your sub-class, you can override this method to initialized different drupal site.
     * @return
     */
    protected void initDrupalSite() {
        // default is to create a Drush site.
        drupalSite = new DDrushSite(config.getProperty("drushExec", ""));
    }


    /**
     * launch the DApplication.
     */
    public void launch() {
        logger.info("Drupal Hybrid Computing VERSION: " + DUtils.getInstance().VERSION);
        if (!DUtils.getInstance().checkJavaVersion()) {
            logger.severe("Please install Java 1.6 or greater version. Your Java version is " + System.getProperty("java.version"));
            System.exit(-1);
        }

        try {
            // initialize Drupal site.
            initDrupalSite();
            assert drupalSite != null;

            if (!drupalSite.checkConnection()) {
                logger.severe("Cannot connect to Drupal site.");
                System.exit(-1);
            }

            switch (runningMode) {
                case FIRST:
                    //runOnce();
                    break;
                case SEQUENTIAL:
                    runSequential();
                    break;
                case PARALLEL:
                    runParallel();
                    break;
                case DAEMON:
                    throw new UnsupportedOperationException("DAEMON running mode not supported yet.");
                    //break;
                case PUSH:
                    runPush();
                    break;
            }
            // even though serial didn't use executor, we still shut it down in case any commands used it.
            //logger.info("Shutdown parallel executor.");
            //executor.shutdown();
        } catch (DRuntimeException e) {
            e.printStackTrace();
            logger.severe("DApplication runtime exception. Exit.");
            System.exit(-1);
        } catch (Throwable e) {
            e.printStackTrace();
            logger.severe("Unexpected errors. Program exits.");
            System.exit(-1);
        }

        logger.info("DApplication is accomplished: " + getIdentifier());
    }

    /**
     * If subclass needs to set properties, Use -D settings instead of override the method here.
     * Will not use
     */
    private Options getShellOptions() {
        Options options = new Options();
        options.addOption("d", "debug", false, "enable debug mode");
        options.addOption("s", "drush", true, "drush command");
        options.addOption("r", "run", true, "program running mode: 'sequential' (default), 'parallel', " +
                "'daemon', 'first', or 'push'");

        OptionGroup executables = new OptionGroup();
        executables.addOption(new Option("h", "help", false, "print this message"));
        executables.addOption(new Option("t", "test", false, "test connection to Drupal site"));
        options.addOptionGroup(executables);

        return options;
    }


    /**
     * Print help message. This can be overridden.
     */
    private void printHelpMessage() {
        HelpFormatter formatter = new HelpFormatter();
        // TODO: first param should be "java -jar xxx.jar [options]
        String syntax = String.format("java -jar %s.jar [options]", getIdentifier());
        formatter.printHelp(syntax, getShellOptions());
    }


    private void runSequential() throws DRuntimeException, DConnectionException {
        assert commandRegistry != null;
        List<DRecord> records = drupalSite.queryActiveRecords(getIdentifier());
        logger.info("Total number of commands to run: " + records.size());

        logger.fine("Sorting commands.");
        Collections.sort(records);

        for (DRecord record : records) {
            if (record.getControl() == DRecord.Control.CNCL) {
                record.setStatus(DRecord.Status.STOP);
            }
            else if (record.isReady() && !commandRegistry.containsKey(record.getCommand())) {
                record.setStatus(DRecord.Status.NRCG);
            }
            else if (record.isReady() && commandRegistry.containsKey(record.getCommand())) {
                DCommand command = DCommand.create(commandRegistry.get(record.getCommand()), this, record);
                command.mapRecord(record);
                command.run();
            } else {
                // for now, we don't care other cases with 'REMT' etc
            }

            logger.info("Command finished running with status: " + record.getStatus().toString());
            // if updateRecord throws exception, then we simply exit running the app unless we fix the problem.
            drupalSite.updateRecord(record);
        }
    }


    private void runParallel() throws DRuntimeException, DConnectionException {
        int coreThreadNum = (Integer) drupalSite.variableGet("computing_thread_core_number", new Integer(2));
        int maxThreadNum = (Integer) drupalSite.variableGet("computing_thread_max_number", new Integer(2));
        if (maxThreadNum < coreThreadNum) {
            logger.warning("thread_max_number cannot be smaller than thread_core_number.");
            maxThreadNum = coreThreadNum;
        }

        // set the executor to be pooled executor.
        ExecutorService executor = new ThreadPoolExecutor(coreThreadNum, maxThreadNum, 0, TimeUnit.SECONDS,
                new PriorityBlockingQueue<Runnable>(), new ThreadPoolExecutor.AbortPolicy()) {
            @Override
            protected void afterExecute(Runnable runnable, Throwable throwable) {
                super.afterExecute(runnable, throwable);
                if (throwable != null) {
                    assert runnable instanceof Future<?>;
                    DRecord record = null;
                    try {
                        record = (DRecord) (((Future<?>) runnable).get());
                        record.setStatus(DRecord.Status.INTR);
                    } catch (InterruptedException e) {
                        assert false;
                    } catch (ExecutionException e) {
                        assert false;
                    }
                }
            }
        };

        List<DRecord> records = drupalSite.queryActiveRecords(getIdentifier());
        //logger.fine("Sorting commands.");
        //Collections.sort(records);
        List<Future<DRecord>> futuresList = new ArrayList<Future<DRecord>>();

        for (DRecord record : records) {
            if (record.getControl() == DRecord.Control.CNCL) {
                record.setStatus(DRecord.Status.STOP);
            }
            else if (record.isReady() && !commandRegistry.containsKey(record.getCommand())) {
                record.setStatus(DRecord.Status.NRCG);
            }
            else if (record.isReady() && commandRegistry.containsKey(record.getCommand())) {
                DCommand command = DCommand.create(commandRegistry.get(record.getCommand()), this, record);
                command.mapRecord(record);
                Future<DRecord> future = executor.submit((Callable<DRecord>)command);
                futuresList.add(future);
            } else {
                // for now, we don't care other cases with 'REMT' etc
            }
        }

        logger.info("Total number of commands to run in parallel: " + futuresList.size());

        for (Future<DRecord> future : futuresList) {
            DRecord record = null;
            try {
                // TODO: could enforce timeout here.
                record = future.get();
                drupalSite.updateRecord(record);
            } catch (InterruptedException e) {
                e.printStackTrace();
                // don't change anything. leave it there.
            } catch (ExecutionException e) {
                e.printStackTrace();
                // don't change anything. leave it there.
            }
        }
        executor.shutdown();
        logger.info("All commands finished running in parallel.");
    }

    private void runPush() throws DConnectionException, DRuntimeException {
        assert commandRegistry != null;
        if (pushOptions == null || pushOptions.length < 2) {
            logger.severe("When running in the push mode, you have to specify at least 'command' and 'description' " +
                    "arguments. Exit.");
            return;
        }
        logger.info("Push to run command with options: " + StringUtils.join(pushOptions, " "));

        String commandString = pushOptions[0];
        String descriptionString = pushOptions[1];
        String[] leftOver = ArrayUtils.subarray(pushOptions, 2, pushOptions.length);

        DRecord record = DRecord.create(getIdentifier(), commandString, descriptionString, null, null, drupalSite.getTimestamp());
        record.setControl(DRecord.Control.PUSH);
        if (!commandRegistry.containsKey(commandString)) {
            record.setStatus(DRecord.Status.NRCG);
        } else {
            DCommand command = DCommand.create(commandRegistry.get(commandString), this, record);
            command.mapArgs(leftOver, record);
            command.mapRecord(record);
            command.run();
        }

        logger.info("Command finished running with status: " + record.getStatus().toString());
        // here we save the new message.
        drupalSite.saveRecord(record);
    }


    /**
     * Specifies the name this Druplet is known as. By default is the class name. You can override default value too.
     * @return The identifier of the app.
     */
    public String getIdentifier() {
        return DUtils.getInstance().getIdentifier(this.getClass());
    }


    /**
     * Register a command with the DApplication.
     * @param commandClass
     */
    public void registerCommand(Class<? extends DCommand> commandClass) {
        // the command class has to be a subclass of DCommand
        assert DCommand.class.isAssignableFrom(commandClass);
        // this method is problematic because it doesn't respect the class' own getIdentifier() method.
        // String id = DUtils.getInstance().getIdentifier(commandClass);

        //use reflection instead.
        try {
            DCommand command = commandClass.newInstance();
            commandRegistry.put(command.getIdentifier(), commandClass);
        } catch (InstantiationException e) {
            e.printStackTrace();
            assert false;
        } catch (IllegalAccessException e) {
            e.printStackTrace();
            assert false;
        }
    }


    /**
     * Register a command with arbitrary identifier.
     * @param identifier
     * @param commandClass
     */
    public void registerCommand(String identifier, Class commandClass) {
        // the command class has to be a subclass of AsyncCommand
        assert DCommand.class.isAssignableFrom(commandClass);
        commandRegistry.put(identifier, commandClass);
    }


    /**
     * Register acceptable command here.
     */
    abstract protected void buildCommandRegistry();

}
