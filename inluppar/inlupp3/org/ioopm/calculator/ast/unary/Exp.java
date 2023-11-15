package org.ioopm.calculator.ast.unary;

import java.util.HashMap;

import org.ioopm.calculator.ast.SymbolicExpression;
import org.ioopm.calculator.ast.atom.Constant;
import org.ioopm.calculator.ast.atom.Variable;

public class Exp extends Unary {
    
    public Exp(SymbolicExpression exp) {
        super("Exp", exp);
    }

    @Override
    public String getName() {
        return "exp";
    }

    public SymbolicExpression eval(HashMap<Variable, SymbolicExpression> vars) {
        SymbolicExpression arg = this.exp.eval(vars);
        if (arg.isConstant()) {
            return new Constant(Math.exp(arg.getValue()));
        }
        else {
            return new Exp(arg);
        }
    }
}
