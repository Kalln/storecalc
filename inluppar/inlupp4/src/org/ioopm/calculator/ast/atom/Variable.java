package org.ioopm.calculator.ast.atom;

import org.ioopm.calculator.Visitor;
import org.ioopm.calculator.ast.IllegalAssignmentException;
import org.ioopm.calculator.ast.SymbolicExpression;

public class Variable extends Atom {

    private String identifier;

    public Variable(String identifier) {
        if (identifier == null) { throw new IllegalAssignmentException("null is not a valid variable name."); }
        this.identifier = identifier;
    }

    public String getVariable() {
        return this.identifier;
    }

    @Override
    public String toString() {
        return this.identifier;
    }

    @Override
    public boolean isVariable() {
        return true;
    }

    @Override
    public int hashCode() {
        return this.identifier.hashCode();
    }

    @Override
    public boolean equals(Object other) {
        if (other instanceof Variable v) {
            return this.identifier.equals(v.identifier);
        } else {
            return false;
        }
    }

    @Override
    public SymbolicExpression accept(Visitor v) {
        return v.visit(this);
    }
}
