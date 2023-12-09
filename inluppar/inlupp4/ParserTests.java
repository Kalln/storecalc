import static org.junit.jupiter.api.Assertions.*;

import org.ioopm.calculator.ast.Environment;
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
import org.ioopm.calculator.ast.unary.Log;
import org.ioopm.calculator.ast.unary.Negation;
import org.ioopm.calculator.ast.unary.Sin;
import org.ioopm.calculator.parser.CalculatorParser;
import org.ioopm.calculator.ast.unary.Exp;
import org.junit.jupiter.api.AfterAll;
import org.junit.jupiter.api.AfterEach;
import org.junit.jupiter.api.BeforeAll;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;




public class ParserTests {
    private Constant c1 = new Constant(42);
    private Constant c2 = new Constant(58);
    private Addition add = new Addition(c1, c2);
    private Subtraction sub = new Subtraction(c1, c2);
    private Multiplication mul = new Multiplication(c1, c2);
    private Division div = new Division(c1, c2);

    private Variable var = new Variable("x");
    private CalculatorParser parser = new CalculatorParser();
    private static Environment env = new Environment();


    @Test
    void basicBinaryOperatorParse(){
        try {
            assertTrue(parser.parse("42", env).equals(c1));
            assertTrue(parser.parse("42 + 58", env).equals(add));
            assertTrue(parser.parse("42 * 58", env).equals(mul));
            assertTrue(parser.parse("42 - 58", env).equals(sub));
            assertTrue(parser.parse("42 / 58", env).equals(div));

        } catch (Exception e) {
            // TODO what to do?

            assertTrue(false); // we got an exception.
        }
    }


    @Test
    void assignmentParse() {
        try {
            assertTrue(parser.parse("42 = x", env).equals(new Assignment(c1, var)));
            assertTrue(parser.parse("42 + 58 = x", env).equals(new Assignment(add, var)));

            // TODO more assignemnts tests, more advanced => can't assign pi, e, Quit, Vars etc.

        } catch (Exception e) {
            assertTrue(false);
        }
        
    }

    @Test
    void namedConstantParse() {
        try {
            assertEquals(parser.parse("pi", env).getValue(), new Constant(Math.PI).getValue());
            assertEquals(parser.parse("e", env).getValue(), new Constant(Math.E).getValue());
        } catch (Exception e) {
            assertTrue(false);
        }
    }

    @Test
    void advancedBinaryOperatorParse() {

        var mul = new Multiplication(c2, c1);
        try {
            assertEquals(parser.parse("(42 + 58) * 42 - 58", env).toString(), new Multiplication(add, sub).toString());
            assertEquals(parser.parse("42 + 58 * 42 - 58", env).toString(), new Addition(c1, new Subtraction(mul, c2)).toString());
            // TODO more

        } catch (Exception e) {
            assertTrue(false);
        }

    }

    @Test
    void commandsParse() {
        // TODO test commands
    }

    @Test
    void unaryParse() {

        // TODO test unary

    }

    
}
