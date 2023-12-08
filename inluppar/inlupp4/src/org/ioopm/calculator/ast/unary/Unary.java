package org.ioopm.calculator.ast.unary;

import org.ioopm.calculator.ast.SymbolicExpression;

public abstract class Unary extends SymbolicExpression {
    public SymbolicExpression exp;

    public abstract String getName();

    public Unary(SymbolicExpression exp) {
        this.exp = exp;
    }

    public SymbolicExpression getExp() {
        return this.exp;
    }

    @Override
    public String toString() {
        return this.getName() + "(" + this.exp.toString() + ")";
    }

    @Override
    public boolean equals(Object other) {
        if (other instanceof Unary u) {
            if (this.getName() == u.getName() && this.exp.equals(u.exp)) {
                return true;
            }
        }
        return false;
    }
}
