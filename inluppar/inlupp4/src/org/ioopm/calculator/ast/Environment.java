package org.ioopm.calculator.ast;

import org.ioopm.calculator.ast.atom.Variable;

import java.util.HashMap;
import java.util.TreeSet;

public class Environment extends HashMap<Variable<String>, SymbolicExpression> {

    @Override
    public String toString() {
        StringBuilder sb = new StringBuilder();
        sb.append("Variables: ");
        TreeSet<Variable<String>> vars = new TreeSet<>(this.keySet());

        for (Variable<String> v : vars) {
            sb.append(v.getName());
            sb.append(" = ");
            sb.append(this.get(v));
            sb.append(", ");
        }

        return sb.toString();
    }
}
