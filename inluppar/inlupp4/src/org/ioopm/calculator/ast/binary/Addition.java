package org.ioopm.calculator.ast.binary;

import org.ioopm.calculator.Visitor;
import org.ioopm.calculator.ast.SymbolicExpression;

public class Addition extends Binary {
    public Addition(SymbolicExpression lhs, SymbolicExpression rhs ) {
        super("Addition", lhs, rhs);
    }

    @Override
    public String getName() {
        return "+";
    }

    @Override
    public int getPriority() {
        return 10;
    }

    @Override
    public SymbolicExpression accept(Visitor v) {
        return v.visit(this);
    }
}
