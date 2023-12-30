package org.ioopm.calculator;

import org.ioopm.calculator.ast.Conditional;
import org.ioopm.calculator.ast.Function;
import org.ioopm.calculator.ast.FunctionCall;
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
import org.ioopm.calculator.ast.command.Quit;
import org.ioopm.calculator.ast.command.Vars;
import org.ioopm.calculator.ast.unary.Cos;
import org.ioopm.calculator.ast.unary.Exp;
import org.ioopm.calculator.ast.unary.Log;
import org.ioopm.calculator.ast.unary.Negation;
import org.ioopm.calculator.ast.unary.Scope;
import org.ioopm.calculator.ast.unary.Sin;

public interface Visitor {
    public SymbolicExpression visit(Addition n);
    public SymbolicExpression visit(Assignment n);
    public SymbolicExpression visit(Clear n);
    public SymbolicExpression visit(Conditional n);
    public SymbolicExpression visit(Constant n);
    public SymbolicExpression visit(NamedConstant n);
    public SymbolicExpression visit(Cos n);
    public SymbolicExpression visit(Division n);
    public SymbolicExpression visit(Exp n);
    public SymbolicExpression visit(False n);
    public SymbolicExpression visit(Function n);
    public SymbolicExpression visit(FunctionCall n);
    public SymbolicExpression visit(FunctionDeclaration n);
    public SymbolicExpression visit(Log n);
    public SymbolicExpression visit(LessThan n);
    public SymbolicExpression visit(LessThanOrEquals n);
    public SymbolicExpression visit(GreaterThan n);
    public SymbolicExpression visit(GreaterThanOrEquals n);
    public SymbolicExpression visit(Equals n);
    public SymbolicExpression visit(NotEquals n);
    public SymbolicExpression visit(Multiplication n);
    public SymbolicExpression visit(Negation n);
    public SymbolicExpression visit(Quit n);
    public SymbolicExpression visit(Scope n);
    public SymbolicExpression visit(Sin n);
    public SymbolicExpression visit(Subtraction n);
    public SymbolicExpression visit(True n);
    public SymbolicExpression visit(Variable n);
    public SymbolicExpression visit(Vars n);
}
