package org.ioopm.calculator.ast.binary;

import org.ioopm.calculator.ast.Environment;
import org.ioopm.calculator.ast.IllegalAssignmentException;
import org.ioopm.calculator.ast.SymbolicExpression;
import org.ioopm.calculator.ast.atom.Variable;

public class Assignment extends Binary {
    public Assignment(SymbolicExpression res, SymbolicExpression key) throws IllegalAssignmentException{
        super("Assignment", res, key);

        if (!key.isVariable()) {
            throw new IllegalAssignmentException("Right hand side must be a variable.");
        }
    }

    public String getName() {
        return "=";
    }

    public SymbolicExpression eval(Environment vars) {
        SymbolicExpression key = this.getRhs();
        if (key instanceof Variable v) {
            var lhsResult = this.getLhs().eval(vars);
            vars.put(v, lhsResult);
            return lhsResult;
        } else {
            throw new RuntimeException("The rhs was not a variable.");
        }
    }
}
