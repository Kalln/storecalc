package inluppar.inlupp3.org.ioopm.calculator.ast.unary;

import inluppar.inlupp3.org.ioopm.calculator.ast.SymbolicExpression;
import inluppar.inlupp3.org.ioopm.calculator.ast.atom.Constant;

public class Log extends Unary {
    
    public Log(SymbolicExpression exp) {
        super("Log", exp);
    }
    
    @Override
    public String getName() {
        return "log";
    }
    public SymbolicExpression eval() {
        SymbolicExpression arg = this.exp.eval();
        if (arg.isConstant()) {
            return new Constant(Math.log(arg.getValue()));
        }
        else {
            return new Log(arg);
        }
    }
}
