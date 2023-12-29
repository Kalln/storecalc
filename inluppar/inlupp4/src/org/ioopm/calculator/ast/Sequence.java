package org.ioopm.calculator.ast;

import java.util.ArrayList;
import java.util.stream.Collectors;

public class Sequence extends ArrayList<SymbolicExpression> {
    @Override
    public String toString() {
        return this.stream()
            .map(x -> x.toString())
            .map(x -> "    " + x)
            .collect(Collectors.joining("\n"));
    }
}
