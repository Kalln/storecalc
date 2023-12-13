package org.ioopm.calculator.ast.binary;

import org.ioopm.calculator.ast.SymbolicExpression;

public abstract class Binary extends SymbolicExpression{
    private SymbolicExpression lhs;
    private SymbolicExpression rhs;

    public Binary(SymbolicExpression lhs, SymbolicExpression rhs) {
        this.lhs = lhs;
        this.rhs = rhs;
    }

    public SymbolicExpression getLhs() {
        return lhs;
    }

    public SymbolicExpression getRhs() {
        return rhs;
    }

    @Override
    public String toString() {
        // check if one side has higher priority than the other
        if (this.lhs.getPriority() < this.getPriority()) {
            return "(" + lhs.toString() + ") " + getName() + " " + rhs.toString();
        } else if (this.rhs.getPriority() < this.getPriority()) {
            return lhs.toString() + " " + getName() + " (" + rhs.toString() + ")";
        }
        // if not, just print it out
        return lhs.toString() + " " + getName() + " " + rhs.toString();
    }

    @Override
    public boolean equals(Object other) {
        if (other instanceof Binary b) {
            return this.lhs.equals(b.lhs) && this.rhs.equals(b.rhs);
        }
        return false;
    }
}
