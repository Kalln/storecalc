package org.ioopm.calculator.ast.unary;


import org.ioopm.calculator.ast.Environment;
import org.ioopm.calculator.ast.SymbolicExpression;
import org.ioopm.calculator.ast.atom.Constant;

public class Exp extends Unary {

    public Exp(SymbolicExpression exp) {
        super(exp);
    }

    @Override
    public String getName() {
        return "Exp";
    }

    public SymbolicExpression eval(Environment vars) {
        SymbolicExpression arg = this.exp.eval(vars);
        if (arg.isConstant()) {
            return new Constant(Math.exp(arg.getValue()));
        }
        else {
            return new Exp(arg);
        }
    }
}
