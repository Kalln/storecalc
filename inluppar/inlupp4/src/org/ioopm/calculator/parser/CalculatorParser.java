package org.ioopm.calculator.parser;

import org.ioopm.calculator.ast.*;

import org.ioopm.calculator.ast.atom.*;
import org.ioopm.calculator.ast.binary.*;
import org.ioopm.calculator.ast.command.*;
import org.ioopm.calculator.ast.unary.*;

import java.io.StreamTokenizer;
import java.io.StringReader;
import java.io.IOException;

import java.util.*;

/**
 * Represents the parsing of strings into valid expressions defined in the AST.
 */
public class CalculatorParser {
    private StreamTokenizer st;
    private Environment env;
    private static char MULTIPLY = '*';
    private static char ADDITION = '+';
    private static char SUBTRACTION = '-';
    private static char DIVISION = '/';
    private static String NEG = "Neg";
    private static char NEGATION = '-';
    private static String SIN = "Sin";
    private static String COS = "Cos";
    private static String LOG = "Log";
    private static String EXP = "Exp";
    private static char ASSIGNMENT = '=';

    // unallowerdVars is used to check if variable name that we
    // want to assign new meaning to is a valid name eg 3 = Quit
    // or 10 + x = L is not allowed
    private final ArrayList<String> unallowedVars = new ArrayList<String>(Arrays.asList(
            "Quit",
            "Vars",
            "Clear",
            "Cos",
            "Sin",
            "Exp",
            "Log"));

    /**
     * Used to parse the inputted string by the Calculator program
     *
     * @param inputString the string used to parse
     * @param vars        the Environment in which the variables exist
     * @return a SymbolicExpression to be evaluated
     * @throws IOException by nextToken() if it reads invalid input
     */
    public SymbolicExpression parse(String inputString, Environment vars) throws IOException {
        this.st = new StreamTokenizer(new StringReader(inputString)); // reads from inputString via StringReader.
        this.env = vars;
        this.st.ordinaryChar('-');
        this.st.ordinaryChar('/');
        this.st.eolIsSignificant(true);
        SymbolicExpression result = statement(); // calls to statement
        return result; // the final result
    }

    /**
     * Checks wether the token read is a command or an assignment
     *
     * @return a SymbolicExpression to be evaluated
     * @throws IOException          by nextToken() if it reads invalid input
     * @throws SyntaxErrorException if the token parsed cannot be turned into a
     *                              valid expression
     */
    private SymbolicExpression statement() throws IOException {
        SymbolicExpression result;
        this.st.nextToken(); // kollar på nästa token som ligger på strömmen

        if (this.st.ttype == StreamTokenizer.TT_EOF) {
            throw new SyntaxErrorException("Error: Expected an expression");
        }

        if (this.st.ttype == StreamTokenizer.TT_WORD) { // vilken typ det senaste tecken vi läste in hade.
            if (this.st.sval.equals("function")) {
                result = function();
            } else if ( this.st.sval.equals("end")){
                result = new End();
            } else if (
                this.st.sval.equals("Quit")
                || this.st.sval.equals("Vars")
                || this.st.sval.equals("Clear")
            ) { // sval = string, Variable
                    result = command();
            } else {
                result = assignment(); // går vidare med uttrycket.
            }
        } else {
            result = assignment(); // om inte == word, gå till assignment ändå (kan vara tt_number)
        }

        if (this.st.nextToken() != StreamTokenizer.TT_EOF) { // token should be an end of stream token if we are done
            if (this.st.ttype == StreamTokenizer.TT_WORD) {
                throw new SyntaxErrorException("Error: Unexpected '" + this.st.sval + "'");
            } else {
                throw new SyntaxErrorException("Error: Unexpected '" + String.valueOf((char) this.st.ttype) + "'");
            }
        }
        return result;
    }

    private SymbolicExpression function() throws IOException {
        // get the func name
        this.st.nextToken(); // skip func
        var functionIdentifier = identifier(); // this is func name
        if (functionIdentifier instanceof Variable v) {
            functionIdentifier = v;
        } else {
            throw new SyntaxErrorException("Something went wrong with identifying the function name.");
        }

        // check for (
        this.st.nextToken();
        if (!(this.st.ttype == '(')) {
            throw new SyntaxErrorException("Expected '(' after function name.");
        }
        // get the arguments, separated by ','.
        this.st.nextToken();
        List<Variable> functionArguments = new ArrayList<>();

        while (this.st.ttype == StreamTokenizer.TT_WORD) {
            var PossibleVariable = identifier();
            if (PossibleVariable instanceof Variable va) {
                functionArguments.add(va);
            } else {
                throw new SyntaxErrorException("Expected a varible name.");
            }
            this.st.nextToken();
            if (this.st.ttype == ',') {
                this.st.nextToken();
            } else if (this.st.ttype == ')') {
                // we done......
                break;
            } else {
                throw new SyntaxErrorException("Expected ',' between arguments.");
            }
        }

        // return the declared function.
        return new FunctionDeclaration(new Function(functionArguments, new Sequence()), functionIdentifier);
    }

    /**
     * Checks what kind of command that should be returned
     *
     * @return an instance of Quit, Clear or Vars depending on the token parsed
     */
    private SymbolicExpression command() {
        if (this.st.sval.equals("Quit")) {
            return Quit.instance();
        } else if (this.st.sval.equals("Clear")) {
            return Clear.instance();
        } else {
            return Vars.instance();
        }
    }

    /**
     * Checks whether the token read is an assignment between 2 expression and
     * descend into the right hand side of '='
     *
     * @return a SymbolicExpression to be evaluated
     * @throws IOException          by nextToken() if it reads invalid input
     * @throws SyntaxErrorException if the token parsed cannot be turned into a
     *                              valid expression,
     *                              the variable on rhs of '=' is a number or
     *                              invalid variable
     */
    private SymbolicExpression assignment() throws IOException {
        SymbolicExpression result = expression();
        this.st.nextToken();
        while (this.st.ttype == ASSIGNMENT) {
            this.st.nextToken();
            if (this.st.ttype == StreamTokenizer.TT_NUMBER) {
                throw new SyntaxErrorException("Error: Numbers cannot be used as a variable name");
            } else if (this.st.ttype != StreamTokenizer.TT_WORD) {
                throw new SyntaxErrorException("Error: Not a valid assignment of a variable"); // this handles faulty
                                                                                               // inputs after the equal
                                                                                               // sign eg. 1 = (x etc
            } else {
                if (this.st.sval.equals("ans")) {
                    throw new SyntaxErrorException("Error: ans cannot be redefined");
                }
                SymbolicExpression key = identifier();
                result = new Assignment(result, key);
            }
            this.st.nextToken();
        }
        this.st.pushBack();
        return result;
    }

    /**
     * Check if valid identifier for variable and return that if so
     *
     * @return a SymbolicExpression that is either a named constant or a new
     *         variable
     * @throws IOException                by nextToken() if it reads invalid input
     * @throws IllegalExpressionException if you try to redefine a string that isn't
     *                                    allowed
     */
    private SymbolicExpression identifier() throws IOException {
        SymbolicExpression result;

        if (this.unallowedVars.contains(this.st.sval)) {
            throw new IllegalExpressionException("Error: cannot redefine " + this.st.sval);
        }

        if (Constants.namedConstants.containsKey(this.st.sval)) {
            result = new NamedConstant(st.sval, Constants.namedConstants.get(st.sval));
        } else {
            result = new Variable(this.st.sval);
        }
        return result;
    }

    /**
     * Checks whether the token read is an addition or subtraction
     * and then continue on with the right hand side of operator
     *
     * @return a SymbolicExpression to be evaluated
     * @throws IOException by nextToken() if it reads invalid input
     */
    private SymbolicExpression expression() throws IOException {
        SymbolicExpression result = term();
        this.st.nextToken();
        while (this.st.ttype == ADDITION || this.st.ttype == SUBTRACTION) {
            int operation = st.ttype;
            this.st.nextToken();
            if (operation == ADDITION) {
                result = new Addition(result, term());
            } else {
                result = new Subtraction(result, term());
            }
            this.st.nextToken();
        }
        this.st.pushBack();
        return result;
    }

    /**
     * Checks wether the token read is an Multiplication or
     * Division and then continue on with the right hand side of
     * operator
     *
     * @return a SymbolicExpression to be evaluated
     * @throws IOException by nextToken() if it reads invalid input
     */
    private SymbolicExpression term() throws IOException {
        SymbolicExpression result = primary();
        this.st.nextToken();
        while (this.st.ttype == MULTIPLY || this.st.ttype == DIVISION) {
            int operation = st.ttype;
            this.st.nextToken();

            if (operation == MULTIPLY) {
                result = new Multiplication(result, primary());
            } else {
                result = new Division(result, primary());
            }
            this.st.nextToken();
        }
        this.st.pushBack();
        return result;
    }

    /**
     * Checks wether the token read is a parantheses and then
     * continue on with the expression inside of it or if the
     * operation is an unary operation and then continue on with
     * the right hand side of that operator else if it's a
     * number/identifier
     *
     * @return a SymbolicExpression to be evaluated
     * @throws IOException          by nextToken() if it reads invalid input
     * @throws SyntaxErrorException if the token parsed cannot be turned into a
     *                              valid expression,
     *                              missing right parantheses
     */
    private SymbolicExpression primary() throws IOException {
        SymbolicExpression result;
        if (this.st.ttype == '(') {
            this.st.nextToken();
            result = assignment();
            /// This captures unbalanced parentheses!
            if (this.st.nextToken() != ')') {
                throw new SyntaxErrorException("expected ')'");
            }
        } else if (this.st.ttype == '{') {
            this.st.nextToken();

            result = new Scope(assignment());
            /// This captures unbalanced braces!
            if (this.st.nextToken() != '}') {
                throw new SyntaxErrorException("expected '}'");
            }
        } else if (this.st.ttype == NEGATION) {
            result = unary();
        } else if (this.st.ttype == StreamTokenizer.TT_WORD) {
            if (st.sval.equals(SIN) ||
                    st.sval.equals(COS) ||
                    st.sval.equals(EXP) ||
                    st.sval.equals(NEG) ||
                    st.sval.equals(LOG)) {

                result = unary();
            } else if (this.st.sval.equals("if")
                    || this.st.sval.equals("else")) {
                    result = conditional();
            }
            else {
                var unknownIdentifier = this.st.sval;
                // check the next if (, we know this is a func call.
                this.st.nextToken();
                this.st.pushBack();
                if (this.st.ttype == '(') {
                    // we got a function call to parse..
                    // but we have to restore the token that we "skipped"
                  this.st.sval = unknownIdentifier;
                  result = functioncall();
                } else {
                    this.st.sval = unknownIdentifier;
                    result = identifier();

                }
            }
        } else {
            this.st.pushBack();
            result = number();
        }
        return result;
    }

    private SymbolicExpression functioncall() throws IOException {
        var functionIdentifer = identifier();
        this.st.nextToken();

        // should be start of arguments..
        if (this.st.ttype != '(') {
            throw new SyntaxErrorException("Expected ( after function name.");
        }
        List<SymbolicExpression> functionArguments = new ArrayList<>();

        this.st.nextToken();

        // while we have not found ) it means that we are still collecting function arguments.
        while (this.st.ttype != ')') {
            functionArguments.add(assignment());
            this.st.nextToken();

            if (this.st.ttype == ')') {
                break;
            } else if (this.st.ttype != ',') {
                throw new SyntaxErrorException("Expected , after argument.");
            }

            this.st.nextToken();
        }

        if (functionIdentifer.isVariable()) {
            var function = env.get(functionIdentifer);
            return new FunctionCall(function != null ? function : functionIdentifer, functionArguments);
        } else {
            throw new SyntaxErrorException("Malformed function call");
        }
    }

    /**
     * Checks what type of Unary operation the token read is and
     * then continues with the expression that the operator holds
     *
     * @return a SymbolicExpression to be evaluated
     * @throws IOException by nextToken() if it reads invalid input
     */
    private SymbolicExpression unary() throws IOException {
        SymbolicExpression result;
        int operationNeg = st.ttype;
        String operation = st.sval;
        this.st.nextToken();
        if (operationNeg == NEGATION || operation.equals(NEG)) {
            result = new Negation(primary());
        } else if (operation.equals(SIN)) {
            result = new Sin(primary());
        } else if (operation.equals(COS)) {
            result = new Cos(primary());
        } else if (operation.equals(LOG)) {
            result = new Log(primary());
        } else {
            result = new Exp(primary());
        }
        return result;
    }

    /**
     * Retrieves the operetation that has been prompted for the conditional.
     * @return string - the conditional that was parsed.
     * @throws IOException if '<', '>', '<=', '>=', '!=', '==' was NOT read.
     */
    private String getConditionalOperation() throws IOException {
        this.st.nextToken(); // go to next, now we get our condition.
        char firstHalfOperation = ((char) st.ttype);
        this.st.nextToken();
        char secondHalfOperation = ((char) st.ttype);

        var sb = new StringBuilder();

        if (firstHalfOperation == '<') {
            sb.append(firstHalfOperation);

            if (secondHalfOperation == '=') {
                // <=
                sb.append(secondHalfOperation);
            } else {
                this.st.pushBack();
            }

        } else if (firstHalfOperation == '>') {
            sb.append(firstHalfOperation);
            if (secondHalfOperation == '=') {
                // >=
                sb.append(secondHalfOperation);
            } else {
                this.st.pushBack();
            }

        } else if (firstHalfOperation == '=') {
            sb.append(firstHalfOperation);
            if (secondHalfOperation == '=') {
                // ==
                sb.append(secondHalfOperation);
            } else {
                // wrong syntax..
                throw new SyntaxErrorException("Not possible to assign in condition.");
            }

        } else if (firstHalfOperation == '!') {
            sb.append(firstHalfOperation);
            if (secondHalfOperation == '=') {
                // !=
                sb.append(secondHalfOperation);
            } else {
                // wrong syntax..
                throw new SyntaxErrorException(
                    "Unexpected symbol "
                    + secondHalfOperation
                    + " after '!' in condition"
                );
            }

        } else {
            throw new SyntaxErrorException("Unexpected symbol.");
        }

        while (this.st.ttype == '<'
            || this.st.ttype == '>'
            || this.st.ttype == '='
        ) {
            this.st.nextToken();
        }

        return sb.toString();

    }
    /**
     * When the token "if" is read this function is called to parse the
     * conditional statement completely.
     *
     * @return              SymbolicExpression to be evaluated.
     * @throws IOException  If the wrong syntax is used. I.E. when stream-
     *                      tokenizer can't read next token.
     */
    private SymbolicExpression conditional() throws IOException {
        if (this.st.ttype == StreamTokenizer.TT_WORD) {
            this.st.nextToken(); // skip the if
        }
        var lhs = primary(); // lhs of condition
        var operation = getConditionalOperation();
        var rhs = primary();
        Condition cond;

        if (operation.equals("<")) {
            cond = new LessThan(lhs, rhs);

        } else if (operation.equals("<=")) {
            cond = new LessThanOrEquals(lhs, rhs);

        } else if (operation.equals(">")) {
            cond = new GreaterThan(lhs, rhs);

        } else if (operation.equals(">=")) {
            cond = new GreaterThanOrEquals(lhs, rhs);

        } else if (operation.equals("==")) {
            cond = new Equals(lhs, rhs);

        } else if (operation.equals("!=")) {
            cond = new NotEquals(lhs, rhs);

        } else {
            throw new SyntaxErrorException("Invalid operation.");
        }
        
        if (operation.endsWith("=")) {
            this.st.nextToken();
        }

        while (this.st.ttype != '{') {
            this.st.nextToken();
        }

        var ifScope = primary();

        while (this.st.ttype != StreamTokenizer.TT_WORD) {
            this.st.nextToken();
        }

        if (!this.st.sval.equals("else")) {
            throw new SyntaxErrorException("Expected else");
        }

        this.st.nextToken();
        var elseScope = primary();

        return new Conditional(cond, ifScope, elseScope);

    }

    /**
     * Checks if the token read is a number - should always be a number in this
     * method
     *
     * @return a SymbolicExpression to be evaluated
     * @throws IOException          by nextToken() if it reads invalid input
     * @throws SyntaxErrorException if the token parsed cannot be turned into a
     *                              valid expression,
     *                              expected a number which is not present
     */
    private SymbolicExpression number() throws IOException {
        this.st.nextToken();
        if (this.st.ttype == StreamTokenizer.TT_NUMBER) {
            return new Constant(this.st.nval);
        } else {
            throw new SyntaxErrorException("Error: Expected number");
        }
    }
}
