package org.ioopm.calculator.ast.binary;

import org.ioopm.calculator.ast.SymbolicExpression;

public abstract class Condition extends Binary {
    public Condition(SymbolicExpression lhs, SymbolicExpression rhs) {
        super(lhs, rhs);
    }
}
