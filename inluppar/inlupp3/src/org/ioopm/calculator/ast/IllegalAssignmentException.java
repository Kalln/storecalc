package org.ioopm.calculator.ast;

import org.ioopm.calculator.parser.SyntaxErrorException;

public class IllegalAssignmentException extends SyntaxErrorException {

    public IllegalAssignmentException(String msg) {
        super(msg);
    }
}
