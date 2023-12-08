package org.ioopm.calculator.ast.atom;

public class NamedConstant extends Atom {
    private String identifier;
    private double value;

    public NamedConstant(String identifier, double value) {
        this.identifier = identifier;
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
        return this.identifier;
    }
}
