package org.ioopm.calculator.ast.unary;

import java.util.HashMap;

import org.ioopm.calculator.ast.SymbolicExpression;
import org.ioopm.calculator.ast.atom.Constant;
import org.ioopm.calculator.ast.atom.Variable;

public class Sin extends Unary {
    public Sin(SymbolicExpression exp) {
        super("Sin", exp);
    }

    @Override
    public String getName() {
        return "sin";
    }

    @Override
    public SymbolicExpression eval(HashMap<Variable, SymbolicExpression> vars) {
        SymbolicExpression arg = this.exp.eval(vars);
        if (arg.isConstant()) {
            return new Constant(Math.sin(arg.getValue()));
        }
        else {
            return new Sin(arg);
        }
    }
}
