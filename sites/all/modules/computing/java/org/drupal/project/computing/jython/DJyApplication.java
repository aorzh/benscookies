package org.drupal.project.computing.jython;

import org.apache.commons.lang3.ArrayUtils;
import org.drupal.project.computing.DApplication;

/**
 * This is to help with Jython applications.
 */
abstract public class DJyApplication extends DApplication {

    @Override
    public String getIdentifier() {
        throw new IllegalArgumentException("Please override getIdentifier()");
    }

    @Override
    public void launch() {
        if (pushOptions != null && pushOptions.length >= 3) {
            // in jython application, the first arg is the script name, and we should remove it.
            pushOptions = ArrayUtils.remove(pushOptions, 0);
        }
        super.launch();
    }

}
