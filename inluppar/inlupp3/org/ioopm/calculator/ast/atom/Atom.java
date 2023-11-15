package inluppar.inlupp3.org.ioopm.calculator.ast.atom;

import inluppar.inlupp3.org.ioopm.calculator.ast.SymbolicExpression;

public abstract class Atom extends SymbolicExpression {

    public Atom(String name) {
        super(name);
    }


    public SymbolicExpression eval() {
        if (this.isConstant()) {
            return new Constant(this.getValue());
        } else {
            return this;
        }
    }
    
}
