package org.ioopm.calculator.ast.command;

import org.ioopm.calculator.ast.SymbolicExpression;

public abstract class Command extends SymbolicExpression {
    public Command(String name, SymbolicExpression exp) {
        super(name, exp);
    }
}
