package org.ioopm.calculator.ast.atom;

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
}
