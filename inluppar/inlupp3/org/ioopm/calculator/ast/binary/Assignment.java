package org.ioopm.calculator.ast.binary;

import org.ioopm.calculator.ast.Environment;
import org.ioopm.calculator.ast.SymbolicExpression;

public class Assignment extends Binary {
    public Assignment(SymbolicExpression lhs, SymbolicExpression rhs ) throws RuntimeException{
        super("Assignment", lhs, rhs);

        if (!lhs.isVariable()) {
            throw new RuntimeException("Left hand side of assignment must be a variable");
        }
    }


    public SymbolicExpression eval(Environment vars) {
        return this.getRhs().eval(vars);
    }
}
