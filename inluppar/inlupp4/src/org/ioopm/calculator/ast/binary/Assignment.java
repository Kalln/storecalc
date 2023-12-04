package org.ioopm.calculator.ast.binary;

import org.ioopm.calculator.ast.Environment;
import org.ioopm.calculator.ast.SymbolicExpression;
import org.ioopm.calculator.ast.atom.Variable;

public class Assignment extends Binary {
    public Assignment(SymbolicExpression res, SymbolicExpression key ) throws RuntimeException{
        super("Assignment", res, key);

        if (!key.isVariable()) {
            throw new RuntimeException("Left hand side of assignment must be a variable");
        }
    }

    public String getName() {
        return "=";
    }


    public SymbolicExpression eval(Environment vars) {
        SymbolicExpression key = this.getRhs();
        if (key instanceof Variable v) {
            vars.put(v, this.getLhs().eval(vars));
        } else {
            throw new RuntimeException("The rhs was not a variable.");
        }
        return key;
    }
}
