package org.ioopm.calculator.ast.binary;

import java.util.HashMap;

import org.ioopm.calculator.ast.SymbolicExpression;
import org.ioopm.calculator.ast.atom.Constant;
import org.ioopm.calculator.ast.atom.Variable;

public class Addition extends Binary {
    public Addition(SymbolicExpression lhs, SymbolicExpression rhs ) {
        super("Addition", lhs, rhs);
    }

    @Override
    public String getName() {
        return "+";
    }

    @Override
    public int getPriority() {
        return 10;
    }

    public SymbolicExpression eval(HashMap<Variable, SymbolicExpression> vars) {
        SymbolicExpression lhs = this.getLhs().eval(vars);
        SymbolicExpression rhs = this.getRhs().eval(vars);

        if (lhs.isConstant() && rhs.isConstant()) {
            return new Constant(lhs.getValue() + rhs.getValue());
        } else {
            return new Addition(lhs, rhs);
        }
    }
}
