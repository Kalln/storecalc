package org.ioopm.calculator.ast.binary;

import org.ioopm.calculator.Visitor;
import org.ioopm.calculator.ast.IllegalAssignmentException;
import org.ioopm.calculator.ast.SymbolicExpression;

public class Assignment extends Binary {
    public Assignment(SymbolicExpression res, SymbolicExpression key) throws IllegalAssignmentException{
        super("Assignment", res, key);

        if (!key.isVariable()) {
            throw new IllegalAssignmentException("Right hand side must be a variable.");
        }
    }

    @Override
    public int getPriority() {
        return 5;
    }

    public String getName() {
        return "=";
    }

    @Override
    public SymbolicExpression accept(Visitor v) {
        return v.visit(this);
    }
}
