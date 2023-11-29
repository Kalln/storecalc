package org.ioopm.calculator.ast.unary;

import org.ioopm.calculator.ast.Environment;
import org.ioopm.calculator.ast.SymbolicExpression;
import org.ioopm.calculator.ast.atom.Constant;

public class Log extends Unary {
    
    public Log(SymbolicExpression exp) {
        super(exp);
    }
    
    @Override
    public String getName() {
        return "Log";
    }
    public SymbolicExpression eval(Environment vars) {
        SymbolicExpression arg = this.exp.eval(vars);
        if (arg.isConstant()) {
            return new Constant(Math.log(arg.getValue()));
        }
        else {
            return new Log(arg);
        }
    }
}
