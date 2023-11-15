package org.ioopm.calculator.ast.binary;

import java.util.HashMap;

import org.ioopm.calculator.ast.SymbolicExpression;
import org.ioopm.calculator.ast.atom.Variable;

public class Assignment extends Binary {
    public Assignment(SymbolicExpression lhs, SymbolicExpression rhs ) throws RuntimeException{
        super("Assignment", lhs, rhs);

        if (!lhs.isVariable()) {
            throw new RuntimeException("Left hand side of assignment must be a variable");
        }
    }


    public SymbolicExpression eval(HashMap<Variable, SymbolicExpression> vars) {
        return this.getRhs().eval(vars);
    }
}
