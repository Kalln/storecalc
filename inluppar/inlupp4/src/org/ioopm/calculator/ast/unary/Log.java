package org.ioopm.calculator.ast.unary;

import org.ioopm.calculator.Visitor;
import org.ioopm.calculator.ast.SymbolicExpression;

public class Log extends Unary {

    public Log(SymbolicExpression exp) {
        super(exp);
    }

    @Override
    public String getName() {
        return "Log";
    }

    @Override
    public SymbolicExpression accept(Visitor v) {
        return v.visit(this);
    }
}
