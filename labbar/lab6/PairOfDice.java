public class PairOfDice {

    private Die die1;
    private Die die2;

    public PairOfDice(int numberOfSides) {
        this.die1 = new Die(numberOfSides);
        this.die2 = new Die(numberOfSides);
    }

    public PairOfDice(){
        this(6);
    }

    public int get(){
        return die1.get() + die2.get();
    }

    public int roll(){
        this.die1.roll();
        this.die2.roll();
        return this.get();
    }

    public int read_first_die() {
        return die1.get();
    }

    public int read_second_die(){
        return die2.get();
    }

    public String toString() {
        return "Die1("+ read_first_die() +"), Die2("+ read_second_die() +")";
    }
    
}
