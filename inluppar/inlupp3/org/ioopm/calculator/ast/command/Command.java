package org.ioopm.calculator.ast.command;

import org.ioopm.calculator.ast.SymbolicExpression;

import org.ioopm.calculator.ast.Environment;

public abstract class Command extends SymbolicExpression {
    public Command(String name, SymbolicExpression exp) {
        super(name, exp);
    }

    public SymbolicExpression eval(Environment vars) throws RuntimeException {
        throw new RuntimeException("commands may not be evaluated");
    }
}
