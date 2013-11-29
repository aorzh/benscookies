package org.drupal.project.computing.common;

import org.apache.commons.lang3.StringUtils;
import org.drupal.project.computing.DCommand;
import org.drupal.project.computing.DCommandExecutionException;
import org.drupal.project.computing.DRecord;
import org.drupal.project.computing.Identifier;

/**
 * This is a simple command that might be used in any DApplication.
 */
@Identifier("PingMe")
public class PingMe extends DCommand {

    private String message;

    private void initialize(String message) {
        this.message = message;
    }

    @Override
    public void mapRecord(DRecord record) {
        initialize(record.getString1());
    }

    @Override
    public void mapArgs(String[] args, DRecord record) {
        record.setString1(StringUtils.join(args, " "));
    }

    @Override
    public void keepResults(DRecord record) {
        record.setString2(message);
    }

    @Override
    protected void execute() throws DCommandExecutionException {
        if (StringUtils.isEmpty(message)) {
            printMessage("Pong.");
        } else {
            printMessage("Pong with message: " + message);
        }
    }
}
