package org.ioopm.calculator.ast.command;

import org.ioopm.calculator.Visitor;
import org.ioopm.calculator.ast.Environment;
import org.ioopm.calculator.ast.SymbolicExpression;

public class Vars extends Command {
    private static final Vars theInstance = new Vars();
    private Vars() {}
    public static Vars instance() { return theInstance; }

    @Override
    public void run(Environment env, int notused_1, int notused_2, int notused_3) {
        System.out.println(env);
    }

    @Override
    public SymbolicExpression accept(Visitor v) {
        return v.visit(this);
    }
}
