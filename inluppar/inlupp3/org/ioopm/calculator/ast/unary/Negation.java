package org.ioopm.calculator.ast.unary;

import org.ioopm.calculator.ast.Environment;
import org.ioopm.calculator.ast.SymbolicExpression;
import org.ioopm.calculator.ast.atom.Constant;

public class Negation extends Unary {

    public Negation(SymbolicExpression exp) {
        super("Negation", exp);
    }

    @Override 
    public String getName() {
        return "Neg";
    }

    @Override
    public String toString() {
        return "-" + exp.getValue();
    }

    public SymbolicExpression eval(Environment vars) {
        SymbolicExpression arg = this.exp.eval(vars);

        if (arg.isConstant()) {
            var tmp = exp.getValue();
            tmp = 2 * tmp;
            return new Constant(exp.getValue() - tmp);
        }
        return new Negation(arg);
    }

}
