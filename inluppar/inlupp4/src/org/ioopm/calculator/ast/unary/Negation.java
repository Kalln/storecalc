package org.ioopm.calculator.ast.unary;

import org.ioopm.calculator.Visitor;
import org.ioopm.calculator.ast.SymbolicExpression;

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

    @Override
    public SymbolicExpression accept(Visitor v) {
        return v.visit(this);
    }
}
