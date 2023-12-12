package org.ioopm.calculator.ast.atom;

import org.ioopm.calculator.Visitor;
import org.ioopm.calculator.ast.SymbolicExpression;

public class Constant extends Atom{
    private double value;

    public Constant(double value) {
        this.value = value;
    }

    @Override
    public boolean isConstant() {
        return true;
    }

    @Override
    public double getValue() {
        return this.value;
    }

    @Override
    public String toString() {
        return String.valueOf(this.value);
    }

    public boolean equals(Object other) {
        if (other instanceof Constant c) {
            return this.getValue() == c.getValue();
        } else {
            return false;
        }
    }

    @Override
    public SymbolicExpression accept(Visitor v) {
        return v.visit(this);
    }
}
