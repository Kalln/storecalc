package org.ioopm.calculator.ast.binary;

import org.ioopm.calculator.Visitor;
import org.ioopm.calculator.ast.IllegalAssignmentException;
import org.ioopm.calculator.ast.SymbolicExpression;

public class Assignment extends Binary {
    public Assignment(SymbolicExpression res, SymbolicExpression key) throws IllegalAssignmentException {
        super(res, key);
        // TODO, don't like this. I think we have to rewrite our parsing tests if we remove this classes
        // responsibility to "check" if it's a correct assignment since NamedConstantCheck should do it now.
        if (key.isVariable() || key.isNamedConstant()) {
            return;
        }
        
        
        throw new IllegalAssignmentException("Right side must be an variable.");
    }

    @Override
    public int getPriority() {
        return 5;
    }

    @Override
    public String getName() {
        return "=";
    }

    @Override
    public SymbolicExpression accept(Visitor v) {
        return v.visit(this);
    }
}
