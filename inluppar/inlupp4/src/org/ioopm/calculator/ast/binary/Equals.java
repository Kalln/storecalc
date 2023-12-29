package org.ioopm.calculator.ast.binary;

import org.ioopm.calculator.Visitor;
import org.ioopm.calculator.ast.SymbolicExpression;

public class Equals extends Condition {

    public Equals(SymbolicExpression lhs, SymbolicExpression rhs) {
        super(lhs, rhs);
    }

    @Override
    public String getName() {
        return "==";
    }

    @Override
    public SymbolicExpression accept(Visitor v) {
        return v.visit(this);
    }

}
