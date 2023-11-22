package org.ioopm.calculator.ast.command;


public class Vars extends Command {
    private static final Vars theInstance = new Vars();
    private Vars() {}
    public static Vars instance() { return theInstance; }

    
}
