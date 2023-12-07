package org.ioopm.calculator.ast.unary;

import org.ioopm.calculator.ast.Environment;
import org.ioopm.calculator.ast.SymbolicExpression;
import org.ioopm.calculator.ast.atom.Constant;

public class Negation extends Unary {

    public Negation(SymbolicExpression exp) {
        super(exp);
    }

    @Override
    public String getName() {
        return "Neg";
    }

    @Override
    public String toString() {
        return this.getPriority() > exp.getPriority()
            ? "-(" + exp.toString() + ")"
            : "-" + exp.toString();
    }

    public SymbolicExpression eval(Environment vars) {
        SymbolicExpression arg = this.exp.eval(vars);

        if (arg.isConstant()) {

            // double the current value, then get the current val and subtract the double of the current value.
            // to make it negative.
            var tmp = exp.getValue();
            tmp = 2 * tmp;
            return new Constant(exp.getValue() - tmp);
        }
        return new Negation(arg);
    }

}
