package inluppar.inlupp3.org.ioopm.calculator.ast.command;

import inluppar.inlupp3.org.ioopm.calculator.ast.SymbolicExpression;

public class Quit extends Command {

    public Quit() {
        super("Quit", null);
    }

    public SymbolicExpression eval() throws RuntimeException {
        throw new RuntimeException("commands may not be evaluated");
    }
    
}
