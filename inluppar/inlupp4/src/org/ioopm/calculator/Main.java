package org.ioopm.calculator;

public class Main {
    public static void main(String[] args) {
        Calculator calc = new Calculator();

        while (true) {
            try {
                calc.run();

            } catch (Exception e) {
                System.out.println(e.getMessage());
            }
        }
    }
}
