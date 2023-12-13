package org.ioopm.calculator.ast.command;

import org.ioopm.calculator.Visitor;
import org.ioopm.calculator.ast.Environment;
import org.ioopm.calculator.ast.SymbolicExpression;

public class Clear extends Command {
    private static final Clear theInstance = new Clear();
    private Clear() {}
    public static Clear instance() { return theInstance; }

    @Override
    public void run(Environment env, int notused_1, int notused_2, int notused_3) {
        env.clear();
    }

    @Override
    public SymbolicExpression accept(Visitor v) {
        return v.visit(this);
    }
}
