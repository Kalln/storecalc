package org.ioopm.calculator.ast.unary;

import org.ioopm.calculator.ast.Environment;
import org.ioopm.calculator.ast.SymbolicExpression;
import org.ioopm.calculator.ast.atom.Constant;

public class Sin extends Unary {
    public Sin(SymbolicExpression exp) {
        super(exp);
    }

    @Override
    public String getName() {
        return "Sin";
    }

    @Override
    public SymbolicExpression eval(Environment vars) {
        SymbolicExpression arg = this.exp.eval(vars);
        if (arg.isConstant()) {
            return new Constant(Math.sin(arg.getValue()));
        }
        else {
            return new Sin(arg);
        }
    }
}
