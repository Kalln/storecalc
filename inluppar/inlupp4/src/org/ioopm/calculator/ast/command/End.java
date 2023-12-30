package org.ioopm.calculator.ast.command;

import org.ioopm.calculator.Visitor;
import org.ioopm.calculator.ast.Environment;
import org.ioopm.calculator.ast.SymbolicExpression;

public class End extends Command {

    private static final End theInstance = new End();
    private End() {}
    public static End instance() { return theInstance; }

    
    @Override
    public SymbolicExpression accept(Visitor v) {
        return v.visit(this);
    }
    
    @Override
    public void run(Environment env, int expressionsEntered, int sucessfullyEvaluated, int fullyEvaluated) {
        // TODO Auto-generated method stub
        throw new UnsupportedOperationException("Unimplemented method 'run'");
    }
}
