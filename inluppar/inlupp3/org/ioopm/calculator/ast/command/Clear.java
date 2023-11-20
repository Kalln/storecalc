package org.ioopm.calculator.ast.command;

public class Clear {
    private static final Clear theInstance = new Clear();
    private Clear() {}
    public static Clear instance() { return theInstance; } 
}
