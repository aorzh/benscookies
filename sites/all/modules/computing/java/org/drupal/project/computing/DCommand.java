package org.drupal.project.computing;

import java.util.concurrent.Callable;
import java.util.logging.Logger;

/**
 * Individual command to be executed. Each command is also registered with a DApplication.
 * A command doesn't necessarily know a DSite. If needed, it can get from DApplication.
 * The Record class needs to know a DSite in order to do database operations.
 *
 * DCommand has no constructor. To initialize a DCommand, use the "create()" factory method,
 * which also initialize things for all DCommand sub-classes. Sub-classes should implement the map* methods for
 * further initialization.
 */
abstract public class DCommand implements Runnable, Callable<DRecord> {

    protected Logger logger = DUtils.getInstance().getPackageLogger();

    protected DApplication application;

    protected DRecord record;


    /**
     * Create a command sub-class, and then initialize it.
     * @param commandClass
     * @param application
     * @param record
     * @return
     */
    public static DCommand create(Class<? extends DCommand> commandClass, DApplication application, DRecord record) {
        assert commandClass != null && application != null && record != null;
        DCommand command = null;
        try {
            command = (DCommand) commandClass.newInstance();
            command.application = application;
            command.record = record;
        } catch (InstantiationException e) {
            e.printStackTrace();
            assert false;
        } catch (IllegalAccessException e) {
            e.printStackTrace();
            assert false;
        }
        return command;
    }


    public static DCommand create(Class<? extends DCommand> commandClass, DApplication application) {
        assert commandClass != null && application != null;
        DRecord record = DRecord.create(application.getIdentifier());
        DCommand command = create(commandClass, application, record);
        record.setCommand(command.getIdentifier());
        record.setDescription("Forged command: " + command.getIdentifier());
        try {
            record.setCreated(application.getDrupalSite().getTimestamp());
        } catch (DConnectionException e) {
            DUtils.getInstance().getPackageLogger().warning("Cannot retrieve timestamp from Drupal site.");
        }
        return command;
    }

    protected DApplication getApplication() {
        assert application != null;
        return this.application;
    }

    protected DRecord getRecord() {
        assert record != null;
        return this.record;
    }

    /**
    * Each DCommand is associated with one DApplication, which means it's associated with one Drupal site.
    * @return
    */
    protected DSite getDrupalSite() {
        assert application != null : "Application can't be null.";
        return application.getDrupalSite();
    }

    public String getIdentifier() {
        return DUtils.getInstance().getIdentifier(this.getClass());
    }

    protected void markStatus(DRecord.Status status) throws DConnectionException {
        assert record != null;
        record.setStatus(status);
        if (record.isSaved()) {
            application.getDrupalSite().updateRecordField(record, "status");
        }
    }

    protected void markProgress(float progress) throws DConnectionException {
        assert record != null;
        assert progress >= 0.0 && progress <= 1.0 : "Progress is in the range of [0, 1].";
        record.setProgress(progress);
        if (record.isSaved()) {
            application.getDrupalSite().updateRecordField(record, "progress");
        }
    }

    protected void printMessage(String message) {
        assert record != null;
        StringBuffer buffer = new StringBuffer();
        if (record.getMessage() != null) {
            buffer.append(record.getMessage());
        }
        buffer.append(message).append("\n");
        record.setMessage(buffer.toString());
    }

    /**
     * Defines how to map a record to the parameters in the command.
     * @param record
     */
    abstract public void mapRecord(DRecord record);

    /**
     * Defines how to map the args to the given record.
     * DApplication will then run mapRecord() so there's no need to initialize the command here.
     * @param args
     * @param record
     */
    abstract public void mapArgs(String[] args, DRecord record);

    /**
     * Saves command results back to the record.
     * If you don't want to save results to the record, you don't need to write code here.
     */
    abstract public void keepResults(DRecord record);

    /**
     * Overrides Runnable.run().
     * Sub-class can either override this method, or override execute() instead. The latter is recommended.
     * If override this method, the sub-class needs to handle "record". If only overrides execute(), no need to handle "record".
     */
    @Override
    public void run() {
        before();  // run code before execution
        try {
            execute();
        } catch (DCommandExecutionException e) {
            try {
                // expected failure, mark status as failure
                markStatus(DRecord.Status.FAIL);
                printMessage(e.getMessage());
            } catch (DConnectionException e1) {
                throw new DRuntimeException(e1);
            }
        } catch (DConnectionException e) {
            // since it's already DConnectionException, it makes no sense to markStatus again.
            // throw RuntimeExcetpion directly.
            throw new DRuntimeException(e);
        } catch (DRuntimeException e) {
            try {
                // unexpected error, mark status as "interrupted"
                markStatus(DRecord.Status.INTR);
                printMessage(e.getMessage());
            } catch (DConnectionException e1) {
                throw new DRuntimeException(e1);
            }
        }
        after(); // run code after execution
    }


    protected void before() {
        assert application != null && record != null;
        try {
            markStatus(DRecord.Status.RUNG);
            record.setStart(application.getDrupalSite().getTimestamp());
        } catch (DConnectionException e) {
            logger.warning("Drupal connection problem. Cannot set status/start.");
            throw new DRuntimeException(e);
        }

        // map record into command parameters.
        // attention: should not do it here, because it's initialization and shouldn't be in run();
        // mapRecord(record);
    }


    protected void after() {
        assert application != null && record != null;
        try {
            long timestamp = application.getDrupalSite().getTimestamp();
            record.setEnd(timestamp);
            record.setUpdated(timestamp);
            if (record.getStatus() == null || record.getStatus() == DRecord.Status.RUNG) {
                markStatus(DRecord.Status.OKOK);
            }
        } catch (DConnectionException e) {
            logger.warning("Drupal connection problem. Cannot set status/updated/end.");
            throw new DRuntimeException(e);
        }
        // save command results back to record.
        keepResults(record);
        // leave record persistence to the application.
    }

    /**
     * The execution of DCommand sub-class doesn't have to care about DRecord.
     */
    abstract protected void execute() throws DCommandExecutionException, DConnectionException;


    /**
     * Impelments Callable::call(). Usually you don't need to override this.
     * @return
     */
    @Override
    public DRecord call() {
        run();
        return record;
    }
}
