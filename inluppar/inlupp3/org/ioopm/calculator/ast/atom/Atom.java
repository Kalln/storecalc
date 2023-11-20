package org.ioopm.calculator.ast.atom;

import java.util.HashMap;

import org.ioopm.calculator.ast.Environment;
import org.ioopm.calculator.ast.SymbolicExpression;

public abstract class Atom extends SymbolicExpression {

    public Atom(String name) {
        super(name);
    }


    public SymbolicExpression eval(Environment vars) {
        if (this.isConstant()) {
            return new Constant(this.getValue());
        } else {
            return this;
        }
    }
    
}
