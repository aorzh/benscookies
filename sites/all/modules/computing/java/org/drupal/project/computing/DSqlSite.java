package org.drupal.project.computing;

import java.util.List;

/**
 * Uses direct JDBC database access to connect to Drupal site database.
 * Unimplemented yet.
 */
public class DSqlSite extends DSite {
    @Override
    public List<DRecord> queryActiveRecords(String appName) throws DConnectionException {
        return null;
    }

    @Override
    public void updateRecord(DRecord record) throws DConnectionException {
    }

    @Override
    public void updateRecordField(DRecord record, String fieldName) throws DConnectionException {
    }

    @Override
    public long saveRecord(DRecord record) throws DConnectionException {
        return 0;
    }

    @Override
    public DRecord loadRecord(long id) throws DConnectionException {
        return null;
    }

    @Override
    public String getDrupalVersion() throws DConnectionException {
        return null;
    }

    @Override
    public Object variableGet(String name, Object defaultValue) throws DConnectionException {
        return null;
    }

    @Override
    public void variableSet(String name, Object value) throws DConnectionException {
    }

    @Override
    public long getTimestamp() throws DConnectionException {
        return 0;
    }

    public static class UnitTest {

        //@Test
        public void testVariables() throws Exception {
            DConfig config = new DConfig();
            config.setProperty("drupal.db.max_batch_size", "2");
            DDatabase db = new DDatabase(config.getDbProperties());

//            String s1 = DrupletUtils.evalPhp("echo serialize(2);");
//            Object[][] params1 = {{"async_command_test1", "1".getBytes()}, {"async_command_test2", s1.getBytes()}, {"async_command_test3", "3".getBytes()}};
//            conn.batch("INSERT INTO {variable}(name, value) VALUES(?, ?)", params1);
//
//            long num1 = (Long) conn.queryValue("SELECT COUNT(*) FROM {variable} WHERE name LIKE 'async_command_test%'");
//            assertEquals(3, num1);
//
//            int v1 = (Integer) conn.variableGet("async_command_test2");
//            assertEquals(v1, 2);
//
//            // test variable set.
//            conn.variableSet("async_command_test2", 100);
//            v1 = (Integer) conn.variableGet("async_command_test2");
//            assertEquals(v1, 100);
//            conn.variableSet("async_command_test2", "Hello");
//            s1 = (String) conn.variableGet("async_command_test2");
//            assertEquals(s1, "Hello");
//
//
//            String s2 = DrupletUtils.evalPhp("echo serialize('abc');");
//            Object[][] params2 = {{s2.getBytes(), "async_command_test1"}};
//            conn.batch("UPDATE {variable} SET value=? WHERE name=?", params2);
//            String v2 = (String) conn.variableGet("async_command_test1");
//            assertEquals(v2, "abc");
//
//            // re-establish connection
//            conn.close();
//            prop = DrupletUtils.loadProperties(DrupletUtils.getConfigPropertiesFile());
//            prop.setProperty("db_max_batch_size", "0");
//            conn = new DrupalConnection(new DrupletConfig(prop));
//            conn.connect();
//
//            Object[][] params3 = {{"async_command_test1"}, {"async_command_test2"}, {"async_command_test3"}};
//            conn.batch("DELETE FROM {variable} WHERE name=?", params3);
//            long num2 = (Long) conn.queryValue("SELECT COUNT(*) FROM {variable} WHERE name LIKE 'async_command_test%'");
//            assertEquals(num2, 0);
//            conn.close();
        }
    }
}
