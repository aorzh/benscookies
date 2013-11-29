package org.drupal.project.computing.jython;

import org.drupal.project.computing.DCommand;

/**
 * This is to help with Jython applications
 */
abstract public class DJyCommand extends DCommand {

    @Override
    public String getIdentifier() {
        throw new IllegalArgumentException("Please override getIdentifier()");
    }

}
