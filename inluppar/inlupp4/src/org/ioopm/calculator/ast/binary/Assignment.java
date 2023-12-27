package org.ioopm.calculator.ast.binary;

import org.ioopm.calculator.Visitor;
import org.ioopm.calculator.ast.SymbolicExpression;

public class Assignment extends Binary {
    public Assignment(SymbolicExpression res, SymbolicExpression key) {
        super(res, key);
    }

    @Override
    public int getPriority() {
        return 5;
    }

    @Override
    public String getName() {
        return "=";
    }

    @Override
    public SymbolicExpression accept(Visitor v) {
        return v.visit(this);
    }
}
