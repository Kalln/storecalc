package inluppar.inlupp3.org.ioopm.calculator.ast.unary;

import inluppar.inlupp3.org.ioopm.calculator.ast.SymbolicExpression;
import inluppar.inlupp3.org.ioopm.calculator.ast.atom.Constant;

public class Cos extends Unary {
    public Cos(SymbolicExpression exp) {
        super("Cos", exp);
    }

    @Override
    public String getName() {
        return "cos";
    }

    public SymbolicExpression eval() {
        SymbolicExpression arg = this.exp.eval();
        if (arg.isConstant()) {
            return new Constant(Math.cos(arg.getValue()));
        }
        else {
            return new Cos(arg);
        }
    }
}
