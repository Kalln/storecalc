package org.ioopm.calculator.ast;

import java.util.List;
import java.util.stream.Collectors;

import org.ioopm.calculator.Visitor;
import org.ioopm.calculator.ast.atom.Variable;

public class Function extends SymbolicExpression {
    private Sequence body;
    private List<Variable> args;

    public Function(List<Variable> args, Sequence body) {
        this.args = args;
        this.body = body;
    }

    @Override
    public Sequence getBody() {
        return body;
    }

    @Override
    public List<Variable> getArgNames() {
        return args;
    }

    @Override
    public boolean isFunction() {
        return true;
    }

    @Override
    public boolean equals(Object other) {
        if (other instanceof Function f) {
            return body.equals(f.getBody())
                && args.equals(f.getArgNames());
        } else {
            return false;
        }
    }

    @Override
    public String toString() {
        return "function("
            + args.stream()
                .map(x -> x.toString())
                .collect(Collectors.joining(", "))
            + ")\n"
            + body.toString() + "\n"
            + "end";
    }

    @Override
    public SymbolicExpression accept(Visitor v) {
        return v.visit(this);
    }
}
