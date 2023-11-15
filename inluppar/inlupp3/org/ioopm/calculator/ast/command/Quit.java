package org.ioopm.calculator.ast.command;

import java.util.HashMap;

import org.ioopm.calculator.ast.SymbolicExpression;
import org.ioopm.calculator.ast.atom.Variable;

public class Quit extends Command {

    public Quit() {
        super("Quit", null);
    }

    @Override
    public SymbolicExpression eval(HashMap<Variable, SymbolicExpression> vars ) throws RuntimeException {
        throw new RuntimeException("commands may not be evaluated");
    }
    
}
