package org.ioopm.calculator;

import java.io.IOException;
import java.util.NoSuchElementException;
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

        // Initialize variables for statistics and create the CalculatorParser that will parse the input.
        // env contains all the environment variables, to be allowed to use constants but also create
        // variables during runtime.
        CalculatorParser calcParser = new CalculatorParser();
        int expressionEntered = 0;
        int sucessfullyEvaluated = 0;
        int fullyEvaluated = 0;
        Environment env = new Environment();

        Scanner scanner = new Scanner(System.in);

        while(true) {
            System.out.print("> ");


            try {
                String input = scanner.nextLine();
                SymbolicExpression result = calcParser.parse(input, env);
                expressionEntered++;

                // Since commands can't be evaluated, we have to check that if the result is a command first before evaluating.
                if (result.isCommand()) {

                    // all commands have their own run methods, that do their own operation.
                    if (result instanceof Quit r) {
                        r.run(env, expressionEntered, sucessfullyEvaluated, fullyEvaluated);
                        // Breaks the while loop.
                        break;
                    }
                    else if (result instanceof Vars r) {
                        System.out.println(env);
                        continue;
                    } else if (result instanceof Clear r) {
                        r.run(env, expressionEntered, sucessfullyEvaluated, fullyEvaluated);
                        continue;
                    }
                } else {

                    // results is not a command, therefore we can evaluate the result.
                    var evaluator = new EvaluationVisitor();
                    var evalRes = evaluator.evaluate(result, env);
                    sucessfullyEvaluated++;

                    // ans variable will keep the last successfully evaluated expression.
                    // this is done by saving 'ans' as a variable in the environment.
                    env.put(new Variable("ans"), evalRes);

                    // Check if the result is a constant, this would mean that we have fully evaluated a expression and can add it to the statistics.
                    if (evalRes instanceof Constant) {
                        fullyEvaluated++;
                    }


                    System.out.println(evalRes);
                }
            } catch (Exception e){
                if (e instanceof NoSuchElementException) { break; }
                System.out.println(e.getMessage());
            }

        }

        scanner.close();
    }

}
