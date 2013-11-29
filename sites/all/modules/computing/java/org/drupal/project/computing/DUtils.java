package org.drupal.project.computing;

import com.google.gson.Gson;
import com.google.gson.GsonBuilder;
import org.apache.commons.exec.CommandLine;
import org.apache.commons.exec.DefaultExecutor;
import org.apache.commons.exec.ExecuteException;
import org.apache.commons.exec.PumpStreamHandler;
import org.apache.commons.lang3.ArrayUtils;
import org.apache.commons.lang3.JavaVersion;
import org.apache.commons.lang3.StringUtils;
import org.apache.commons.lang3.SystemUtils;
import org.junit.Test;

import java.io.*;
import java.util.List;
import java.util.Properties;
import java.util.Scanner;
import java.util.logging.Logger;

import static junit.framework.Assert.assertEquals;
import static junit.framework.Assert.assertFalse;
import static junit.framework.Assert.assertTrue;

/**
 * Singleton of the utilities class.
 */
public class DUtils {

    ////// Singleton template ////////
    private static DUtils ourInstance = new DUtils();
    public static DUtils getInstance() {
        return ourInstance;
    }
    private DUtils() {
    }

    ///////////////////  code begins /////////////////

    public final String VERSION = "7.x-1.0-alpha1";

    private Logger logger = getPackageLogger();

    private Gson defaultGson = new GsonBuilder().create();


    public final Logger getPackageLogger() {
        return Logger.getLogger("org.drupal.project.computing");
    }


    /**
     * Execute a command in the working dir, and return the output as a String. If error, log the errors in logger.
     * This is the un-refined version using Process and ProcessBuilder. See the other version with commons-exec.
     *
     * @param command The list of command and parameters.
     * @param workingDir The working directory. Could be null. The it's default user.dir.
     * @return command output.
     */
    @Deprecated
    public String executeShell(List<String> command, File workingDir) {
        logger.finest("Running system command: " + StringUtils.join(command, ' '));
        ProcessBuilder processBuilder = new ProcessBuilder(command);
        if (workingDir != null && workingDir.exists() && workingDir.isDirectory()) {
            processBuilder.directory(workingDir);
        } else {
            logger.fine("Using current user directory to run system command.");
        }

        try {
            Process process = processBuilder.start();
            process.waitFor();

            if (process.exitValue() != 0) {
                logger.severe(readContent(new InputStreamReader(process.getErrorStream())));
                throw new DRuntimeException("Unexpected error executing system command: " + process.exitValue());
            } else {
                // running successfully.
                return readContent(new InputStreamReader(process.getInputStream()));
            }
        } catch (IOException e) {
            throw new DRuntimeException(e);
        } catch (InterruptedException e) {
            throw new DRuntimeException(e);
        }
    }


    /**
     * Execute a command in the working dir, and return the output as a String. If error, log the errors in logger.
     * TODO: this should take care of arbitrary encoding/decoding.
     * TODO: check to make sure it won't output confidential information from settings.php, etc.
     *
     * @param commandLine The command line object
     * @param workingDir The working directory. Could be null. The it's default user.dir.
     * @param input Input string
     * @return command output.
     */
    public String executeShell(CommandLine commandLine, File workingDir, String input) throws DSystemExecutionException {
        DefaultExecutor executor = new DefaultExecutor();
        // executor.setExitValue(0);
        // ExecuteWatchdog watchdog = new ExecuteWatchdog(60);
        // executor.setWatchdog(watchdog);
        if (workingDir != null) {
            executor.setWorkingDirectory(workingDir);
        }

        ByteArrayInputStream in = StringUtils.isNotEmpty(input) ? new ByteArrayInputStream(input.getBytes()) : null;
        ByteArrayOutputStream out = new ByteArrayOutputStream();
        ByteArrayOutputStream err = new ByteArrayOutputStream();

        executor.setStreamHandler(new PumpStreamHandler(out, err, in));
        logger.finest("Shell command to run: " + commandLine.toString());
        if (StringUtils.isNotBlank(input)) {
            logger.finest("Shell command input stream: " + input);
        }

        try {
            int exitValue = executor.execute(commandLine);
        } catch (ExecuteException e) {
            // this could happen when exit value is not 0. the error message will include exit value.
            //e.printStackTrace();
            if (StringUtils.isNotBlank(out.toString())) {
                logger.fine("Shell command failed output: " + out.toString());
            }
            throw new DSystemExecutionException(e);
        } catch (IOException e) {
            //e.printStackTrace();
            if (StringUtils.isNotBlank(out.toString())) {
                logger.fine("Shell command failed output: " + out.toString());
            }
            throw new DSystemExecutionException(e);
        } finally {
            // if there's any error, give it a chance to print error message in "finally" before throwing exception.
            if (err.toString() != null && err.toString().length() > 0) {
                logger.warning("Shell command error stream message: " + err.toString());
            }
        }
        return out.toString();
    }


    public String executeShell(String command) throws DSystemExecutionException {
        logger.finest("Running system command: " + command);
        CommandLine commandLine = CommandLine.parse(command);
        return executeShell(commandLine, null, null);
    }


    /**
     * From the input reader and get all its content.
     *
     * @param input input reader
     * @return the content of the reader in String.
     * @throws IOException
     */
    public String readContent(Reader input) throws IOException {
        StringBuilder sb = new StringBuilder();
        int c;
        while ((c = input.read()) != -1) {
            sb.append((char)c);
        }
        return sb.toString();
    }


    /**
     * Check to make sure Java is > 1.6
     * @return True if Java version is satisfied.
     */
    public boolean checkJavaVersion() {
        //String version = System.getProperty("java.version");
        //return version.compareTo("1.6") >= 0;
        return SystemUtils.isJavaVersionAtLeast(JavaVersion.JAVA_1_6);
    }


    /**
     * Get either the identifier if presented, or the class name.
     * @param classObject
     * @return
     */
    public String getIdentifier(Class<?> classObject) {
        Identifier id = classObject.getAnnotation(Identifier.class);
        if (id != null) {
            return id.value();
        } else {
            return classObject.getSimpleName();
        }
    }


    /**
     * Get the long value from any Object, if possible.
     *
     * @param value The object that could either be null, or int, or string.
     * @return  The long value of the "value".
     */
    public Long getLong(Object value) {
        if (value == null) {
            return null;
        } else if (Integer.class.isInstance(value)) {
            return ((Integer) value).longValue();
        } else if (Long.class.isInstance(value)) {
            return (Long) value;
        } else if (String.class.isInstance(value)) {
            return Long.valueOf((String) value);
        } else if (Float.class.isInstance(value)) {
            return ((Float) value).longValue();
        } else if (Double.class.isInstance(value)) {
            return ((Double) value).longValue();
        } else {
            throw new IllegalArgumentException("Cannot parse value: " + value.toString());
        }
    }

    /**
     * Encapsulate json object.
     * @param obj
     * @return
     */
    public String toJson(Object obj) {
        return defaultGson.toJson(obj);
    }

    public Gson getDefaultGson() {
        return defaultGson;
    }

    public Properties loadProperties(String configString) {
        Properties config = new Properties();
        try {
            config.load(new StringReader(configString));
        } catch (IOException e) {
            throw new DRuntimeException("Cannot read config string in DUtils.");
        }
        return config;
    }

    /**
     * Utility class to run PHP snippet
     */
    public static class Php {

        private final String phpExec;

        public Php(String phpExec) {
            assert StringUtils.isNotEmpty(phpExec);
            this.phpExec = phpExec;
        }

        public Php(DConfig config) {
            this(config.getPhpExec());
        }

        public Php() {
            this(new DConfig());
        }

        /**
         * Evaluates PHP code and return the output in JSON. JSR 223 should be the recommended approach, but there is no
         * good PHP engine. PHP-Java bridge is simply a wrapper of php-cgi and doesn't do much, and is not as flexible as
         * we call php exec. Quercus works well, but the jar file is too big to include here. In short, we'll simply call
         * PHP executable and get results in JSON. see [#1220194]
         *
         * @param phpCode PHP code snippet.
         * @return PHP code execution output.
         */
        public String evaluate(String phpCode) throws DSystemExecutionException {
            // test php executable
            String testPhp = getInstance().executeShell(phpExec + " -v");
            if (!testPhp.startsWith("PHP")) {
                throw new DSystemExecutionException("Cannot execute php executable: " + phpExec);
            }
            getInstance().logger.fine("Evaluate with PHP version: " + new Scanner(testPhp).nextLine());

            CommandLine commandLine = new CommandLine(phpExec);

            // we could suppress error. but this is not good because we want it generate errors when fails.
            // in command line "-d error_reporting=0", but here it didn't work.
            //commandLine.addArgument("-d error_reporting=0");

            commandLine.addArgument("--");  // execute input from shell.
            return getInstance().executeShell(commandLine, null, phpCode);
        }


        /**
         * Extract the php code snippet that defines phpVar from the php file phpFile. For example, to get $databases
         * from settings.php: extractPhpVariable(DConfig.locateFile("settings.php", "$databases");
         *
         * PHP code: see parse.php.
         *
         * FIXME: if the file has someting like "var_dump($databases);", the code would return "$databases);", which is incorrect.
         * need to fix it in the next release.
         *
         * @param phpFile the PHP file to be processed.
         * @param phpVar the PHP variable to be extracted. It can only be the canonical form $xxx, not $xxx['xxx'].
         * @return the PHP code that defines phpVar.
         */
        public String extractVariable(File phpFile, String phpVar) throws DSystemExecutionException {
            assert phpFile.isFile() && phpVar.matches("\\$\\w+");
            final String phpExec = "<?php\n" +
                    "function extract_variable($file, $var_name) {\n" +
                    "  $content = file_get_contents($file);\n" +
                    "  $tokens = token_get_all($content);\n" +
                    "  $code = '';\n" +
                    "\n" +
                    "  $phase = 'skip';\n" +
                    "  foreach ($tokens as $token) {\n" +
                    "    if (is_array($token) && $token[0] == T_VARIABLE && $token[1] == $var_name) {\n" +
                    "      $phase = 'accept';\n" +
                    "      $code .= $token[1];\n" +
                    "    }\n" +
                    "    else if ($phase == 'accept') {\n" +
                    "      $code .= is_array($token) ? $token[1] : $token;\n" +
                    "      if ($token == ';') {\n" +
                    "        $phase = 'skip';\n" +
                    "        $code .= \"\\n\";\n" +
                    "      }\n" +
                    "    }\n" +
                    "  }\n" +
                    "\n" +
                    "  return $code;\n" +
                    "}\n";

            String code = String.format("%s\necho extract_variable('%s', '%s');",
                    phpExec,
                    phpFile.getAbsolutePath().replaceAll("'", "\\'"),
                    phpVar);
            return evaluate(code);
        }
    }


    /**
     * This is the utility class to run drush command.
     */
    public static class Drush {

        private final String drushExec;

        public Drush(String drushExec) {
            assert StringUtils.isNotEmpty(drushExec);
            this.drushExec = drushExec;
        }

        public Drush(DConfig config) {
            this(config.getDrushExec());
        }

        public Drush() {
            this(new DConfig());
        }


        public String execute(String[] command) throws DConnectionException {
            return execute(command, null);
        }

        /**
         * Execute Drush command, and returns STDOUT results.
         * attention: here we use shell stdout to output results, which is problematic.
         *
         * possible problems are:
         * 1) security concerns, where confidential info might get printed
         * 2) encoding/decoding characters to byte streams, especially with non-english languages and serializd strings.
         * 3) escaping problems
         * 4) output might get messed up with other sub-processes.
         *
         * some other IPC approaches: 1) Apache Camel, 2) /tmp files, 3) message queue
         * however, STDOUT is the simplest approach for now.
         *
         * @param command The drush command to execute.
         * @param input Input stream, could be null.
         * @return STDOUT results.
         * @throws DConnectionException
         */
        public String execute(String[] command, String input) throws DConnectionException {
            try {
                CommandLine cmdLine = CommandLine.parse(drushExec);
                // 2nd parameter is crucial. without it, there would be escaping problems.
                // false means we didn't escape the params and we want CommandLine to escape for us.
                cmdLine.addArguments(command, false);

                //System.out.println(cmdLine.toString());
                return getInstance().executeShell(cmdLine, null, input);

            } catch (DSystemExecutionException e) {
                throw new DConnectionException("Cannot execute drush.", e);
            }
        }

        /**
         * Get Drush version.
         * @return
         * @throws DConnectionException
         */
        public String getVersion() throws DConnectionException {
            return execute(new String[]{"version", "--pipe"}).trim();
        }

        public String getDrushExec() {
            return drushExec;
        }

        /**
         * Get Drupal core-status info.
         * @see "drush core-status"
         *
         * @return
         * @throws DConnectionException
         */
        public Properties getCoreStatus() throws DConnectionException {
            Properties coreStatus = new Properties();
            try {
                coreStatus.load(new StringReader(execute(new String[]{"core-status", "--pipe"})));
            } catch (IOException e) {
                getInstance().logger.severe("Error running drush core-status.");
                e.printStackTrace();
            }
            return coreStatus;
        }

        /**
         * Run any drupal code and get returns results in json.
         *
         * @param phpCode Should not use "<?php ... ?>"
         *
         * @return execution results in JSON.
         * @throws DConnectionException
         */
        public String computingEval(String phpCode) throws DConnectionException {
            if (!checkComputing()) {
                String message = String.format("Drush command '%s' is invalid, or the 'computing' drush command not found at target Drupal site.", drushExec);
                throw new DConnectionException(message);
            }
            // this is the stub function to run any Drupal code.
            String[] command = {"computing-eval", "--pipe", "-"};
            return execute(command, phpCode);
        }


        /**
         * Call any Drupal functions and returns results in json.
         *
         * @param params First param is the function name; the rest are parameters in json.
         *               Callers are responsible to wrap the params in json, but not responsible to escape them as command line args.
         * @return Execution results in JSON.
         * @throws DConnectionException
         */
        public String computingCall(String[] params) throws DConnectionException {
            if (!checkComputing()) {
                String message = String.format("Drush command '%s' is invalid, or the 'computing' drush command not found at target Drupal site.", drushExec);
                throw new DConnectionException(message);
            }
            String[] command = {"computing-call", "--pipe"};
            command = ArrayUtils.addAll(command, params);
            return execute(command);
        }

        /**
         * Check if the "computing" module drush command is available. It doesn't check if the module itself is enabled or not.
         * @return true if computing* drush command is available, or false if not.
         */
        public boolean checkComputing() {
            try {
                String[] command = {"help", "--pipe", "--filter=computing"};
                String results = execute(command);
                return true; // if computing category is not found, then there'll be exception.
            } catch (DConnectionException e) {
                return false;
            }
        }

    }


    public static class UnitTest {
        @Test
        public void testExecuteShell() throws Exception {
            String result = DUtils.getInstance().executeShell("echo Hello").trim();
            // System.out.println(result);
            assertEquals(result, "Hello");

            DUtils.getInstance().executeShell("touch /tmp/computing.test");
            assertTrue((new File("/tmp/computing.test")).exists());
            DUtils.getInstance().executeShell("rm /tmp/computing.test");
            assertTrue(!(new File("/tmp/computing.test")).exists());

            DUtils.getInstance().executeShell(CommandLine.parse("touch computing.test"), new File("/tmp"), null);
            assertTrue((new File("/tmp/computing.test")).exists());
            DUtils.getInstance().executeShell("rm /tmp/computing.test");
            assertTrue(!(new File("/tmp/computing.test")).exists());

            String msg = DUtils.getInstance().executeShell(CommandLine.parse("cat"), null, "hello\u0004");    // Ctrl+D, end of stream.
            assertEquals("hello", msg.trim());

            // this shows input stream automatically sends Ctrl+D.
            msg = DUtils.getInstance().executeShell(CommandLine.parse("cat"), null, "hello, world");
            assertEquals("hello, world", msg.trim());

            /*CommandLine commandLine = CommandLine.parse("drush @dev1");
           commandLine.addArgument("php-eval");
           commandLine.addArgument("echo 'Hello';", true);
           System.out.println(commandLine.toString());
           commandLine.addArgument("echo drupal_json_encode(eval('return node_load(1);'));", true);
           System.out.println(DUtils.getInstance().executeShell(commandLine, null));*/
        }

        @Test
        public void testDrush() throws Exception {
            DUtils.Drush drush = new DUtils.Drush("drush @dev");
            assertTrue(drush.checkComputing());
            assertEquals("Expected drush version", "5.4", drush.getVersion());

            // test drupal version
            Properties coreStatus = drush.getCoreStatus();
            //coreStatus.list(System.out);
            assertTrue("Expected drupal version", coreStatus.getProperty("drupal_version").startsWith("7"));

            // test execute php
            String jsonStr = drush.computingEval("return node_load(1);").trim();
            System.out.println(jsonStr);
            assertTrue(jsonStr.startsWith("{") && jsonStr.endsWith("}"));

            jsonStr = drush.computingEval("node_load(1);").trim();
            System.out.println(jsonStr);
            assertEquals("null", jsonStr);


            // test computing call
            Gson gson = new Gson();
            String s2 = drush.computingCall(new String[]{"variable_get", gson.toJson("install_profile")});
            //System.out.println(s2);
            assertEquals("standard", gson.fromJson(s2, String.class));

            // test check computing
            Drush invalidDrush = new Drush("drush @xxx");
            assertFalse(invalidDrush.checkComputing());
            invalidDrush = new Drush("drush @local");
            assertFalse(invalidDrush.checkComputing());
        }

        @Test
        public void testLocalDrush() throws Exception {
            // test local environment
            Drush drush = new Drush("drush @local");
            Properties coreStatus = drush.getCoreStatus();
            assertTrue(coreStatus.getProperty("drupal_version").startsWith("7"));
            assertEquals("/Users/danithaca/Development/drupal7", coreStatus.getProperty("drupal_root"));
            assertEquals("/Users/danithaca/Development/drupal7", drush.execute(new String[]{"drupal-directory", "--local"}).trim());
        }

        @Test
        public void testMisc() {
            assertEquals(new Long(12L), DUtils.getInstance().getLong(new Long(12L)));
            assertEquals(new Long(5L), DUtils.getInstance().getLong("5"));
            assertEquals(new Long(100L), DUtils.getInstance().getLong(new Float(100.53)));
        }

        @Test
        public void testPhp() throws Exception {
            String results;
            DConfig config = new DConfig();
            assertEquals("php", config.getPhpExec());

            Php php = new Php();

            // System.out.println(DUtils.getInstance().executeShell("php -v"));

            results = php.evaluate("<?php echo 'hello, world';");
            assertEquals("hello, world", results.trim());
            results = php.evaluate("<?php echo json_encode(100);");
            assertEquals(new Integer(100), DUtils.getInstance().getDefaultGson().fromJson(results, Integer.class));

            // try to get $databases from settings.php.
            config.setProperty("drupal.drush", "drush @local");
            File settingsFile = config.locateFile("settings.php");
            String databasesCode = php.extractVariable(settingsFile, "$databases");
            assertTrue(databasesCode.startsWith("$databases"));
        }

        /**
         * This method is not supposed to test anything. It's simply running to print out some stuff.
         */
        @Test
        public void simplePrint() throws Exception {
            DConfig config = new DConfig();
            config.setProperty("drupal.drush", "drush @local");
            File settingsFile = config.locateFile("settings.php");
            //String settingsCode = DUtils.getInstance().readContent(new FileReader(settingsFile));
            System.out.println("Settings.php: " + settingsFile.getAbsolutePath());

            Php php = new Php(config);
            System.out.println(php.extractVariable(settingsFile, "$databases"));
            //System.out.println(DUtils.getInstance().stripPhpComments(settingsCode));
        }
    }
}
