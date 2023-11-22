package org.ioopm.calculator.ast;

import org.ioopm.calculator.ast.atom.Variable;

import java.util.HashMap;

public class Environment extends HashMap<Variable, SymbolicExpression> {}
