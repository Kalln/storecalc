import static org.junit.jupiter.api.Assertions.*;

import org.ioopm.calculator.ast.Environment;
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
import org.ioopm.calculator.ast.unary.Exp;
import org.junit.jupiter.api.AfterAll;
import org.junit.jupiter.api.AfterEach;
import org.junit.jupiter.api.BeforeAll;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;

public class StandardTests {
    // Results might not match expected values exactly, some error is acceptable and expected.
    private double acceptableFloatError = 0.00001;

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
            SymbolicExpression as1 = new Assignment(new Variable("x"), new Constant(6));
            SymbolicExpression as = new Assignment(new Constant(3), new Constant(6));
        } catch (RuntimeException e) {
            assertEquals(e.getMessage(), "Right hand side must be a variable.");
        }
        Environment env = new Environment();
        Assignment as2 = new Assignment(new Constant(42), new Variable("x"));
        assertEquals(as2.eval(env).getValue(), 42.0);
        assertEquals(new Variable("x").eval(env), new Constant(42));
        
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
        var pi = new NamedConstant("π", Math.PI);

        var sin = new Sin(new Constant(5));
        var cos = new Cos(con);
        var exp = new Exp(con);
        var log = new Log(v);
        var neg = new Negation(pi);

        var add = new Addition(new Constant(2), new Constant(3456));
        var mul = new Multiplication(new Constant(0), new Constant(67));
        var sub = new Subtraction(new Constant(345) , new Constant(5));
        var div = new Division(v, con);
        SymbolicExpression as = new Assignment(new Constant(6), new Variable("x"));

        // var as = new Assignment(con, v);

        // TODO: check if assignment needs lower priority than addition/subtraction
        assertTrue(as.getPriority() < add.getPriority());
        assertTrue(add.getPriority() < mul.getPriority());
        assertTrue(sub.getPriority() < sin.getPriority());

        assertEquals(mul.getPriority(), div.getPriority());
        assertEquals(add.getPriority(), sub.getPriority());

        assertEquals(pi.getPriority(), con.getPriority());
        assertEquals(v.getPriority(), con.getPriority());

        assertEquals(sin.getPriority(), cos.getPriority());
        assertEquals(exp.getPriority(), cos.getPriority());
        assertEquals(log.getPriority(), cos.getPriority());
        assertEquals(neg.getPriority(), cos.getPriority());
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

        var logE = new Log(new Constant(Math.E));
        assertEquals(logE.eval(new Environment()).getValue(), 1.0, acceptableFloatError);

        var log100 = new Log(new Constant(100));
        assertEquals(log100.eval(new Environment()).getValue(), Math.log(100), acceptableFloatError);
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
    void isCommandTest() {
        // atom
        SymbolicExpression c1 = new Constant(42);
        SymbolicExpression nc = new NamedConstant("x", 42);
        SymbolicExpression v = new Variable("y");

        // binary
        SymbolicExpression a = new Addition(c1, v);
        SymbolicExpression as = new Assignment(c1, v);
        SymbolicExpression d = new Division(v, a);
        SymbolicExpression m = new Multiplication(as, d);
        SymbolicExpression s = new Subtraction(d, m);

        // command
        SymbolicExpression q = Quit.instance();
        SymbolicExpression c = Clear.instance();
        SymbolicExpression vars = Vars.instance();

        // unary
        SymbolicExpression cos = new Cos(c1);
        SymbolicExpression exp = new Exp(c1);
        SymbolicExpression log = new Log(c1);
        SymbolicExpression neg = new Negation(c1);
        SymbolicExpression sin = new Sin(c1);

        // test that all non-commands return false on fn isCommand()
        // atom
        assertFalse(c1.isCommand());
        assertFalse(nc.isCommand());
        assertFalse(v.isCommand());

        // binary
        assertFalse(a.isCommand());
        assertFalse(as.isCommand());
        assertFalse(d.isCommand());
        assertFalse(m.isCommand());
        assertFalse(s.isCommand());

        // command -> true
        assertTrue(q.isCommand());
        assertTrue(c.isCommand());
        assertTrue(vars.isCommand());

        // unary
        assertFalse(cos.isCommand());
        assertFalse(exp.isCommand());
        assertFalse(log.isCommand());
        assertFalse(neg.isCommand());
        assertFalse(sin.isCommand());
    }

    @Test
    void testToStringAtom() {
        SymbolicExpression c1 = new Constant(42);
        SymbolicExpression nc = new NamedConstant("x", 42);
        SymbolicExpression v = new Variable("y");

        assertEquals(c1.toString(), "42.0");
        assertEquals(nc.toString(), "x");
        assertEquals(v.toString(), "y");

    }

    @Test
    void testToStringBinary() {
        SymbolicExpression a = new Addition(new Constant(42), new Variable("x"));
        SymbolicExpression as = new Assignment(new Constant(42), new Variable("y"));
        SymbolicExpression d = new Division(new Variable("x"), a);
        SymbolicExpression m = new Multiplication(a, d);
        SymbolicExpression s = new Subtraction(d, m);

        assertEquals(a.toString(), "42.0 + x");
        assertEquals(as.toString(), "42.0 = y");
        assertEquals(d.toString(), "x / (42.0 + x)");
        assertEquals(m.toString(), "(42.0 + x) * x / (42.0 + x)");
        assertEquals(s.toString(), "x / (42.0 + x) - (42.0 + x) * x / (42.0 + x)");

    }

    @Test
    void testToStringUnary() {
        SymbolicExpression c1 = new Constant(42);
        SymbolicExpression cos = new Cos(c1);
        SymbolicExpression exp = new Exp(c1);
        SymbolicExpression log = new Log(c1);
        SymbolicExpression neg = new Negation(c1);
        SymbolicExpression sin = new Sin(c1);

        assertEquals(cos.toString(), "Cos(42.0)");
        assertEquals(exp.toString(), "Exp(42.0)");
        assertEquals(log.toString(), "Log(42.0)");
        assertEquals(neg.toString(), "-42.0");
        assertEquals(sin.toString(), "Sin(42.0)");

        var nested = new Cos(new Negation(new Cos(new Log(new Constant(36)))));
        assertEquals(nested.toString(), "Cos(-Cos(Log(36.0)))");

        var negatedAdd = new Negation(new Addition(new Constant(3), new Variable("z")));
        assertEquals(negatedAdd.toString(), "-(3.0 + z)");

        assertEquals(new Negation(new Constant(6)).toString(), "-6.0");
        assertEquals(new Negation(new Variable("i")).toString(), "-i");

    }

    @Test
    void getNametest() {
        // atom
        SymbolicExpression c1 = new Constant(42);
        SymbolicExpression nc = new NamedConstant("x", 42);
        SymbolicExpression v = new Variable("y");

        // binary
        SymbolicExpression a = new Addition(c1, v);
        SymbolicExpression as = new Assignment(c1, v);
        SymbolicExpression d = new Division(v, a);
        SymbolicExpression m = new Multiplication(as, d);
        SymbolicExpression s = new Subtraction(d, m);

        // command
        SymbolicExpression q = Quit.instance();
        SymbolicExpression c = Clear.instance();
        SymbolicExpression vars = Vars.instance();

        // unary
        SymbolicExpression cos = new Cos(c1);
        SymbolicExpression exp = new Exp(c1);
        SymbolicExpression log = new Log(c1);
        SymbolicExpression neg = new Negation(c1);
        SymbolicExpression sin = new Sin(c1);

        var thrown = assertThrows(RuntimeException.class, () -> {
            c1.getName();
            nc.getName();
            v.getName();
            q.getName();
            c.getName();
            vars.getName();
        });

        assertEquals("getName() not implemented for expressions with no operator", thrown.getMessage());


        assertEquals(a.getName(), "+");
        assertEquals(as.getName(), "=");
        assertEquals(d.getName(), "/");
        assertEquals(m.getName(), "*");
        assertEquals(s.getName(), "-");


        assertEquals(cos.getName(), "Cos");
        assertEquals(exp.getName(), "Exp");
        assertEquals(log.getName(), "Log");
        assertEquals(neg.getName(), "Neg");
        assertEquals(sin.getName(), "Sin");


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
