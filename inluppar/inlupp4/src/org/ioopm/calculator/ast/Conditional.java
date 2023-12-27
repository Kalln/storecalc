package org.ioopm.calculator.ast;

import org.ioopm.calculator.Visitor;

public class Conditional extends SymbolicExpression {
    private SymbolicExpression condition;
    private SymbolicExpression ifClause;
    private SymbolicExpression elseClause;
    public Conditional(
        SymbolicExpression condition,
        SymbolicExpression ifClause,
        SymbolicExpression elseClause
    ) {
        this.condition = condition;
        this.ifClause = ifClause;
        this.elseClause = elseClause;
    }

    public SymbolicExpression getIfClause() {
        return ifClause;
    }

    public SymbolicExpression getElseClause() {
        return elseClause;
    }

    public SymbolicExpression getCondition() {
        return condition;
    }

    @Override
    public SymbolicExpression accept(Visitor v) {
        return v.visit(this);
    }

    public String toString() {
        return "if " + condition + " " + ifClause + " else " + elseClause;
    }
}
