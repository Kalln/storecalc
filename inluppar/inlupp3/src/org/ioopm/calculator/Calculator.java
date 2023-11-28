package org.ioopm.calculator;

import java.io.IOException;
import java.util.Scanner;

import org.ioopm.calculator.ast.Environment;
import org.ioopm.calculator.ast.IllegalExpressionException;
import org.ioopm.calculator.ast.SymbolicExpression;
import org.ioopm.calculator.ast.atom.Constant;
import org.ioopm.calculator.ast.atom.Variable;
import org.ioopm.calculator.ast.command.*;
import org.ioopm.calculator.parser.CalculatorParser;
import org.ioopm.calculator.parser.SyntaxErrorException;

public class Calculator {

    public static void main(String[] args) throws IOException, SyntaxErrorException, IllegalExpressionException {    
    CalculatorParser calcParser = new CalculatorParser();
    int expressionEntered = 0;
    int sucessfullyEvaluated = 0;
    int fullyEvaluated = 0;
    Environment env = new Environment();

    Scanner scanner = new Scanner(System.in);

    while(true) {
        System.out.print("> ");
        String input = scanner.nextLine();
        SymbolicExpression result = calcParser.parse(input, env);
        expressionEntered++;
        if (result.isCommand()) {
            if (result instanceof Quit r) {
                r.run(env, expressionEntered, sucessfullyEvaluated, fullyEvaluated);
                break;
            }
            else if (result instanceof Vars r) {
                r.run(env, expressionEntered, sucessfullyEvaluated, fullyEvaluated);
                continue;
            } else if (result instanceof Clear r) {
                r.run(env, expressionEntered, sucessfullyEvaluated, fullyEvaluated);
                continue;
            }
        }
        else {
            var evalRes = result.eval(env);
            sucessfullyEvaluated++;
            env.put(new Variable("ans"), evalRes);
            System.out.println(evalRes);

            if (evalRes instanceof Constant) {
                fullyEvaluated++;
            }
        }

    }}
    
}
