package inluppar.inlupp3.org.ioopm.calculator.ast.unary;

import inluppar.inlupp3.org.ioopm.calculator.ast.SymbolicExpression;

public abstract class Unary extends SymbolicExpression {
    public SymbolicExpression exp;
    
    public Unary(String name, SymbolicExpression exp) {
        super(name, exp);
        this.exp = exp;
    }

    public SymbolicExpression getExp() {
        return this.exp;
    }

    @Override 
    public String toString() {
        return this.getName() + "(" + this.exp.toString() + ")";
    }

    @Override
    public int getPriority() {
        return 70;
    }
}
