package org.ioopm.calculator;

import org.ioopm.calculator.ast.SymbolicExpression;

public class PreEvaluationChecker {
    NamedConstantChecker namedConstantChecker;
    ReassignmentChecker ReassignmentChecker;

    boolean namedConstantCheckerResult;
    boolean reassignmentCheckerResult;

    public PreEvaluationChecker(SymbolicExpression expressionToCheck) {

        this.namedConstantChecker = new NamedConstantChecker();
        this.ReassignmentChecker = new ReassignmentChecker();

        this.namedConstantCheckerResult = namedConstantChecker.check(expressionToCheck);
        this.reassignmentCheckerResult = ReassignmentChecker.check(expressionToCheck);

    }


    public boolean ExpressionNotOK() {
        return !(namedConstantCheckerResult && reassignmentCheckerResult);
    }

}
