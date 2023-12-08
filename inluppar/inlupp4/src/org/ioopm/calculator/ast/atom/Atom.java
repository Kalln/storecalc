package org.ioopm.calculator.ast.atom;

import org.ioopm.calculator.ast.Environment;
import org.ioopm.calculator.ast.SymbolicExpression;

public abstract class Atom extends SymbolicExpression {

    public SymbolicExpression eval(Environment vars) {
        if (this.isConstant()) {
            return new Constant(this.getValue());
        } else {
            return this;
        }
    }

}
