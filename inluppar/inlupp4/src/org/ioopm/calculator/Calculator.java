package org.ioopm.calculator;

import java.io.IOException;
import java.util.NoSuchElementException;
import java.util.Scanner;

import org.ioopm.calculator.ast.StackEnvironment;
import org.ioopm.calculator.ast.End;
import org.ioopm.calculator.ast.Function;
import org.ioopm.calculator.ast.IllegalExpressionException;
import org.ioopm.calculator.ast.SymbolicExpression;
import org.ioopm.calculator.ast.atom.Constant;
import org.ioopm.calculator.ast.atom.Variable;
import org.ioopm.calculator.ast.binary.FunctionDeclaration;
import org.ioopm.calculator.parser.CalculatorParser;
import org.ioopm.calculator.parser.SyntaxErrorException;

public class Calculator {
    CalculatorParser calcParser;
    int expressionEntered;
    int sucessfullyEvaluated;
    int fullyEvaluated;
    boolean activeFunctionState;
    StackEnvironment env;
    FunctionDeclaration buildingFunctionDeclaration;

    public Calculator() {
        this.calcParser = new CalculatorParser();
        this.expressionEntered = 0;
        this.sucessfullyEvaluated = 0;
        this.fullyEvaluated = 0;
        this.activeFunctionState = false;
        this.env = new StackEnvironment();
        this.buildingFunctionDeclaration = null;
    }

    public void run() throws SyntaxErrorException {
        
        // prompt
        System.out.print(activeFunctionState ? ">> " : "> ");

        SymbolicExpression result = null;
        // get user input and parse.
        try {
            String input = getUserInput();
            result = parseString(input);
            expressionEntered++;

        } catch (Exception e) {
            System.out.println(e.getMessage());
        }

        if (result.isCommand()) {
            result.run(env, expressionEntered, sucessfullyEvaluated, fullyEvaluated);
            return;
        }

        var functionEnd = false;
        if (result instanceof End) {
            if (!activeFunctionState) {
                throw new SyntaxErrorException("Can't end a function when there is no active function.");
            }
            // save function
            result = buildingFunctionDeclaration;
            activeFunctionState = false;
            functionEnd = true;
        }

        PreEvaluationChecker checkerResult = new PreEvaluationChecker(result);

        if (checkerResult.ExpressionNotOK()) {
            if (!checkerResult.namedConstantCheckerResult) {
                System.out.println(checkerResult.namedConstantChecker);
            } else {
                System.out.println(checkerResult.ReassignmentChecker);
            }
            return;
        }


        // TODO abstract
        
        // result is a function declaration, should only happen ones.
        if (result instanceof FunctionDeclaration f && ! functionEnd) {
            if (activeFunctionState) {
                throw new SyntaxErrorException("function can't be declared in another function.");
            } else {
                System.out.println("Function state: TRUE");
                activeFunctionState = true;

                buildingFunctionDeclaration = f;
            }
            return;
        }
        if (activeFunctionState) {
            // we will continue to add each line to current func body.. without evaluating
            if (buildingFunctionDeclaration.getFunction() instanceof Function f) {
                f.getBody().add(result);
            } else {
                throw new SyntaxErrorException("SOmething wrong happened.");
            }
            // buildingFunctionDeclaration.getFunction().getBody().add(result);
            return;

        }
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
    }
    // evaluate
    

    
    

    private SymbolicExpression parseString(String input) {
        try {
            this.expressionEntered++;
            return this.calcParser.parse(input, this.env);
        } catch (Exception e) {
            System.out.println(e.getMessage());
            return null;
        }
    }

    private String getUserInput() {
        try {
            var scanner = new Scanner(System.in);
            return scanner.nextLine();
        } catch (Exception e) {
            System.out.println(e.getMessage());
            return null;
        }
    }

    public static void main(String[] args) throws IOException, SyntaxErrorException, IllegalExpressionException {

        // Initialize variables for statistics and create the CalculatorParser that will parse the input.
        // env contains all the environment variables, to be allowed to use constants but also create
        // variables during runtime.
        CalculatorParser calcParser = new CalculatorParser();
        int expressionEntered = 0;
        int sucessfullyEvaluated = 0;
        int fullyEvaluated = 0;
        StackEnvironment env = new StackEnvironment();
        FunctionDeclaration buildingFunctionDeclaration = null;
        boolean activeFunctionState = false;

        try (var scanner = new Scanner(System.in)) {
            while(true) {
                System.out.print(activeFunctionState ? ">> " : "> ");

                try {
                    String input = scanner.nextLine();
                    SymbolicExpression result = calcParser.parse(input, env);
                    expressionEntered++;

                    // Since commands can't be evaluated, we have to check that if the result is a command first before evaluating.
                    // all commands have their own run methods, that do their own operation.
                    if (result.isCommand()) {
                        result.run(env, expressionEntered, sucessfullyEvaluated, fullyEvaluated);
                        continue;
                    } else {
                        var functionEnd = false;
                        if (result instanceof End) {
                            if (!activeFunctionState) {
                                throw new SyntaxErrorException("Can't end a function when there is no active function.");
                            }
                            // save function
                            result = buildingFunctionDeclaration;
                            activeFunctionState = false;
                            functionEnd = true;
                        }
                        // results is not a command, therefore we can evaluate the result.
                        
                        PreEvaluationChecker preChecker = new PreEvaluationChecker(result);
                        if (!preChecker.ExpressionNotOK()) {

                            // result is a function declaration, should only happen ones...
                            if (result instanceof FunctionDeclaration f && ! functionEnd) {
                                if (activeFunctionState) {
                                    throw new SyntaxErrorException("function can't be declared in another function.");
                                } else {
                                    System.out.println("Function state: TRUE");
                                    activeFunctionState = true;

                                    buildingFunctionDeclaration = f;
                                }
                                continue;
                            }

                            if (activeFunctionState) {
                                // we will continue to add each line to current func body.. without evaluating
                                if (buildingFunctionDeclaration.getFunction() instanceof Function f) {
                                    f.getBody().add(result);
                                } else {
                                    throw new SyntaxErrorException("SOmething wrong happened.");
                                }
                                continue;

                            }

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
                        } else if (preChecker.namedConstantCheckerResult) {
                            System.out.println(preChecker.namedConstantChecker);
                        } else {
                            System.out.println(preChecker.ReassignmentChecker);
                        }

                    }
                } catch (Exception e){
                    if (e instanceof NoSuchElementException) { break; }
                    System.out.println(e.getMessage());
                }

            }
        }

    }

}
