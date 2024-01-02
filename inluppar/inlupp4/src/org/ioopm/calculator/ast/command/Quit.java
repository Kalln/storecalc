package org.ioopm.calculator.ast.command;

import org.ioopm.calculator.Visitor;
import org.ioopm.calculator.ast.Environment;
import org.ioopm.calculator.ast.SymbolicExpression;

public class Quit extends Command {
    private static final Quit theInstance = new Quit();
    private Quit() {}
    public static Quit instance() { return theInstance; }

    @Override
    public void run(Environment env, int expressionsEntered, int sucessfullyEvaluated, int fullyEvaluated) {
        System.out.println("""
            -------------------------------------------------------
            Total expressions entered: %d.
            Successfully evaluated expression: %d.
            Fully evaluated expression. %d
            -------------------------------------------------------

            Closing calculator...

            -------------------------------------------------------
            """.formatted(expressionsEntered, sucessfullyEvaluated, fullyEvaluated)
        );
        System.exit(0);
    }

    @Override
    public SymbolicExpression accept(Visitor v) {
        return v.visit(this);
    }
}
