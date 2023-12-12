package org.ioopm.calculator.ast.unary;


import org.ioopm.calculator.Visitor;
import org.ioopm.calculator.ast.SymbolicExpression;

public class Exp extends Unary {

    public Exp(SymbolicExpression exp) {
        super(exp);
    }

    @Override
    public String getName() {
        return "Exp";
    }

    @Override
    public SymbolicExpression accept(Visitor v) {
        return v.visit(this);
    }
}
