package org.ioopm.calculator;

import java.util.Locale;

import org.ioopm.calculator.ast.Conditional;
import org.ioopm.calculator.ast.Function;
import org.ioopm.calculator.ast.FunctionCall;
import org.ioopm.calculator.ast.IllegalExpressionException;
import org.ioopm.calculator.ast.StackEnvironment;
import org.ioopm.calculator.ast.SymbolicExpression;
import org.ioopm.calculator.ast.atom.Constant;
import org.ioopm.calculator.ast.atom.False;
import org.ioopm.calculator.ast.atom.NamedConstant;
import org.ioopm.calculator.ast.atom.True;
import org.ioopm.calculator.ast.atom.Variable;
import org.ioopm.calculator.ast.binary.Addition;
import org.ioopm.calculator.ast.binary.Assignment;
import org.ioopm.calculator.ast.binary.Division;
import org.ioopm.calculator.ast.binary.Equals;
import org.ioopm.calculator.ast.binary.FunctionDeclaration;
import org.ioopm.calculator.ast.binary.LessThan;
import org.ioopm.calculator.ast.binary.LessThanOrEquals;
import org.ioopm.calculator.ast.binary.GreaterThan;
import org.ioopm.calculator.ast.binary.GreaterThanOrEquals;
import org.ioopm.calculator.ast.binary.Multiplication;
import org.ioopm.calculator.ast.binary.NotEquals;
import org.ioopm.calculator.ast.binary.Subtraction;
import org.ioopm.calculator.ast.command.Clear;
import org.ioopm.calculator.ast.command.End;
import org.ioopm.calculator.ast.command.Quit;
import org.ioopm.calculator.ast.command.Vars;
import org.ioopm.calculator.ast.unary.Cos;
import org.ioopm.calculator.ast.unary.Exp;
import org.ioopm.calculator.ast.unary.Log;
import org.ioopm.calculator.ast.unary.Negation;
import org.ioopm.calculator.ast.unary.Scope;
import org.ioopm.calculator.ast.unary.Sin;

public class EvaluationVisitor implements Visitor {
    private StackEnvironment env = null;

    /*
     * Evaluate a given SymbolicExpression topLevel. This will "find" the correct
     * accept() corresponding to that Expression type and return it's evaluated
     * expression.
     */
    public SymbolicExpression evaluate(SymbolicExpression topLevel, StackEnvironment env) {
        this.env = env;
        env.clearStack(); // Clear all environments except the global one
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
            env.put(v, lhsResult);
            return lhsResult;
        } else {
            throw new RuntimeException("Right hand side was not a variable.");
        }
    }

    @Override
    public SymbolicExpression visit(FunctionDeclaration n) {
        // A FunctionDeclaration is just a special type of Assignment
        var key = n.getRhs(); // TODO: do we need a new method to get the function name i.e. "rhs"
        var function = n.getFunction();
        if (function.isFunction()) {
            if (key instanceof Variable v) {
                var lhsResult = n.getLhs().accept(this);
                env.put(v, lhsResult);
                return lhsResult;
            } else {
                throw new RuntimeException("Right hand side was not a variable.");
            }
        } else {
            throw new IllegalExpressionException("Attempt to declare non-function in function declaration");
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
    public SymbolicExpression visit(Conditional n) {
        var condition = n.getCondition().accept(this);
        if (condition.isBoolean()) {
            return condition.isTrue()
                ? n.getIfClause().accept(this)
                : n.getElseClause().accept(this);
        } else {
            throw new RuntimeException("Unable to evaluate condition");
        }
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
    public SymbolicExpression visit(Scope n) {
        env.pushEnvironment();
        var arg = n.getExp().accept(this);
        env.popEnvironment();
        return arg.isConstant()
            ? arg
            : n;
    }

    @Override
    public SymbolicExpression visit(Sin n) {
        var arg = n.getExp().accept(this);
        return arg.isConstant()
            ? new Constant(Math.sin(arg.getValue()))
            : n;
    }

    @Override
    public SymbolicExpression visit(LessThan n) {
        var lhs = n.getLhs().accept(this);
        var rhs = n.getRhs().accept(this);

        return lhs.isConstant() && rhs.isConstant()
            ? lhs.getValue() < rhs.getValue()
                ? new True()
                : new False()
            : new LessThan(lhs, rhs);
    }

    @Override
    public SymbolicExpression visit(LessThanOrEquals n) {
        var lhs = n.getLhs().accept(this);
        var rhs = n.getRhs().accept(this);

        return lhs.isConstant() && rhs.isConstant()
            ? lhs.getValue() <= rhs.getValue()
                ? new True()
                : new False()
            : new LessThanOrEquals(lhs, rhs);
    }

    @Override
    public SymbolicExpression visit(GreaterThan n) {
        var lhs = n.getLhs().accept(this);
        var rhs = n.getRhs().accept(this);

        return lhs.isConstant() && rhs.isConstant()
            ? lhs.getValue() > rhs.getValue()
                ? new True()
                : new False()
            : new LessThanOrEquals(lhs, rhs);
    }

    @Override
    public SymbolicExpression visit(GreaterThanOrEquals n) {
        var lhs = n.getLhs().accept(this);
        var rhs = n.getRhs().accept(this);

        return lhs.isConstant() && rhs.isConstant()
            ? lhs.getValue() >= rhs.getValue()
                ? new True()
                : new False()
            : new LessThanOrEquals(lhs, rhs);
    }

    @Override
    public SymbolicExpression visit(Equals n) {
        var lhs = n.getLhs().accept(this);
        var rhs = n.getRhs().accept(this);

        return lhs.isConstant() && rhs.isConstant()
            ? lhs.getValue() == rhs.getValue()
                ? new True()
                : new False()
            : new LessThanOrEquals(lhs, rhs);
    }

    @Override
    public SymbolicExpression visit(NotEquals n) {
        var lhs = n.getLhs().accept(this);
        var rhs = n.getRhs().accept(this);

        return lhs.isConstant() && rhs.isConstant()
            ? lhs.getValue() != rhs.getValue()
                ? new True()
                : new False()
            : new LessThanOrEquals(lhs, rhs);
    }

    @Override
    public SymbolicExpression visit(False n) {
        return n;
    }

    @Override
    public SymbolicExpression visit(True n) {
        return n;
    }

    @Override
    public SymbolicExpression visit(FunctionCall n) {
        var f = n.getFunction().accept(this);
        if (f.isFunction()) {
            var functionArgs = f.getArgNames();
            var callArgs = n.getArgs()
                .stream()
                .map(x -> x.accept(this))
                .toList();

            if (functionArgs.size() != callArgs.size()) {
                throw new IllegalExpressionException("Function argument list size mismatch");
            }

            if (
                ! callArgs.stream().allMatch(
                    x -> x.isConstant() || x.isFunction() || x.isBoolean()
                )
            ) {
                return new FunctionCall(f, callArgs);
            } else {
                env.pushEnvironment();
                for (var i = 0; i < functionArgs.size(); ++i) {
                    env.put(functionArgs.get(i), callArgs.get(i));
                }

                var body = f.getBody();
                // Default return value TODO: should we disallow empty functions?
                SymbolicExpression result = new False();
                for (var statement : body) {
                    result = statement.accept(this);
                }
                env.popEnvironment();

                return result;
            }


        } else {
            throw new IllegalExpressionException("Attempt to call non-function");
        }
    }

    @Override
    public SymbolicExpression visit(Function n) {
        return n;
    }

    @Override
    public SymbolicExpression visit(End n) {
        // TODO Auto-generated method stub
        throw new UnsupportedOperationException("Unimplemented method 'visit'");
    }
}
