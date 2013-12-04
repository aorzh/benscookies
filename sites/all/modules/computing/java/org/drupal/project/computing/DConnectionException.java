package org.drupal.project.computing;

import org.drupal.project.computing.DRuntimeException;

/**
 * This exception is thrown when there's something wrong communicating with the Drupal site (DSite)
 * Caller is expected to catch this exception and handles it properly.s
 */
public class DConnectionException extends Exception {
    public DConnectionException() {
        super();
    }

    public DConnectionException(String s) {
        super(s);
    }

    public DConnectionException(String s, Throwable throwable) {
        super(s, throwable);
    }

    public DConnectionException(Throwable throwable) {
        super(throwable);
    }
}
