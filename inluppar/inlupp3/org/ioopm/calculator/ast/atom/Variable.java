package inluppar.inlupp3.org.ioopm.calculator.ast.atom;

public class Variable extends Atom {

    private String identifier;

    public Variable(String identifier) {
        super("Variable");
        this.identifier = identifier;
    }

    public String getVariable() {
        return this.identifier;
    }

    public String toString() {
        return this.identifier;
    }

    public boolean isVariable() {
        return true;
    }
    
}
