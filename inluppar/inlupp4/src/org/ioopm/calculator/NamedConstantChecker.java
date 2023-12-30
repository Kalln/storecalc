package org.ioopm.calculator;

import java.util.ArrayList;

import org.ioopm.calculator.ast.Conditional;
import org.ioopm.calculator.ast.Function;
import org.ioopm.calculator.ast.FunctionCall;
import org.ioopm.calculator.ast.IllegalAssignmentException;
import org.ioopm.calculator.ast.SymbolicExpression;
import org.ioopm.calculator.ast.atom.*;
import org.ioopm.calculator.ast.binary.*;
import org.ioopm.calculator.ast.unary.*;
import org.ioopm.calculator.ast.command.*;


public class NamedConstantChecker implements Visitor {

    ArrayList<NamedConstant> FoundIllegalAssignments = new ArrayList<>();

    public boolean check(SymbolicExpression topLevel) {
        this.FoundIllegalAssignments = new ArrayList<>();
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
    public SymbolicExpression visit(FunctionDeclaration n) throws IllegalAssignmentException {
        // A FunctionDeclaration is just a special type of Assignment
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
    public SymbolicExpression visit(Conditional n) {
        n.getIfClause().accept(this);
        n.getElseClause().accept(this);
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
    public SymbolicExpression visit(Scope n) {
        n.getExp().accept(this);
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

    @Override
    public SymbolicExpression visit(LessThan n) {
        n.getLhs().accept(this);
        n.getRhs().accept(this);

        return null;
    }

    @Override
    public SymbolicExpression visit(False n) {
        return null;
    }

    @Override
    public SymbolicExpression visit(True n) {
        return null;
    }

    @Override
    public SymbolicExpression visit(LessThanOrEquals n) {
        n.getLhs().accept(this);
        n.getRhs().accept(this);
        return null;
    }

    @Override
    public SymbolicExpression visit(GreaterThan n) {
        n.getLhs().accept(this);
        n.getRhs().accept(this);
        return null;
    }

    @Override
    public SymbolicExpression visit(GreaterThanOrEquals n) {
        n.getLhs().accept(this);
        n.getRhs().accept(this);
        return null;
    }

    @Override
    public SymbolicExpression visit(Equals n) {
        n.getLhs().accept(this);
        n.getRhs().accept(this);
        return null;
    }

    @Override
    public SymbolicExpression visit(NotEquals n) {
        n.getLhs().accept(this);
        n.getRhs().accept(this);
        return null;
    }

    @Override
    public SymbolicExpression visit(FunctionCall n) {
        for (var x : n.getArgs()) {
            x.accept(this);
        }
        return null;
    }

    @Override
    public SymbolicExpression visit(Function n) {
        return null;
    }

    @Override
    public SymbolicExpression visit(End n) {
        return null;
    }

}
