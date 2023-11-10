import java.util.Scanner;



class MyDieTest {

    public static void main(String [] args) {
        Scanner sc = new Scanner(System.in);
        System.out.print("Number of sides: ");
        int sides = sc.nextInt();
        Die d = new Die(sides);
        var total = 0;
        for (var i = 0; i < 10; i++) {
            var roll = d.roll();
            System.out.println(d);
            total += roll;
        }

        System.out.println("Total value from 10 rolls: " + total);


        // pair of dice

        PairOfDice twoDie = new PairOfDice(6);
        twoDie.roll();
        System.out.println(twoDie);
    }
}
