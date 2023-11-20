package org.ioopm.calculator.ast.unary;

import org.ioopm.calculator.ast.Environment;
import org.ioopm.calculator.ast.SymbolicExpression;
import org.ioopm.calculator.ast.atom.Constant;

public class Cos extends Unary {
    public Cos(SymbolicExpression exp) {
        super("Cos", exp);
    }

    @Override
    public String getName() {
        return "Cos";
    }

    public SymbolicExpression eval(Environment vars) {
        SymbolicExpression arg = this.exp.eval(vars);
        if (arg.isConstant()) {
            return new Constant(Math.cos(arg.getValue()));
        }
        else {
            return new Cos(arg);
        }
    }
}
