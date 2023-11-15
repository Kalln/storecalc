package org.ioopm.calculator.ast.unary;

import java.util.HashMap;

import org.ioopm.calculator.ast.SymbolicExpression;
import org.ioopm.calculator.ast.atom.Constant;
import org.ioopm.calculator.ast.atom.Variable;

public class Log extends Unary {
    
    public Log(SymbolicExpression exp) {
        super("Log", exp);
    }
    
    @Override
    public String getName() {
        return "log";
    }
    public SymbolicExpression eval(HashMap<Variable, SymbolicExpression> vars) {
        SymbolicExpression arg = this.exp.eval(vars);
        if (arg.isConstant()) {
            return new Constant(Math.log(arg.getValue()));
        }
        else {
            return new Log(arg);
        }
    }
}
