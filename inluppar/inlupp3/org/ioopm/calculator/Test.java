package org.ioopm.calculator;

import java.util.HashMap;

import org.ioopm.calculator.ast.*;
import org.ioopm.calculator.ast.atom.Constant;
import org.ioopm.calculator.ast.atom.Variable;
import org.ioopm.calculator.ast.binary.Addition;
import org.ioopm.calculator.ast.binary.Multiplication;
import org.ioopm.calculator.ast.binary.Division;
import org.ioopm.calculator.ast.binary.Subtraction;
import org.ioopm.calculator.ast.unary.Cos;
import org.ioopm.calculator.ast.unary.Log;
import org.ioopm.calculator.ast.unary.Sin;



public class Test {
    public static void main(String[] args) {
        Test t = new Test();
        t.test_simple_mult();
        t.test_simple_add();
        t.test_simple_sub();
        t.test_simple_div();
        t.test_simple_const();
        t.test_simple_log();
        t.test_simple_sin_cos();
        t.test_simple_equal();
        t.test_simple_eval_add();
        t.test_simple_eval_sub();
        t.test_simple_eval_mult();
        t.test_simple_eval_div();
    }

    private void testPrinting(String expected, SymbolicExpression e) {
        if (expected.equals("" + e)) {
            System.out.println("PASS: " + e);
        } else {
            System.out.println("Error: expected " + expected + " but got " + e);
        }

    }
     private void testEvaluating(SymbolicExpression expected, SymbolicExpression e) {
        HashMap<Variable, SymbolicExpression> vars = new HashMap<Variable, SymbolicExpression>();
        
        SymbolicExpression r = e.eval(vars);
        if (r.equals(expected)) {
            System.out.println("Passed: " + r);
        } else {
            System.out.println("Error: expected '" + expected + "' but got '" + r + "'");
        }
    }

    private void testEqual(SymbolicExpression first, SymbolicExpression second, boolean isEqual) {
        if (isEqual == first.equals(second)) {
            if (isEqual) {
                System.out.println("PASS: " + first + " == " + second);
            } else {
                System.out.println("PASS: " + first + " != " + second);
            }
        } else {
            System.out.println("Error: expected " + first + " == " + second);
        }
    }

    private void test_simple_mult() {
        Constant c1 = new Constant(5);
        Constant c2 = new Constant(2);
        Variable v = new Variable("x");
        Addition a = new Addition(c1, v);
        Multiplication m = new Multiplication(a, c2);

        testPrinting("(5.0 + x) * 2.0", m);
    }

    private void test_simple_add() {
        Constant c1 = new Constant(5);
        Constant c2 = new Constant(5);
        Variable v = new Variable("x");

        Addition a = new Addition(c1, v);
        Addition a2 = new Addition(c1, c2);


        testPrinting("5.0 + x", a);
        testPrinting("5.0 + 5.0", a2);
    }

    private void test_simple_sub() {
        Constant c1 = new Constant(5);
        Constant c2 = new Constant(2);
        Variable v = new Variable("x");
        Subtraction s = new Subtraction(c1, v);
        Subtraction s2 = new Subtraction(c1, c2);

        testPrinting("5.0 - x", s);
        testPrinting("5.0 - 2.0", s2);
    }

    private void test_simple_div() {
        Constant c1 = new Constant(5);
        Constant c2 = new Constant(2);
        Variable v = new Variable("x");
        Addition a = new Addition(c1, v);
        Division d = new Division(a, c2);

        testPrinting("(5.0 + x) / 2.0", d);
    }

    private void test_simple_const() {
        Constant c1 = new Constant(42);


        testPrinting("42.0", c1);
    }

    private void test_simple_log() {
        Constant c1 = new Constant(5);
        Constant c2 = new Constant(2);
        Variable v = new Variable("x");
        Addition a = new Addition(c1, v);
        Multiplication m = new Multiplication(a, c2);
        Log l = new Log(m);

        testPrinting("log((5.0 + x) * 2.0)", l);
    }

    private void test_simple_sin_cos() {
        Constant c1 = new Constant(5);
        Constant c2 = new Constant(2);
        Variable v = new Variable("x");
        Addition a = new Addition(c1, v);
        Multiplication m = new Multiplication(a, c2);

        Cos c = new Cos(v);
        Sin s = new Sin(m);

        testPrinting("cos(x)", c);
        testPrinting("sin((5.0 + x) * 2.0)", s);
    }

    private void test_simple_equal(){
        Constant c1 = new Constant(5);
        Constant c2 = new Constant(5);
        Constant c3 = new Constant(4);

        testEqual(c1, c2, true);
        testEqual(c1, c3, false);
    }

    private void test_simple_eval_add() {
        SymbolicExpression a = new Addition(new Constant(5), new Constant(37));
        SymbolicExpression b = new Constant(42);
        testEvaluating(b, a); /// Tests if reducing a returns b (it should!)
    }

    private void test_simple_eval_mult() {
        SymbolicExpression a = new Multiplication(new Constant(5), new Constant(37));
        SymbolicExpression b = new Constant(185);
        testEvaluating(b, a);
    }

    private void test_simple_eval_sub() {
        SymbolicExpression a = new Subtraction(new Constant(5), new Constant(37));
        SymbolicExpression b = new Constant(-32);
        testEvaluating(b, a);
    }

    private void test_simple_eval_div() {
        SymbolicExpression a = new Division(new Constant(5), new Constant(5));
        SymbolicExpression b = new Constant(1);
        testEvaluating(b, a);
    }


}
