package org.ioopm.calculator.ast;

import java.util.ArrayDeque;
import java.util.Deque;

import org.ioopm.calculator.ast.atom.Variable;

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

    @Override
    public SymbolicExpression put(Variable key, SymbolicExpression value) {
        // TODO: add a test case for this
        var previous = this.get(key);
        if (stack.isEmpty()) {
            super.put(key, value);
        } else {
            stack.peek().put(key, value);
        }
        return previous;
    }
}
