package org.ioopm.calculator.ast.atom;

import org.ioopm.calculator.ast.Environment;
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

    public String toString() {
        return this.identifier;
    }

    public boolean isVariable() {
        return true;
    }

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

    public SymbolicExpression eval(Environment vars) {
        SymbolicExpression result = vars.get(this);
        return result != null ? result : this;
    }

}
