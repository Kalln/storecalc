package inluppar.inlupp3.org.ioopm.calculator.ast.binary;

import inluppar.inlupp3.org.ioopm.calculator.ast.SymbolicExpression;
import inluppar.inlupp3.org.ioopm.calculator.ast.atom.Constant;

public class Division extends Binary{
    public Division(SymbolicExpression lhs, SymbolicExpression rhs ) {
        super("Division", lhs, rhs);
    }

    @Override
    public String getName() {
        return "/";
    }

    @Override
    public int getPriority() {
        return 25;
    }

    public SymbolicExpression eval() {
        SymbolicExpression lhs = this.getLhs().eval();
        SymbolicExpression rhs = this.getRhs().eval();

        if (lhs.isConstant() && rhs.isConstant()) {
            return new Constant(lhs.getValue() / rhs.getValue());
        } else {
            return new Addition(lhs, rhs);
        }
    }
}
