import java.lang.Thread;

public class Simulator {
    

    public static void main(String[] args) throws InterruptedException {
        Simulation sim = new Simulation(15, 4, 90, 10);

        
        while(sim.time < 100) {
            System.out.print("\033[2J\033[;H"); // clears the terminal
            sim.step();
            Thread.sleep(1000);
        }
   
        
    }
}
