package org.ioopm.calculator.ast.atom;

import org.ioopm.calculator.Visitor;
import org.ioopm.calculator.ast.SymbolicExpression;

public class False extends Boolean {
    public False() {

    }

    @Override
    public boolean isTrue() {
        return false;
    }

    @Override
    public SymbolicExpression accept(Visitor v) {
        return v.visit(this);
    }

    @Override
    public boolean equals(Object other) {
        return other instanceof False;
    }

    @Override
    public String toString() {
        return "False";
    }
}
