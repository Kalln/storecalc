package org.ioopm.calculator.ast.binary;

import org.ioopm.calculator.ast.SymbolicExpression;

public abstract class Condition extends Binary {
    public Condition(SymbolicExpression lhs, SymbolicExpression rhs) {
        super(lhs, rhs);
    }

    @Override
    public int getPriority() {
        return 50;
    }

    @Override
    public String toString() {
        if (this.getLhs().isConstant() || this.getLhs().isVariable() 
            && this.getRhs().isConstant() || this.getRhs().isVariable()) {
            return this.getLhs().toString() + " " + this.getName() + " " + this.getRhs().toString();
        }
        return "(" + this.getLhs().toString() + ") " + this.getName() + " (" + this.getRhs().toString() + ")";
    }
}
