package org.ioopm.calculator.ast.unary;

import org.ioopm.calculator.Visitor;
import org.ioopm.calculator.ast.SymbolicExpression;

public class Cos extends Unary {
    public Cos(SymbolicExpression exp) {
        super(exp);
    }

    @Override
    public String getName() {
        return "Cos";
    }

    @Override
    public SymbolicExpression accept(Visitor v) {
        return v.visit(this);
    }
}
