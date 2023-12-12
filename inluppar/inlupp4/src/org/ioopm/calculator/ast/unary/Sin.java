package org.ioopm.calculator.ast.unary;

import org.ioopm.calculator.Visitor;
import org.ioopm.calculator.ast.SymbolicExpression;

public class Sin extends Unary {
    public Sin(SymbolicExpression exp) {
        super(exp);
    }

    @Override
    public String getName() {
        return "Sin";
    }

    @Override
    public SymbolicExpression accept(Visitor v) {
        return v.visit(this);
    }
}
