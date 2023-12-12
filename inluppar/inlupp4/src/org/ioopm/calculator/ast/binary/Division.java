package org.ioopm.calculator.ast.binary;

import org.ioopm.calculator.Visitor;
import org.ioopm.calculator.ast.SymbolicExpression;

public class Division extends Binary{
    public Division(SymbolicExpression lhs, SymbolicExpression rhs ) {
        super("Division", lhs, rhs);
    }

    @Override
    public String getName() {
        return "/";
    }

    @Override
    public int getPriority() {
        return 25;
    }

    @Override
    public SymbolicExpression accept(Visitor v) {
        return v.visit(this);
    }
}
