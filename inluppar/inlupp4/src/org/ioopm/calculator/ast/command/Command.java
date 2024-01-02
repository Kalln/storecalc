package org.ioopm.calculator.ast.command;

import org.ioopm.calculator.ast.SymbolicExpression;

public abstract class Command extends SymbolicExpression {
    @Override
    public boolean isCommand() {
        return true;
    }
}
