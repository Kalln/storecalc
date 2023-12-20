package org.ioopm.calculator;

import java.util.ArrayList;
import java.util.HashMap;

import org.ioopm.calculator.ast.IllegalAssignmentException;
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
import org.ioopm.calculator.ast.unary.Scope;
import org.ioopm.calculator.ast.unary.Sin;

public class ReassignmentChecker implements Visitor {

    private HashMap<String, Integer> assignedVariables;


    private ArrayList<String> getIllegalAssignments(HashMap<String, Integer> assignedVars) {
        ArrayList<String> IllegalAssignments = new ArrayList<>();

        for (var entry : assignedVars.keySet()) {
            if (assignedVars.get(entry) > 1) {
                IllegalAssignments.add(entry);
            }
        }

        return IllegalAssignments;
    }

    public boolean check(SymbolicExpression topLevel) {
        this.assignedVariables = new HashMap<>();
        topLevel.accept(this);
        ArrayList<String> IllegalAssignments = new ArrayList<>();

        for (var entry : assignedVariables.keySet()) {
            if (assignedVariables.get(entry) > 1) {
                IllegalAssignments.add(entry);
            }
        }

        if (IllegalAssignments.size() == 0) {
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

        // add
        if (assignedVariables.get(n.getRhs().getName()) == null) {
            assignedVariables.put(n.getRhs().getName(), 1);
        } else {
            var currentvalue = assignedVariables.get(n.getRhs().getName());
            assignedVariables.put(n.getRhs().getName(), currentvalue + 1);
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
    public SymbolicExpression visit(Scope n) {
        // TODO: should scopes have different rules for this?
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
        sb.append("ERROR: Variable assigned twice: ");

        for (var assignedVariable : getIllegalAssignments(this.assignedVariables)) {
            sb.append(assignedVariable);
            sb.append(", ");
        }

        return sb.toString();
    }

}
