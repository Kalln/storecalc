package Polymorfism;

import java.util.ArrayList;
import java.util.List;

abstract class Player { 
    public Player() { }
}

class ListOfPlayer <T extends Player> {
    List<T> playerLs = new ArrayList<>();

    public void addPlayer(T player) {
        playerLs.add(player);
    }

}

class BaseballPlayer extends Player {
    public BaseballPlayer() {
    
    }

    public String toString() {
        return "Baseball Player";
    }
}

class FotballPlayer extends Player {
    public FotballPlayer() {
        
    }

     public String toString() {
        return "Fotboll";
    }
}

public class E11 {
    public static void main(String[] args) {
        BaseballPlayer b1 = new BaseballPlayer();
        FotballPlayer f1 = new FotballPlayer();

        ListOfPlayer<Player> ls = new ListOfPlayer<>();
        ls.addPlayer(b1);
        ls.addPlayer(f1);
        System.out.println(ls.playerLs.get(1));

/*         ListOfPlayer<FotballPlayer> lss = new ListOfPlayer<>();
        lss.addPlayer(b1);
        lss.addPlayer(f1); */
    }
}

