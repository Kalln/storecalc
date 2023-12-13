package org.ioopm.calculator.ast;

import org.ioopm.calculator.ast.atom.Variable;

import java.util.HashMap;
import java.util.TreeSet;

public class Environment extends HashMap<Variable, SymbolicExpression> {


    // TODO fix raw types
    @Override
    public String toString() {
        StringBuilder sb = new StringBuilder();
        sb.append("Variables: ");
        TreeSet<Variable> vars = new TreeSet<>(this.keySet());

        for (Variable v : vars) {
            sb.append(v.getName());
            sb.append(" = ");
            sb.append(this.get(v));
            sb.append(", ");
        }

        return sb.toString();
    }
}

