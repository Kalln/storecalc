package org.ioopm.calculator;

import java.util.ArrayList;
import java.util.List;
import java.util.TreeSet;

import org.ioopm.calculator.ast.IllegalAssignmentException;
import org.ioopm.calculator.ast.SymbolicExpression;

import org.ioopm.calculator.ast.atom.*;
import org.ioopm.calculator.ast.binary.*;
import org.ioopm.calculator.ast.unary.*;
import org.ioopm.calculator.ast.command.*;


public class NamedConstantChecker implements Visitor {    

    ArrayList<NamedConstant> FoundIllegalAssignments = new ArrayList<>();

    public boolean check(SymbolicExpression topLevel) {
        topLevel.accept(this);

        if (FoundIllegalAssignments.size() == 0) {
            return true;
        }
        return false;
    }

    public SymbolicExpression visit(Addition a) {
        a.getLhs().accept(this);
        a.getRhs().accept(this);
        return null;
    }

    @Override
    public SymbolicExpression visit(Assignment n) throws IllegalAssignmentException {
        n.getLhs().accept(this);
        n.getRhs().accept(this);

        if (n.getRhs().isNamedConstant()) {
            // illegal assignment
            this.FoundIllegalAssignments.add((NamedConstant) n.getRhs());
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
        n.getRhs().accept(this);
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
        n.getRhs().accept(this);
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

    @Override
    public String toString() {
        StringBuilder sb = new StringBuilder();
        sb.append("ERROR: Asssignment to named constant: ");
        
        for (var NC : this.FoundIllegalAssignments) {
            sb.append("\n");
            sb.append(NC.getName());
            sb.append(" = ");
            sb.append(NC.getValue());
            sb.append(", ");
        }

        return sb.toString();
    }
    
}
