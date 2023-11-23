package org.ioopm.calculator;

import java.io.IOException;
import java.util.Scanner;

import org.ioopm.calculator.ast.Environment;
import org.ioopm.calculator.ast.IllegalExpressionException;
import org.ioopm.calculator.ast.SymbolicExpression;
import org.ioopm.calculator.ast.command.*;
import org.ioopm.calculator.parser.CalculatorParser;
import org.ioopm.calculator.parser.SyntaxErrorException;

public class Calculator {

    public static void main(String[] args) throws IOException, SyntaxErrorException, IllegalExpressionException {    
    CalculatorParser calcParser = new CalculatorParser();
    Environment env = new Environment();

    Scanner scanner = new Scanner(System.in);

    while(true) {
        System.out.print("> ");
        String input = scanner.nextLine();
        SymbolicExpression result = calcParser.parse(input, env);

        if (result.isCommand()) {
            if (result instanceof Quit) {
                break;
            }
            else if (result instanceof Vars) {
                System.out.println(env.toString());
                continue;
            } else if (result instanceof Clear) {
                env.clear();
            }
        }

        System.out.println(result.eval(env));
    }}
    
}
