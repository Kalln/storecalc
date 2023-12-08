package org.ioopm.calculator.ast.command;

import org.ioopm.calculator.ast.Environment;

public class Quit extends Command {

    private static final Quit theInstance = new Quit();
    private Quit() {}
    public static Quit instance() { return theInstance; }

    public void run(Environment env, int expressionsEntered, int sucessfullyEvaluated, int fullyEvaluated) {
        String endingOfProgram = """
                -------------------------------------------------------
                Total expressions entered: %d.
                Successfully evaluated expression: %d.
                Fully evaluated expression. %d
                -------------------------------------------------------

                Closing calculator...

                -------------------------------------------------------
                """.formatted(expressionsEntered, sucessfullyEvaluated, fullyEvaluated);
        System.out.println(endingOfProgram);
    }

}
