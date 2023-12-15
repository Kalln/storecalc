package org.ioopm.calculator;

import org.ioopm.calculator.ast.IllegalAssignmentException;
import org.ioopm.calculator.ast.SymbolicExpression;

import org.ioopm.calculator.ast.atom.*;
import org.ioopm.calculator.ast.binary.*;
import org.ioopm.calculator.ast.unary.*;
import org.ioopm.calculator.ast.command.*;


public class NamedConstantChecker implements Visitor {    

    public boolean check(SymbolicExpression topLevel) {
        topLevel.accept(this);
        return true;
    }

    public SymbolicExpression visit(Addition a) {
        a.getLhs().accept(this);
        a.getLhs().accept(this);
        return null;
    }

    @Override
    public SymbolicExpression visit(Assignment n) throws IllegalAssignmentException {
        n.getLhs().accept(this);
        n.getLhs().accept(this);

        if (n.getRhs().isNamedConstant()) {
            // illegal assignment
            throw new IllegalAssignmentException(n.getRhs().getName() + "Can't be reassigned.");
        }
        return null;
    }

    @Override
    public SymbolicExpression visit(Clear n) {
        return null;
    }

    @Override
    public SymbolicExpression visit(Constant n) {
        return null;
    }

    @Override
    public SymbolicExpression visit(NamedConstant n) {
        return null;
    }

    @Override
    public SymbolicExpression visit(Cos n) {
        n.getExp().accept(this);
        return null;
    }

    @Override
    public SymbolicExpression visit(Division n) {
        n.getLhs().accept(this);
        n.getLhs().accept(this);
        return null;
    }

    @Override
    public SymbolicExpression visit(Exp n) {
        n.getExp().accept(this);
        return null;
    }

    @Override
    public SymbolicExpression visit(Log n) {
        n.getExp().accept(this);
        return null;
    }

    @Override
    public SymbolicExpression visit(Multiplication n) {
        n.getLhs().accept(this);
        n.getLhs().accept(this);
        return null;
    }

    @Override
    public SymbolicExpression visit(Negation n) {
        n.getExp().accept(this);
        return null;
    }

    @Override
    public SymbolicExpression visit(Quit n) {
        return null;
    }

    @Override
    public SymbolicExpression visit(Sin n) {
        n.getExp().accept(this);
        return null;
    }

    @Override
    public SymbolicExpression visit(Subtraction n) {
            n.getLhs().accept(this);
            n.getLhs().accept(this);
        return null;
    }

    @Override
    public SymbolicExpression visit(Variable n) {
        return null;
    }

    @Override
    public SymbolicExpression visit(Vars n) {
        return null;
    }
    
}
