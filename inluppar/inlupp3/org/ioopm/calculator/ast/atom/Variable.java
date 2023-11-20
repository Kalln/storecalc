package org.ioopm.calculator.ast.atom;

import org.ioopm.calculator.ast.Environment;
import org.ioopm.calculator.ast.SymbolicExpression;

public class Variable extends Atom {

    private String identifier;

    public Variable(String identifier) {
        super("Variable");
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

    public boolean equals(Variable other) {
        return this.identifier.equals(other.getVariable());
    }

    public SymbolicExpression eval(Environment vars) {
        if (vars.get(this) == null) {
            return this;
        } else {
            return vars.get(this);
        }
    }
    
}
