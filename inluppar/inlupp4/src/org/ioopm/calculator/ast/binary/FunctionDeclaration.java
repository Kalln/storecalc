package org.ioopm.calculator.ast.binary;

import java.util.stream.Collectors;

import org.ioopm.calculator.Visitor;
import org.ioopm.calculator.ast.SymbolicExpression;

public class FunctionDeclaration extends Assignment {
    /**
     * Get the name of the declared function
     * @return The name of the function as a string
     */
    private String getFunctionName() {
        return this.getRhs().toString();
    }

    /**
     * Get the Function object that is declared
     * @return The Function that is declared
     */
    public SymbolicExpression getFunction() {
        return this.getLhs();
    }

    /**
     * Get a string version of the function body,
     * including the argument list, or " (invalid) invalid"
     * if the SymbolicExpression function is not a Function
     * @return The function body as a String
     */
    private String getFunctionBody() {
        var f = this.getFunction();
        return f.isFunction()
            ? "("
                + f.getArgNames()
                    .stream()
                    .map(x -> x.toString())
                    .collect(Collectors.joining(", "))
                + ")\n"
                + f.getBody()
                    .stream()
                    .map(x -> x.toString())
                    .map(x -> "    " + x)
                    .collect(Collectors.joining("\n"))
            : " *invalid*";
    }

    public FunctionDeclaration(SymbolicExpression function, SymbolicExpression name) {
        super(function, name);
    }

    @Override
    public String getName() {
        return "FunctionDeclaration";
    }

    @Override
    public String toString() {
        return "function "
            + this.getFunctionName()
            + this.getFunctionBody()
            + "\nend";
    }

    @Override
    public SymbolicExpression accept(Visitor v) {
        return v.visit(this);
    }

}
