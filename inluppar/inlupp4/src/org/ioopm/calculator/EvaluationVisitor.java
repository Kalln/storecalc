package org.ioopm.calculator;

import java.util.Locale;

import org.ioopm.calculator.ast.Environment;
import org.ioopm.calculator.ast.IllegalExpressionException;
import org.ioopm.calculator.ast.SymbolicExpression;
import org.ioopm.calculator.ast.atom.Constant;
import org.ioopm.calculator.ast.atom.NamedConstant;
import org.ioopm.calculator.ast.atom.Variable;
import org.ioopm.calculator.ast.binary.Addition;
import org.ioopm.calculator.ast.binary.Assignment;
import org.ioopm.calculator.ast.binary.Division;
import org.ioopm.calculator.ast.binary.Multiplication;
import org.ioopm.calculator.ast.binary.Subtraction;
import org.ioopm.calculator.ast.command.Clear;
import org.ioopm.calculator.ast.command.Quit;
import org.ioopm.calculator.ast.command.Vars;
import org.ioopm.calculator.ast.unary.Cos;
import org.ioopm.calculator.ast.unary.Exp;
import org.ioopm.calculator.ast.unary.Log;
import org.ioopm.calculator.ast.unary.Negation;
import org.ioopm.calculator.ast.unary.Sin;

public class EvaluationVisitor implements Visitor {
    private Environment env = null;

    /*
     * Evaluate a given SymbolicExpression topLevel. This will "find" the correct 
     * accept() corresponding to that Expression type and return it's evaluated 
     * expression.
     */
    public SymbolicExpression evaluate(SymbolicExpression topLevel, Environment env) {
        this.env = env;
        return topLevel.accept(this);
    }

    // This method gets called from Addition.accept(Visitor v) -- you should
    // be able to see from the eval() methods how these should behave (i.e.,
    // compare this method with your Addition::eval() and Symbolic.addition)
    public SymbolicExpression visit(Addition n) {
        var lhs = n.getLhs().accept(this);
        var rhs = n.getRhs().accept(this);

        return lhs.isConstant() && rhs.isConstant()
            ? new Constant(lhs.getValue() + rhs.getValue())
            : new Addition(lhs, rhs);
    }

    @Override
    public SymbolicExpression visit(Subtraction n) {
        var lhs = n.getLhs().accept(this);
        var rhs = n.getRhs().accept(this);

        return lhs.isConstant() && rhs.isConstant()
            ? new Constant(lhs.getValue() - rhs.getValue())
            : new Subtraction(lhs, rhs);
    }

    @Override
    public SymbolicExpression visit(Multiplication n) {
        var lhs = n.getLhs().accept(this);
        var rhs = n.getRhs().accept(this);

        return lhs.isConstant() && rhs.isConstant()
            ? new Constant(lhs.getValue() * rhs.getValue())
            : new Multiplication(lhs, rhs);
    }

    @Override
    public SymbolicExpression visit(Division n) {
        var lhs = n.getLhs().accept(this);
        var rhs = n.getRhs().accept(this);

        // division by 0 is not possible.
        if (rhs.isConstant() && rhs.getValue() == 0) {
            throw new IllegalExpressionException("Dividing by 0 is not possible.");
        }

        return lhs.isConstant() && rhs.isConstant()
            ? new Constant(lhs.getValue() / rhs.getValue())
            : new Division(lhs, rhs);
    }

    @Override
    public SymbolicExpression visit(Assignment n) {
        var key = n.getRhs();
        if (key instanceof Variable v) {
            var lhsResult = n.getLhs().accept(this);
            env.put(v, lhsResult); // TODO type check...
            return lhsResult;
        } else {
            throw new RuntimeException("Right hand side was not a variable.");
        }
    }

    @Override
    public SymbolicExpression visit(Clear n) {
        throw new RuntimeException("Commands may not be evaluated");
    }

    @Override
    public SymbolicExpression visit(Quit n) {
        throw new RuntimeException("Commands may not be evaluated");
    }

    @Override
    public SymbolicExpression visit(Vars n) {
        throw new RuntimeException("Commands may not be evaluated");
    }

    @Override
    public SymbolicExpression visit(Constant n) {
        return n;
    }

    @Override
    public SymbolicExpression visit(NamedConstant n) {
        return new Constant(n.getValue());
    }

    @Override
    public SymbolicExpression visit(Variable n) {
        var result = env.get(n);
        return result != null ? result : n;
    }

    @Override
    public SymbolicExpression visit(Cos n) {
        var arg = n.getExp().accept(this);
        return arg.isConstant()
            ? new Constant(Math.cos(arg.getValue()))
            : n;
    }

    @Override
    public SymbolicExpression visit(Exp n) {
        var arg = n.getExp().accept(this);
        return arg.isConstant()
            ? new Constant(Math.exp(arg.getValue()))
            : n;
    }

    @Override
    public SymbolicExpression visit(Log n) {
        var arg = n.getExp().accept(this);
        if (arg.isConstant()) {
            var argumentVal = arg.getValue();

            // when x <= 0, Log(x) is undefined.
            if (argumentVal <= 0) {
                throw new IllegalExpressionException(
                    String.format(Locale.US, "Log(%f) is undefined", argumentVal)
                );
            }

            return new Constant(Math.log(argumentVal));
        } else {
            return n;
        }
    }

    @Override
    public SymbolicExpression visit(Negation n) {
        var arg = n.getExp().accept(this);
        return arg.isConstant()
            ? new Constant(-arg.getValue())
            : n;
    }

    @Override
    public SymbolicExpression visit(Sin n) {
        var arg = n.getExp().accept(this);
        return arg.isConstant()
            ? new Constant(Math.sin(arg.getValue()))
            : n;
    }
}
