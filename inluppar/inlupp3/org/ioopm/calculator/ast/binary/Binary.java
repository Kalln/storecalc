package inluppar.inlupp3.org.ioopm.calculator.ast.binary;

import inluppar.inlupp3.org.ioopm.calculator.ast.SymbolicExpression;

public abstract class Binary extends SymbolicExpression{
    private SymbolicExpression lhs;
    private SymbolicExpression rhs;

    public Binary(String name, SymbolicExpression lhs, SymbolicExpression rhs) {
        super(name, lhs, rhs);
        this.lhs = lhs; 
        this.rhs = rhs;
    }

    public SymbolicExpression getLhs() {
        return lhs;
    }

    public SymbolicExpression getRhs() {
        return rhs;
    }

    public String toString() {
        // check if one side has higher priority than the other
        if (this.lhs.getPriority() < this.getPriority()) {
            return "(" + lhs.toString() + ") " + getName() + " " + rhs.toString();
        } else if (this.rhs.getPriority() < this.getPriority()) {
            return lhs.toString() + " " + getName() + " (" + rhs.toString() + ")";
        }
        // if not, just print it out
        return lhs.toString() + " " + getName() + " " + rhs.toString();
    }
}
