package org.ioopm.calculator.ast.binary;


import org.ioopm.calculator.Visitor;
import org.ioopm.calculator.ast.SymbolicExpression;

public class Multiplication extends Binary{
    public Multiplication(SymbolicExpression lhs, SymbolicExpression rhs ) {
        super("Multiplication", lhs, rhs);
    }

    @Override
    public String getName() {
        return "*";
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
