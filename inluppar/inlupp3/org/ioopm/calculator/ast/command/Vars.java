package inluppar.inlupp3.org.ioopm.calculator.ast.command;

import inluppar.inlupp3.org.ioopm.calculator.ast.SymbolicExpression;

public class Vars extends Command {

    public Vars() {
        super("Vars", null);
    }

    public SymbolicExpression eval() throws RuntimeException {
        throw new RuntimeException("commands may not be evaluated");
    }
    
}
