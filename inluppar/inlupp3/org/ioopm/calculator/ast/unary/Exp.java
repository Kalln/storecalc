package inluppar.inlupp3.org.ioopm.calculator.ast.unary;

import inluppar.inlupp3.org.ioopm.calculator.ast.SymbolicExpression;
import inluppar.inlupp3.org.ioopm.calculator.ast.atom.Constant;

public class Exp extends Unary {
    
    public Exp(SymbolicExpression exp) {
        super("Exp", exp);
    }

    @Override
    public String getName() {
        return "exp";
    }

    public SymbolicExpression eval() {
        SymbolicExpression arg = this.exp.eval();
        if (arg.isConstant()) {
            return new Constant(Math.exp(arg.getValue()));
        }
        else {
            return new Exp(arg);
        }
    }
}
