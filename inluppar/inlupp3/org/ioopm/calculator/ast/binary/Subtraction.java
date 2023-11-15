package inluppar.inlupp3.org.ioopm.calculator.ast.binary;

import inluppar.inlupp3.org.ioopm.calculator.ast.SymbolicExpression;
import inluppar.inlupp3.org.ioopm.calculator.ast.atom.Constant;

public class Subtraction extends Binary {

    public Subtraction(SymbolicExpression lhs, SymbolicExpression rhs ) {
        super("Subtraciton", lhs, rhs);
    }

    @Override
    public String getName() {
        return "-";
    }

    @Override
    public int getPriority() {
        return 10;
    }

    public SymbolicExpression eval() {
        SymbolicExpression lhs = this.getLhs().eval();
        SymbolicExpression rhs = this.getRhs().eval();

        if (lhs.isConstant() && rhs.isConstant()) {
            return new Constant(lhs.getValue() - rhs.getValue());
        } else {
            return new Addition(lhs, rhs);
        }
    }
}
