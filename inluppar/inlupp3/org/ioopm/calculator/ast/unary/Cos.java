package org.ioopm.calculator.ast.unary;

import java.util.HashMap;

import org.ioopm.calculator.ast.SymbolicExpression;
import org.ioopm.calculator.ast.atom.Constant;
import org.ioopm.calculator.ast.atom.Variable;

public class Cos extends Unary {
    public Cos(SymbolicExpression exp) {
        super("Cos", exp);
    }

    @Override
    public String getName() {
        return "cos";
    }

    public SymbolicExpression eval(HashMap<Variable, SymbolicExpression> vars) {
        SymbolicExpression arg = this.exp.eval(vars);
        if (arg.isConstant()) {
            return new Constant(Math.cos(arg.getValue()));
        }
        else {
            return new Cos(arg);
        }
    }
}
