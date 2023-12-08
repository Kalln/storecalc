package org.ioopm.calculator.ast;



public abstract class SymbolicExpression {


    /**
     * Abstract method that forces class-children to have eval method that will evaluate the given expression.
     * @param vars - Environment containing the variables in the running program
     * @return A new SymbolicExpression.
     */
    public abstract SymbolicExpression eval(Environment vars);


    /**
     * By defualt all isX methods are false, these has to been overwritten by childrens of this class.
     *
     */
    public boolean isCommand() {
        return false;
    }
    public boolean isConstant() {
        return false;
    }

    public boolean isVariable() {
        return false;
    }

    /**
     * Priority that determines how paranthesis are printed.
     * @return a number priority, If a operation has higher priority the paranthesis will be printed on that expression.
     */
    public int getPriority() {
            return 100;
        }

    public String getName() {
        throw new RuntimeException("getName() not implemented for expressions with no operator");
    }

    public double getValue() {
        throw new RuntimeException("getValue() not implemented for non constants");
    }
}
