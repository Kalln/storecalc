package org.ioopm.calculator.ast;

import org.ioopm.calculator.ast.atom.Variable;

import java.util.HashMap;
import java.util.stream.Collectors;

public class Environment extends HashMap<Variable, SymbolicExpression> {
    @Override
    public String toString() {
        return "Variables: "
            + this.keySet()
                .stream()
                .sorted()
                .map(x -> x.getName() + " = " + this.get(x))
                .collect(Collectors.joining(", "));
    }
}
