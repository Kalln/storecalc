import static org.junit.jupiter.api.Assertions.*;

import java.io.IOException;

import org.ioopm.calculator.ast.Conditional;
import org.ioopm.calculator.ast.Environment;
import org.ioopm.calculator.ast.IllegalExpressionException;
import org.ioopm.calculator.ast.atom.Constant;
import org.ioopm.calculator.ast.atom.NamedConstant;
import org.ioopm.calculator.ast.atom.Variable;
import org.ioopm.calculator.ast.binary.Addition;
import org.ioopm.calculator.ast.binary.Assignment;
import org.ioopm.calculator.ast.binary.Division;
import org.ioopm.calculator.ast.binary.Equals;
import org.ioopm.calculator.ast.binary.LessThan;
import org.ioopm.calculator.ast.binary.LessThanOrEquals;
import org.ioopm.calculator.ast.binary.GreaterThan;
import org.ioopm.calculator.ast.binary.GreaterThanOrEquals;
import org.ioopm.calculator.ast.binary.Multiplication;
import org.ioopm.calculator.ast.binary.NotEquals;
import org.ioopm.calculator.ast.binary.Subtraction;
import org.ioopm.calculator.ast.command.Clear;
import org.ioopm.calculator.ast.command.Quit;
import org.ioopm.calculator.ast.command.Vars;
import org.ioopm.calculator.ast.unary.Cos;
import org.ioopm.calculator.ast.unary.Exp;
import org.ioopm.calculator.ast.unary.Log;
import org.ioopm.calculator.ast.unary.Negation;
import org.ioopm.calculator.ast.unary.Scope;
import org.ioopm.calculator.ast.unary.Sin;
import org.ioopm.calculator.parser.CalculatorParser;
import org.ioopm.calculator.parser.SyntaxErrorException;
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
    void basicBinaryOperatorParse() {
        try {
            assertTrue(parser.parse("42", env).equals(c1));
            assertTrue(parser.parse("42 + 58", env).equals(add));
            assertTrue(parser.parse("42 * 58", env).equals(mul));
            assertTrue(parser.parse("42 - 58", env).equals(sub));
            assertTrue(parser.parse("42 / 58", env).equals(div));

            assertTrue(parser.parse("(42)", env).equals(c1));
            assertTrue(parser.parse("(42 + 58)", env).equals(add));
            assertTrue(parser.parse("(42 * 58)", env).equals(mul));
            assertTrue(parser.parse("(42 - 58)", env).equals(sub));
            assertTrue(parser.parse("(42 / 58)", env).equals(div));

            assertTrue(parser.parse("((42))", env).equals(c1));
            assertTrue(parser.parse("((42) + (58))", env).equals(add));
            assertTrue(parser.parse("((42) * (58))", env).equals(mul));
            assertTrue(parser.parse("((42) - (58))", env).equals(sub));
            assertTrue(parser.parse("((42) / (58))", env).equals(div));
        } catch (Exception e) {
            assertTrue(false); // The above test cases should not throw any exceptions
        }
    }

    @Test
    void assignmentParse() {
        try {
            assertTrue(parser.parse("42 = x", env).equals(new Assignment(c1, var)));
            assertTrue(parser.parse("42 + 58 = x", env).equals(new Assignment(add, var)));
            assertEquals(
                    parser.parse("42 = x = y", env),
                    new Assignment(new Assignment(c1, new Variable("x")), new Variable("y")));
            assertEquals(
                    parser.parse("(42 = x) = y", env),
                    new Assignment(new Assignment(c1, new Variable("x")), new Variable("y")));

            assertThrows(SyntaxErrorException.class, () -> parser.parse("42 = (x = y)", env));

            assertThrows(IllegalExpressionException.class, () -> parser.parse("5 = Vars", env));
            assertThrows(IllegalExpressionException.class, () -> parser.parse("7 = Quit", env));
            assertThrows(IllegalExpressionException.class, () -> parser.parse("11 = Clear", env));
        } catch (Exception e) {
            assertTrue(false);
        }

    }

    @Test
    void namedConstantParse() {
        try {
            assertEquals(parser.parse("pi", env), new NamedConstant("pi", Math.PI));
            assertEquals(parser.parse("e", env), new NamedConstant("e", Math.E));

            assertEquals(parser.parse("pi", env).getValue(), Math.PI);
            assertEquals(parser.parse("e", env).getValue(), Math.E);
        } catch (Exception e) {
            assertTrue(false);
        }
    }

    @Test
    void advancedBinaryOperatorParse() {
        var mul = new Multiplication(c2, c1);

        try {
            assertEquals(
                parser.parse("(42 + 58) * 42 - 58", env).toString(),
                new Multiplication(add, sub).toString()
            );

            assertEquals(
                parser.parse("42 + 58 * 42 - 58", env).toString(),
                new Addition(c1, new Subtraction(mul, c2)).toString()
            );

            // (Sin(3*x) * Cos(2*y)) / (a*a + b*b)
            var q1 = new Division(
                new Multiplication(
                    new Sin(new Multiplication(new Constant(3), new Variable("x"))),
                    new Cos(new Multiplication(new Constant(2), new Variable("y")))
                ),
                new Addition(
                    new Multiplication(new Variable("a"), new Variable("a")),
                    new Multiplication(new Variable("b"), new Variable("b"))
                )
            );

            // (4 * x * y) / (Cos(pi/4) + 2)
            var q2 = new Division(
                new Multiplication(
                    new Multiplication(new Constant(4), new Variable("x")),
                    new Variable("y")
                ),
                new Addition(
                    new Cos(
                        new Division(
                            new NamedConstant("pi", Math.PI),
                             new Constant(4)
                        )
                    ),
                    new Constant(2)
                )
            );

            // Exp(-(2*z)) / Log(5*w)

            var q3 = new Division(
                new Exp(new Negation(new Multiplication(new Constant(2), new Variable("z")))),
                new Log(new Multiplication(new Constant(5), new Variable("w"))));

            // Sin(x)*Sin(x) / Log(x*x + 1)
            var q4 = new Division(
                new Multiplication(new Sin(new Variable("x")), new Sin(new Variable("x"))),
                new Log(new Addition(new Multiplication(new Variable("x"), new Variable("x")), new Constant(1))));

            // (-(p*p) + 5*q) / Cos(r)
            var q5 = new Division(
                new Addition(
                    new Negation(new Multiplication(new Variable("p"), new Variable("p"))),
                    new Multiplication(new Constant(5), new Variable("q"))
                ),
                new Cos(new Variable("r"))
            );

            assertEquals(
                parser.parse(
                    ("(Sin(3*x) * Cos(2*y)) / (a*a + b*b)" // q1
                        + " - (4 * x * y) / (Cos(pi/4) + 2)" // q2
                        + " + Exp(-(2*z)) / Log(5*w)" // q3
                        + " - Sin(x)*Sin(x) / Log(x*x + 1)" // q4
                        + " + (-(p*p) + 5*q) / Cos(r)" // q5
                    ),
                    env),
                    new Addition(
                        new Subtraction(
                                new Addition(
                                        new Subtraction(
                                                q1,
                                                q2),
                                        q3),
                                q4),
                        q5
                    )
            );

        } catch (Exception e) {
            assertTrue(false);
        }

    }

    @Test
    void commandsParse() {
        try {
            assertTrue(parser.parse("Quit", env).isCommand());
            assertTrue(parser.parse("Vars", env).isCommand());
            assertTrue(parser.parse("Clear", env).isCommand());

            // There should only be one of each command, so we compare
            // with `==` to ensure no more instances are created
            assertTrue(parser.parse("Quit", env) == Quit.instance());
            assertTrue(parser.parse("Vars", env) == Vars.instance());
            assertTrue(parser.parse("Clear", env) == Clear.instance());
        } catch (Exception e) {
            assertTrue(false);
        }
    }

    @Test
    void unaryParse() {
        try {
            // Sin(1/2)
            assertTrue(
                parser.parse("Sin(1/2)", env)
                    .equals(
                        new Sin(
                            new Division(
                                new Constant(1),
                                new Constant(2)
                            )
                        )
                    )
            );

            // Cos(1)
            assertTrue(parser.parse("Cos(1)", env)
                .equals(new Cos(new Constant(1))));

            // Cos(Sin(1))
            assertTrue(parser.parse("Cos(Sin(1))", env)
                .equals(
                    new Cos(
                        new Sin(
                            new Constant(1)))));

        } catch (Exception e) {
            assertTrue(false);
        }

    }

    @Test
    void scopeParse() {
        try {
            assertEquals(
                parser.parse("{1 = x} + {1 = x}", env),
                new Addition(
                    new Scope(new Assignment(new Constant(1), new Variable("x"))),
                    new Scope(new Assignment(new Constant(1), new Variable("x")))));

            assertEquals(
                    parser.parse("{{1 = x} = x}", env),
                    new Scope(new Assignment(new Scope(new Assignment(new Constant(1), new Variable("x"))),
                        new Variable("x"))));
            assertEquals(
                    parser.parse("(1 = x) + {(2 + x = x) + {3 + x = x}}", env),
                    new Addition(
                            new Assignment(new Constant(1), new Variable("x")),
                            new Scope(
                                    new Addition(
                                            new Assignment(
                                                    new Addition(new Constant(2), new Variable("x")),
                                                    new Variable("x")),
                                            new Scope(
                                                    new Assignment(
                                                            new Addition(new Constant(3), new Variable("x")),
                                                            new Variable("x")))))));
        } catch (Exception e) {
            System.out.println(e);
            assertTrue(false);
        }

    }

    @Test
    void testConditional() throws IOException {
        var ifscope = new Scope(new Constant(42));
        var elsescope = new Scope(new Constant(4711));

        // if x < y { 42 } else { 4711 }
        var cond1 = new LessThan(new Variable("x"), new Variable("y"));
        var c1 = new Conditional(cond1, ifscope, elsescope);
        assertEquals(parser.parse("if x < y { 42 } else { 4711 }", env).toString(), c1.toString());

        // if x <= y { 42 } else { 4711 }
        var cond2 = new LessThanOrEquals(new Variable("x"), new Variable("y"));
        var c2 = new Conditional(cond2, ifscope, elsescope);
        assertEquals(parser.parse("if x <= y { 42 } else { 4711 }", env).toString(), c2.toString());

        // if x > y { 42 } else { 4711 }
        var cond3 = new GreaterThan(new Variable("x"), new Variable("y"));
        var c3 = new Conditional(cond3, ifscope, elsescope);
        assertEquals(parser.parse("if x > y { 42 } else { 4711 }", env).toString(), c3.toString());

        // if x >= y { 42 } else { 4711 }
        var cond4 = new GreaterThanOrEquals(new Variable("x"), new Variable("y"));
        var c4 = new Conditional(cond4, ifscope, elsescope);
        assertEquals(parser.parse("if x >= y { 42 } else { 4711 }", env).toString(), c4.toString());

        // if x == y { 42 } else { 4711 }
        var cond5 = new Equals(new Variable("x"), new Variable("y"));
        var c5 = new Conditional(cond5, ifscope, elsescope);
        assertEquals(parser.parse("if x == y { 42 } else { 4711 }", env).toString(), c5.toString());

        // if x != y { 42 } else { 4711 }
        var cond6 = new NotEquals(new Variable("x"), new Variable("y"));
        var c6 = new Conditional(cond6, ifscope, elsescope);
        assertEquals(parser.parse("if x != y { 42 } else { 4711 }", env).toString(), c6.toString());
    }

}
