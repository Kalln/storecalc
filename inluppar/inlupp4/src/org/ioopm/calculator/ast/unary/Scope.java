package org.ioopm.calculator.ast.unary;

import org.ioopm.calculator.Visitor;
import org.ioopm.calculator.ast.SymbolicExpression;

public class Scope extends Unary {

    public Scope(SymbolicExpression exp) {
        super(exp);
    }

    @Override
    public String getName() {
        return "Scope";
    }

    @Override
    public SymbolicExpression accept(Visitor v) {
        return v.visit(this);
    }
}
