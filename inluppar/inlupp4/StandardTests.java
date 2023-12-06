import static org.junit.jupiter.api.Assertions.*;

import org.ioopm.calculator.ast.Environment;
import org.ioopm.calculator.ast.SymbolicExpression;
import org.ioopm.calculator.ast.atom.Constant;
import org.ioopm.calculator.ast.atom.Variable;
import org.ioopm.calculator.ast.binary.Addition;
import org.ioopm.calculator.ast.binary.Assignment;
import org.ioopm.calculator.ast.binary.Division;
import org.ioopm.calculator.ast.binary.Multiplication;
import org.ioopm.calculator.ast.binary.Subtraction;
import org.ioopm.calculator.ast.unary.Cos;
import org.ioopm.calculator.ast.unary.Log;
import org.ioopm.calculator.ast.unary.Negation;
import org.ioopm.calculator.ast.unary.Sin;
import org.ioopm.calculator.ast.unary.Exp;
import org.junit.jupiter.api.AfterAll;
import org.junit.jupiter.api.AfterEach;
import org.junit.jupiter.api.BeforeAll;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;

public class StandardTests {
    @BeforeAll
    static void initAll() {

    }

    @BeforeEach
    void init() {

    }

    @Test
    void constantTest() {
        SymbolicExpression c = new Constant(42);
        SymbolicExpression c2 = new Constant(42);

        assertTrue(c.getValue() == 42);
        assertTrue(c.equals(c2));
        assertTrue(c.eval(null).equals(c2));

        assertTrue(c.isConstant());
        assertFalse(c.isCommand());
        assertFalse(c.isVariable());

    }

    @Test
    void variableTest() {
        Variable v = new Variable("x");
        assertTrue(v.getVariable() == "x");

        Variable longvar = new Variable("AveryLongNameIsHereIDontReallyKnowWhatToWriteHereExceptForSomethingIWillNowJustWriteSomethingSomethingSomething");
        assertTrue(longvar.getVariable() == "AveryLongNameIsHereIDontReallyKnowWhatToWriteHereExceptForSomethingIWillNowJustWriteSomethingSomethingSomething");

        assertFalse(longvar.equals(v));

        SymbolicExpression v2 = new Variable("x");

        assertTrue(v.equals(v2));

        assertTrue(v.isVariable());
        assertFalse(v.isConstant());
        assertFalse(v.isCommand());


    }

    @Test
    void additionTest() {
        Environment vars = new Environment();

        SymbolicExpression add = new Addition(new Constant(42), new Constant(8));
        assertTrue(add.eval(null).getValue() == 50);
        assertTrue(new Constant(42).getValue() == 42);

        Addition add2 = new Addition(add, add);
        // should be equal both ways.
        assertTrue(add2.getLhs().equals(add));
        assertTrue(add.equals(add2.getLhs()));
        assertTrue(add2.getRhs().equals(add));
        assertTrue(add.equals(add2.getRhs()));

        assertTrue(add2.equals(add2));

        assertTrue(add2.eval(vars).getValue() == 100);

        assertFalse(add.isVariable());
        assertFalse(add.isConstant());
        assertFalse(add.isCommand());

    }

    @Test
    void assignmentTest() {
        try {
            SymbolicExpression as = new Assignment(new Constant(3), new Constant(6));
            SymbolicExpression as1 = new Assignment(null, new Constant(6));
        } catch (RuntimeException e) {
            assertTrue(e.getMessage() == "Left hand side of assignment must be a variable");
        }

        Assignment as2 = new Assignment(new Constant(42), new Variable("x"));

        // TODO
    }

    @Test
    void divisionTest() {
        Environment vars = new Environment();
        Division div = new Division(new Constant(42), new Constant(2));
        div.eval(vars).getValue();
        assertTrue(div.eval(vars).getValue() == 21);
    }

    @Test
    void multiplicationTest() {
        SymbolicExpression a = new Multiplication(new Constant(5), new Constant(37));
        SymbolicExpression b = new Constant(185);
        assertTrue(a.eval(null).equals(b));
    }

    @Test
    void subtractionTest() {
        SymbolicExpression a = new Subtraction(new Constant(5), new Constant(37));
        SymbolicExpression b = new Constant(-32);
        assertTrue(a.eval(null).equals(b));
    }

    @Test
    void cossinTest() {
        Constant c1 = new Constant(5);
        Constant c2 = new Constant(2);
        Variable v = new Variable("x");
        Addition a = new Addition(c1, v);
        Multiplication m = new Multiplication(a, c2);

        Cos c = new Cos(v);
        Sin s = new Sin(m);

        assertTrue(c.toString().equals("Cos(x)"));
        assertTrue(s.toString().equals("Sin((5.0 + x) * 2.0)"));
        assertTrue(new Cos(c1).eval(null).getValue() == Math.cos(5));
    }

    @Test
    void expTest() {
        var c1 = new Constant(5);
        var v = new Variable("x");

        var c = new Exp(v);
        var c2 = new Exp(new Addition(new Constant(5), new Constant(2)));

        assertTrue(c.toString().equals("Exp(x)"));

        assertEquals(new Exp(c1).eval(null).getValue(), Math.exp(5));
        assertEquals(c2.eval(null).getValue(), Math.exp(7));
        assertFalse(c2.isConstant());
    }

    @Test
    void testPriority() {
        var v = new Variable("var");
        var con = new Constant(9);
        var sin = new Sin(new Constant(5));
        var cos = new Cos(con);

        var add = new Addition(new Constant(2), new Constant(3456));
        var mul = new Multiplication(new Constant(0), new Constant(67));
        var sub = new Subtraction(new Constant(345) , new Constant(5));
        var div = new Division(v, con);

        assertTrue(add.getPriority() <  mul.getPriority());
        assertTrue(sin.getPriority() >  sub.getPriority());

        assertEquals(mul.getPriority(), div.getPriority());
        assertEquals(add.getPriority(), sub.getPriority());

        assertEquals(v.getPriority(), con.getPriority());
        assertEquals(sin.getPriority(), cos.getPriority());

        //TODO: check more priority
    }

    @Test
    void logTest() {
        Constant c1 = new Constant(5);
        Constant c2 = new Constant(2);
        Variable v = new Variable("x");
        Addition a = new Addition(c1, v);
        Multiplication m = new Multiplication(a, c2);
        Log l = new Log(m);

        assertTrue(l.toString().equals("Log((5.0 + x) * 2.0)"));
        // TODO test eval
    }

    @Test
    void negTest() {
        SymbolicExpression a = new Negation(new Constant(4));
        SymbolicExpression b = new Addition(new Constant(4), a);
        SymbolicExpression c = new Constant(0);

        assertTrue(a.eval(null).equals(new Constant(-4)));
        assertTrue(b.eval(null).equals(c));
    }

    @Test
    void advancedExpressionsTest() {
        SymbolicExpression c1 = new Constant(5);
        SymbolicExpression c2 = new Constant(2);
        SymbolicExpression v = new Variable("x");
        SymbolicExpression a = new Addition(c1, v);
        SymbolicExpression m1 = new Multiplication(a, c2);
        SymbolicExpression d = new Division(m1, a);

        assertTrue(d.toString().equals("(5.0 + x) * 2.0 / (5.0 + x)"));
    }

    @Test
    void succeedingTest() {
        assertTrue(true);
    }

    @AfterEach
    void tearDown() {
    }

    @AfterAll
    static void tearDownAll() {
    }

}
