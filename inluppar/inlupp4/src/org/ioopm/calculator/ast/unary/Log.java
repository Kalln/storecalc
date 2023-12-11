package org.ioopm.calculator.ast.unary;

import org.ioopm.calculator.ast.Environment;
import org.ioopm.calculator.ast.IllegalExpressionException;
import org.ioopm.calculator.ast.SymbolicExpression;
import org.ioopm.calculator.ast.atom.Constant;
import org.ioopm.calculator.parser.SyntaxErrorException;

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
            double argumentVal = arg.getValue();
            
            // when x <= 0, Log(x) is undefined.
            if (argumentVal <= 0) {
                throw new IllegalExpressionException(String.format("Log(%f) is undefined", argumentVal));
            } 

            return new Constant(Math.log(arg.getValue()));
            
        }
        else {
            return new Log(arg);
        }
    }
}
