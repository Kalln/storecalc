package org.ioopm.calculator.ast;

import java.util.List;
import java.util.stream.Collectors;

import org.ioopm.calculator.Visitor;

public class FunctionCall extends SymbolicExpression {
    private SymbolicExpression function;
    private List<SymbolicExpression> args;

    public FunctionCall(SymbolicExpression function, List<SymbolicExpression> args) {
        this.function = function;
        this.args = args;
    }

    public SymbolicExpression getFunction() {
        return function;
    }

    public List<SymbolicExpression> getArgs() {
        return args;
    }

    @Override
    public String toString() {
        return function.toString()
            + "("
            + args.stream()
                .map(x -> x.toString())
                .collect(Collectors.joining(", "))
            + ")";
    }

    @Override
    public boolean equals(Object other) {
        if (other instanceof FunctionCall f) {
            return function.equals(f.getFunction())
                && args.equals(f.getArgs());
        } else {
            return false;
        }
    }

    @Override
    public SymbolicExpression accept(Visitor v) {
        return v.visit(this);
    }
}
