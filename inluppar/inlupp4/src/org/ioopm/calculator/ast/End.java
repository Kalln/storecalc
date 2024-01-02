package org.ioopm.calculator.ast;

import org.ioopm.calculator.Visitor;

public class End extends SymbolicExpression {
    public End() {

    }

    @Override
    public SymbolicExpression accept(Visitor v) {
        return v.visit(this);
    }
}
