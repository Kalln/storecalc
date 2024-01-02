package org.ioopm.calculator.ast;

import java.util.List;

import org.ioopm.calculator.Visitor;
import org.ioopm.calculator.ast.atom.Variable;


public abstract class SymbolicExpression {

    /**
     * Tells the Evaluator what type of SymbolicExpression should be evaluated.
     * @param v {Visitor} - A evaluatable Symbolicexpression.
     * @return Evaluated SymbolicExpression
     */
    public abstract SymbolicExpression accept(Visitor v);

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
    public boolean isBoolean() {
        return false;
    }
    public boolean isNamedConstant() {
        return false;
    }
    public boolean isVariable() {
        return false;
    }
    public boolean isFunction() {
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

    public boolean isTrue() {
        throw new RuntimeException("isTrue() not implemented for non booleans");
    }

    public Sequence getBody() {
        throw new RuntimeException("getBody() not implemented for non functions");
    }
    public List<Variable> getArgNames() {
        throw new RuntimeException("getArgNames() not implemented for non functions");
    }
    public void run(Environment env, int expressionsEntered, int sucessfullyEvaluated, int fullyEvaluated) {
        throw new RuntimeException("run() not implemented for non commands");
    }
}
