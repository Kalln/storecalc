import static org.junit.jupiter.api.Assertions.*;

import org.ioopm.calculator.EvaluationVisitor;
import org.ioopm.calculator.NamedConstantChecker;
import org.ioopm.calculator.ReassignmentChecker;
import org.ioopm.calculator.ast.StackEnvironment;
import org.ioopm.calculator.ast.Conditional;
import org.ioopm.calculator.ast.IllegalAssignmentException;
import org.ioopm.calculator.ast.SymbolicExpression;
import org.ioopm.calculator.ast.atom.Constant;
import org.ioopm.calculator.ast.atom.False;
import org.ioopm.calculator.ast.atom.NamedConstant;
import org.ioopm.calculator.ast.atom.True;
import org.ioopm.calculator.ast.atom.Variable;
import org.ioopm.calculator.ast.binary.Addition;
import org.ioopm.calculator.ast.binary.Assignment;
import org.ioopm.calculator.ast.binary.Division;
import org.ioopm.calculator.ast.binary.Equal;
import org.ioopm.calculator.ast.binary.LessThan;
import org.ioopm.calculator.ast.binary.LessThanEqual;
import org.ioopm.calculator.ast.binary.MoreThan;
import org.ioopm.calculator.ast.binary.MoreThanEqual;
import org.ioopm.calculator.ast.binary.Multiplication;
import org.ioopm.calculator.ast.binary.Subtraction;
import org.ioopm.calculator.ast.command.Clear;
import org.ioopm.calculator.ast.command.Quit;
import org.ioopm.calculator.ast.command.Vars;
import org.ioopm.calculator.ast.unary.Cos;
import org.ioopm.calculator.ast.unary.Log;
import org.ioopm.calculator.ast.unary.Negation;
import org.ioopm.calculator.ast.unary.Scope;
import org.ioopm.calculator.ast.unary.Sin;
import org.ioopm.calculator.parser.CalculatorParser;
import org.ioopm.calculator.ast.unary.Exp;
import org.junit.jupiter.api.AfterAll;
import org.junit.jupiter.api.AfterEach;
import org.junit.jupiter.api.BeforeAll;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;

public class StandardTests {
    // Results might not match expected values exactly, some error is acceptable and expected.
    private double acceptableFloatError = 0.00001;
    private EvaluationVisitor visitor = new EvaluationVisitor();
    private StackEnvironment env = new StackEnvironment();

    @BeforeAll
    static void initAll() {

    }

    @BeforeEach
    void init() {
        visitor = new EvaluationVisitor();
        env = new StackEnvironment();
    }

    @Test
    void constantTest() {
        SymbolicExpression c = new Constant(42);
        SymbolicExpression c2 = new Constant(42);

        assertTrue(c.getValue() == 42);
        assertTrue(c.equals(c2));
        assertTrue(visitor.evaluate(c, env).equals(c2));

        assertTrue(c.isConstant());
        assertFalse(c.isCommand());
        assertFalse(c.isVariable());

    }

    @Test
    void variableTest() {
        Variable v = new Variable("x");
        assertTrue(v.getName() == "x");

        Variable longvar = new Variable("AveryLongNameIsHereIDontReallyKnowWhatToWriteHereExceptForSomethingIWillNowJustWriteSomethingSomethingSomething");
        assertTrue(longvar.getName() == "AveryLongNameIsHereIDontReallyKnowWhatToWriteHereExceptForSomethingIWillNowJustWriteSomethingSomethingSomething");

        assertFalse(longvar.equals(v));

        SymbolicExpression v2 = new Variable("x");

        assertTrue(v.equals(v2));

        assertTrue(v.isVariable());
        assertFalse(v.isConstant());
        assertFalse(v.isCommand());


    }

    @Test
    void additionTest() {
        SymbolicExpression add = new Addition(new Constant(42), new Constant(8));
        assertTrue(visitor.evaluate(add, env).getValue() == 50);
        assertTrue(new Constant(42).getValue() == 42);

        Addition add2 = new Addition(add, add);
        // should be equal both ways.
        assertTrue(add2.getLhs().equals(add));
        assertTrue(add.equals(add2.getLhs()));
        assertTrue(add2.getRhs().equals(add));
        assertTrue(add.equals(add2.getRhs()));

        assertTrue(add2.equals(add2));

        assertTrue(visitor.evaluate(add2, env).getValue() == 100);

        assertFalse(add.isVariable());
        assertFalse(add.isConstant());
        assertFalse(add.isCommand());

    }

    @Test
    void assignmentTest() {
        assertThrows(IllegalAssignmentException.class, () -> new Assignment(new Constant(7), new Variable(null)));

        var as2 = new Assignment(new Constant(42), new Variable("x"));
        assertEquals(visitor.evaluate(as2, env).getValue(), 42, acceptableFloatError);
        assertTrue(env.containsKey(new Variable("x")));
        assertEquals(env.get(new Variable("x")).getValue(), 42, acceptableFloatError);

        var as3 = new Assignment(new Addition(new Constant(1000), new Constant(234)), new Variable("qwerty"));
        assertEquals(visitor.evaluate(as3, env).getValue(), 1234, acceptableFloatError);
        assertTrue(env.containsKey(new Variable("qwerty")));
        assertEquals(env.get(new Variable("qwerty")).getValue(), 1234, acceptableFloatError);
    }

    @Test
    void divisionTest() {
        Division div = new Division(new Constant(42), new Constant(2));
        assertTrue(visitor.evaluate(div, env).getValue() == 21);
    }

    @Test
    void multiplicationTest() {
        SymbolicExpression a = new Multiplication(new Constant(5), new Constant(37));
        SymbolicExpression b = new Constant(185);
        assertTrue(visitor.evaluate(a, env).equals(b));
    }

    @Test
    void subtractionTest() {
        SymbolicExpression a = new Subtraction(new Constant(5), new Constant(37));
        SymbolicExpression b = new Constant(-32);
        assertTrue(visitor.evaluate(a, env).equals(b));
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
        assertTrue(visitor.evaluate(new Cos(c1), env).getValue() == Math.cos(5));

        var s2 = new Sin(new Division(new Constant(Math.PI), new Constant(2)));
        assertEquals(visitor.evaluate(s2, env).getValue(), 1);
    }

    @Test
    void expTest() {
        var c1 = new Constant(5);
        var v = new Variable("x");

        var c = new Exp(v);
        var c2 = new Exp(new Addition(new Constant(5), new Constant(2)));

        assertTrue(c.toString().equals("Exp(x)"));

        assertEquals(visitor.evaluate(new Exp(c1), env).getValue(), Math.exp(5));
        assertEquals(visitor.evaluate(c2, env).getValue(), Math.exp(7));
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
        var as = new Assignment(new Constant(6), new Variable("x"));


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
        assertEquals(visitor.evaluate(logE, env).getValue(), 1.0, acceptableFloatError);

        var log100 = new Log(new Constant(100));
        assertEquals(visitor.evaluate(log100, env).getValue(), Math.log(100), acceptableFloatError);
    }

    @Test
    void negTest() {
        SymbolicExpression a = new Negation(new Constant(4));
        SymbolicExpression b = new Addition(new Constant(4), a);
        SymbolicExpression c = new Constant(0);

        assertTrue(visitor.evaluate(a, env).equals(new Constant(-4)));
        assertTrue(visitor.evaluate(b, env).equals(c));
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
    void test_namedconstantCheck() {
        var constchecker = new NamedConstantChecker();
        var parser = new CalculatorParser();
        var env = new StackEnvironment();

        var unallowedExpr1 = "(5 = pi) + (3 = e)";
        var unallowedExpr2 = "(5 = pi) + (3 = d)";

        var allowedExpr1 = "pi + 5 = x";

        try {
            assertFalse(constchecker.check(parser.parse(unallowedExpr1, env)));
            assertFalse(constchecker.check(parser.parse(unallowedExpr2, env)));
            assertTrue(constchecker.check(parser.parse(allowedExpr1, env)));

        } catch (Exception error) {
            assertEquals(error.getMessage(), "");
        }
    }

    @Test
    void test_reassignmentChecker() {
        var assigncheck = new ReassignmentChecker();
        var parser = new CalculatorParser();
        var env = new StackEnvironment();

        var unallowedExpr1 = "(6 = x) + (4 = x)";
        var unallowedExpr2 = "(5+4 = y) - (41 = x) + (23 = y) + (11 = y)";

        var allowedExpr1 = "(5 = y) + (2 = x)";
        var allowedExpr2 = "5 = y";
        var allowedExpr3 = "(y = y)";

        try {
            assertFalse(assigncheck.check(parser.parse(unallowedExpr1, env)));
            assertFalse(assigncheck.check(parser.parse(unallowedExpr2, env)));
            assertTrue(assigncheck.check(parser.parse(allowedExpr1, env)));
            assertTrue(assigncheck.check(parser.parse(allowedExpr2, env)));
            assertTrue(assigncheck.check(parser.parse(allowedExpr3, env)));
        } catch (Exception error) {
            assertEquals(error.getMessage(), "");
        }
    }

    @Test
    void testEvaluateScope() {
        try {
            assertEquals(
                // {1 = x} + {1 = x}
                2.0,
                visitor.evaluate(
                    new Addition(
                        new Scope(new Assignment(new Constant(1), new Variable("x"))),
                        new Scope(new Assignment(new Constant(1), new Variable("x")))
                    ),
                    env
                ).getValue(),
                acceptableFloatError
            );
            assertEquals(
                1.0,
                // {{1 = x} = x}
                visitor.evaluate(
                    new Scope(new Assignment(new Scope(new Assignment(new Constant(1), new Variable("x"))), new Variable("x"))),
                    env
                ).getValue(),
                acceptableFloatError
            );
            assertEquals(
                // (1 = x) + {(2 + x = x) + {3 + x = x}}
                10.0,
                visitor.evaluate(
                    new Addition(
                        new Assignment(new Constant(1), new Variable("x")),
                        new Scope(
                            new Addition(
                                new Assignment(
                                    new Addition(new Constant(2), new Variable("x")),
                                    new Variable("x")
                                ),
                                new Scope(
                                    new Assignment(
                                        new Addition(new Constant(3), new Variable("x")),
                                        new Variable("x")
                                    )
                                )
                            )
                        )
                    ),
                    env
                ).getValue(),
                acceptableFloatError
            );
            assertEquals(
                // {{1 = x} = x} = y
                
                1.0,
                visitor.evaluate(
                    new Assignment(
                        new Scope(
                            new Assignment(
                                new Scope(
                                    new Assignment(
                                        new Constant(1), new Variable("x")
                                    )
                                ),
                                new Variable("x")
                            )
                        ),
                        new Variable("y")
                    ),
                    env
                    ).getValue(),acceptableFloatError
            );

        } catch (Exception e) {
            assertTrue(false);
        }
    }

    @Test
    void testTrue() {
        assertTrue(new True().isBoolean());
        assertTrue(new True().isTrue());

        assertEquals(new True(), new True());

        assertEquals(new True().toString(), "True");

        assertEquals(visitor.evaluate(new True(), env), new True());
    }

    @Test
    void testFalse() {
        assertTrue(new False().isBoolean());
        assertFalse(new False().isTrue());

        assertEquals(new False(), new False());
        assertNotEquals(new True(), new False());

        assertEquals(new False().toString(), "False");

        assertEquals(visitor.evaluate(new False(), env), new False());

    }

    @Test
    void testLessThan() {
        // 5 < 6
        var l1 = new LessThan(new Constant(5), new Constant(6));
        var evalResult1 = visitor.evaluate(l1, env);

        assertEquals(evalResult1, new True());
        assertEquals(l1.toString(), "5.0 < 6.0");

        // 1234 < 341
        var l2 = new LessThan(new Constant(1234), new Constant(341));
        var evalResult2 = visitor.evaluate(l2, env);

        assertEquals(evalResult2, new False());
        assertEquals(l2.toString(), "1234.0 < 341.0");

        // x < 1
        var l3 = new LessThan(new Variable("x"), new Constant(1));
        var evalResult3 = visitor.evaluate(l3, env);

        assertEquals(evalResult3, new LessThan(new Variable("x"), new Constant(1)));
        assertEquals(l3.toString(), "x < 1.0");

        // 9 < asdf
        var l4 = new LessThan(new Constant(9), new Variable("asdf"));
        var evalResult4 = visitor.evaluate(l4, env);

        assertEquals(evalResult4, new LessThan(new Constant(9), new Variable("asdf")));
        assertEquals(l4.toString(), "9.0 < asdf");

        // y < z
        var l5 = new LessThan(new Variable("y"), new Variable("z"));
        var evalResult5 = visitor.evaluate(l5, env);

        assertEquals(evalResult5, new LessThan(new Variable("y"), new Variable("z")));
        assertEquals(l5.toString(), "y < z");

        // 5.5 < 6.5
        var l6 = new LessThan(new Constant(5.5), new Constant(6.5));
        var evalResult6 = visitor.evaluate(l6, env);

        assertEquals(evalResult6, new True());
        assertEquals(l6.toString(), "5.5 < 6.5");
    }

    @Test
    void testEvaluateExpression() {
        assertEquals(
            1.0,
            visitor.evaluate(
                new Conditional(
                    new LessThan(new Constant(5), new Constant(6)),
                    new Scope(new Constant(1)),
                    new Scope(new Constant(2))
                ),
                env
            ).getValue(),
            acceptableFloatError
        );
        assertEquals(
            new Conditional(
                new LessThan(new Constant(5), new Constant(6)),
                new Scope(new Constant(1)),
                new Scope(new Constant(2))
            ).toString(),
            "if 5.0 < 6.0 {1.0} else {2.0}"
        );
        assertEquals(
            7.0,
            visitor.evaluate(
                new Conditional(
                    new LessThan(new Constant(10), new Constant(1)),
                    new Scope(new Constant(5)),
                    new Scope(new Constant(7))
                ),
                env
            ).getValue(),
            acceptableFloatError
        );
        assertEquals(
            new Conditional(
                new LessThan(new Constant(10), new Constant(1)),
                new Scope(new Constant(5)),
                new Scope(new Constant(7))
            ).toString(),
            "if 10.0 < 1.0 {5.0} else {7.0}"
        );
    }
    @Test
    void testLessThanEqual() {
        // 5 <= 7
        var l1 = new LessThanEqual(new Constant(5), new Constant(7));
        var evall1 = visitor.evaluate(l1, env);

        assertEquals(evall1, new True());
        assertEquals(l1.toString(), "5.0 <= 7.0");

        // 1321 <= 1000
        var l2 = new LessThanEqual(new Constant(1321) , new Constant(1000));
        var evall2 = visitor.evaluate(l2, env);

        assertEquals(evall2, new False());
        assertEquals(l2.toString(), "1321.0 <= 1000.0");

        // 5 <= 5
        var l3 = new LessThanEqual(new Constant(5), new Constant(5));
        var evall3 = visitor.evaluate(l3, env);

        assertEquals(evall3, new True());
        assertEquals(l3.toString(), "5.0 <= 5.0");

        // 5 + 2 <= 5 + 5 
        var l4 = new LessThanEqual(
                    new Addition(
                        new Constant(5),
                        new Constant(2)),
                    new Addition(
                        new Constant(5),
                        new Constant(5)
                    ));
        var evall4 = visitor.evaluate(l4, env);

        assertEquals(evall4, new True());
        assertEquals(l4.toString(), "(5.0 + 2.0) <= (5.0 + 5.0)");

        // 5 - 2 <= 1 + 2
        var l5 = new LessThanEqual(
                        new Subtraction(
                            new Constant(5),
                            new Constant(2)
                        ), 
                        new Addition(
                            new Constant(1),
                            new Constant(2)));
        var evall5 = visitor.evaluate(l5, env);

        assertEquals(evall5, new True());
        assertEquals(l5.toString(), "(5.0 - 2.0) <= (1.0 + 2.0)");
    }

    @Test
    void testMoreThan() {
        // 5 > 7
        var l1 = new MoreThan(new Constant(5), new Constant(7));
        var evall1 = visitor.evaluate(l1, env);

        assertEquals(evall1, new False());
        assertEquals(l1.toString(), "5.0 > 7.0");

        // 1321 > 1000
        var l2 = new MoreThan(new Constant(1321) , new Constant(1000));
        var evall2 = visitor.evaluate(l2, env);

        assertEquals(evall2, new True());
        assertEquals(l2.toString(), "1321.0 > 1000.0");

        // 5 > 5
        var l3 = new MoreThan(new Constant(5), new Constant(5));
        var evall3 = visitor.evaluate(l3, env);

        assertEquals(evall3, new False());
        assertEquals(l3.toString(), "5.0 > 5.0");

        // 5 + 2 > 5 + 5 
        var l4 = new MoreThan(
                    new Addition(
                        new Constant(5),
                        new Constant(2)),
                    new Addition(
                        new Constant(5),
                        new Constant(5)
                    ));
        var evall4 = visitor.evaluate(l4, env);

        assertEquals(evall4, new False());
        assertEquals(l4.toString(), "(5.0 + 2.0) > (5.0 + 5.0)");

        // 5 - 2 > 1 + 2
        var l5 = new MoreThan(
                        new Subtraction(
                            new Constant(5),
                            new Constant(2)
                        ), 
                        new Addition(
                            new Constant(1),
                            new Constant(2)));
        var evall5 = visitor.evaluate(l5, env);

        assertEquals(evall5, new False());
        assertEquals(l5.toString(), "(5.0 - 2.0) > (1.0 + 2.0)");

    }

    @Test
    void testMoreThanEqual() {
        // 5 >= 7
        var l1 = new MoreThanEqual(new Constant(5), new Constant(7));
        var evall1 = visitor.evaluate(l1, env);

        assertEquals(evall1, new False());
        assertEquals(l1.toString(), "5.0 >= 7.0");

        // 1321 >= 1000
        var l2 = new MoreThanEqual(new Constant(1321) , new Constant(1000));
        var evall2 = visitor.evaluate(l2, env);

        assertEquals(evall2, new True());
        assertEquals(l2.toString(), "1321.0 >= 1000.0");

        // 5 >= 5
        var l3 = new MoreThanEqual(new Constant(5), new Constant(5));
        var evall3 = visitor.evaluate(l3, env);

        assertEquals(evall3, new True());
        assertEquals(l3.toString(), "5.0 >= 5.0");

        // 5 + 2 >= 5 + 5 
        var l4 = new MoreThanEqual(
                    new Addition(
                        new Constant(5),
                        new Constant(2)),
                    new Addition(
                        new Constant(5),
                        new Constant(5)
                    ));
        var evall4 = visitor.evaluate(l4, env);

        assertEquals(evall4, new False());
        assertEquals(l4.toString(), "(5.0 + 2.0) >= (5.0 + 5.0)");

        // 5 - 2 >= 1 + 2
        var l5 = new MoreThanEqual(
                        new Subtraction(
                            new Constant(5),
                            new Constant(2)
                        ), 
                        new Addition(
                            new Constant(1),
                            new Constant(2)));
        var evall5 = visitor.evaluate(l5, env);

        assertEquals(evall5, new True());
        assertEquals(l5.toString(), "(5.0 - 2.0) >= (1.0 + 2.0)");

    }

    @Test
    void testEqual() {
        // 5 == 7
        var l1 = new Equal(new Constant(5), new Constant(7));
        var evall1 = visitor.evaluate(l1, env);

        assertEquals(evall1, new False());
        assertEquals(l1.toString(), "5.0 == 7.0");

        // 1321 == 1000
        var l2 = new Equal(new Constant(1321) , new Constant(1000));
        var evall2 = visitor.evaluate(l2, env);

        assertEquals(evall2, new False());
        assertEquals(l2.toString(), "1321.0 == 1000.0");

        // 5 == 5
        var l3 = new Equal(new Constant(5), new Constant(5));
        var evall3 = visitor.evaluate(l3, env);

        assertEquals(evall3, new True());
        assertEquals(l3.toString(), "5.0 == 5.0");

        // 5 + 2 == 5 + 5 
        var l4 = new Equal(
                    new Addition(
                        new Constant(5),
                        new Constant(2)),
                    new Addition(
                        new Constant(5),
                        new Constant(5)
                    ));
        var evall4 = visitor.evaluate(l4, env);

        assertEquals(evall4, new False());
        assertEquals(l4.toString(), "(5.0 + 2.0) == (5.0 + 5.0)");

        // 5 - 2 == 1 + 2
        var l5 = new Equal(
                        new Subtraction(
                            new Constant(5),
                            new Constant(2)
                        ), 
                        new Addition(
                            new Constant(1),
                            new Constant(2)));
        var evall5 = visitor.evaluate(l5, env);

        assertEquals(evall5, new True());
        assertEquals(l5.toString(), "(5.0 - 2.0) == (1.0 + 2.0)");

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
