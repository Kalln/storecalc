package org.ioopm.calculator.ast.command;

import java.util.Map;

import org.ioopm.calculator.Visitor;
import org.ioopm.calculator.ast.Environment;
import org.ioopm.calculator.ast.SymbolicExpression;
import org.ioopm.calculator.ast.atom.Variable;

public class Vars extends Command {
    private static final Vars theInstance = new Vars();
    private Vars() {}
    public static Vars instance() { return theInstance; }


    // TODO fix
    @Override
    public void run(Environment env, int notused_1, int notused_2, int notused_3) {
        for (Map.Entry<Variable, SymbolicExpression> entry : env.entrySet()) {
            System.out.println(entry.getKey() + " -> " + entry.getValue());
        }
    }

    @Override
    public SymbolicExpression accept(Visitor v) {
        return v.visit(this);
    }
}
