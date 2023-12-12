package org.ioopm.calculator.ast.atom;

import org.ioopm.calculator.Visitor;
import org.ioopm.calculator.ast.SymbolicExpression;

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

    public boolean equals(Object other) {
        return other instanceof NamedConstant nc
            && this.identifier.equals(nc.identifier)
            && nc.value == this.value;
    }

    @Override
    public SymbolicExpression accept(Visitor v) {
        return v.visit(this);
    }
}
