package org.ioopm.calculator.ast;

import java.util.ArrayDeque;
import java.util.Deque;

public class StackEnvironment extends Environment {
    private Deque<Environment> stack = new ArrayDeque<>();

    public StackEnvironment() {
        super();
    }

    public void pushEnvironment() {
        stack.push(new Environment());
    }

    public void popEnvironment() {
        stack.pop();
    }

    @Override
    public SymbolicExpression get(Object v) {
        var containing = stack.stream().filter(x -> x.containsKey(v)).findFirst();
        return containing.isPresent()
            ? containing.orElseThrow().get(v)
            : super.get(v);
    }
}
