package org.ioopm.calculator.ast.atom;

import org.ioopm.calculator.Visitor;
import org.ioopm.calculator.ast.SymbolicExpression;

public class True extends Boolean {
    @Override
    public SymbolicExpression accept(Visitor v) {
        return v.visit(this);
    }

    @Override
    public boolean isTrue() {
        return true;
    }

    @Override
    public boolean equals(Object other) {
        return other instanceof True;
    }

    @Override
    public String toString() {
        return "True";
    }
}
