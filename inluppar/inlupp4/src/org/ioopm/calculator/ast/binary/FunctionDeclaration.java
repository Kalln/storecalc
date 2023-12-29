package org.ioopm.calculator.ast.binary;

import org.ioopm.calculator.Visitor;
import org.ioopm.calculator.ast.SymbolicExpression;

public class FunctionDeclaration extends Assignment {
    public FunctionDeclaration(SymbolicExpression res, SymbolicExpression key) {
        super(res, key);
    }

    @Override
    public String getName() {
        return "FunctionDeclaration";
    }

    @Override
    public String toString() {
        return "TODO"; // TODO
    }

    @Override
    public SymbolicExpression accept(Visitor v) {
        return v.visit(this);
    }

}
