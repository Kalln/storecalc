package inluppar.inlupp3.org.ioopm.calculator.ast.unary;

import inluppar.inlupp3.org.ioopm.calculator.ast.SymbolicExpression;
import inluppar.inlupp3.org.ioopm.calculator.ast.atom.Constant;

public class Sin extends Unary {
    public Sin(SymbolicExpression exp) {
        super("Sin", exp);
    }

    @Override
    public String getName() {
        return "sin";
    }

    public SymbolicExpression eval() {
        SymbolicExpression arg = this.exp.eval();
        if (arg.isConstant()) {
            return new Constant(Math.sin(arg.getValue()));
        }
        else {
            return new Sin(arg);
        }
    }
}
