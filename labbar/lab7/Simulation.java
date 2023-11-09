import java.util.Random;

import Customers.Customer;
import Customers.ElderlyCustomer;
import Customers.SelfScanningCustomer;

public class Simulation {
    Store store;
    int time; // Current time in simulation
    int intesity; // How often a new customers is created.
    int maxGroceries; // maximum amount of groceries that a customer have.
    int thresholdForNewRegister; // The avg length of open register queues to open a new register.


   /**
    * Statistics variables.
    */
    int customersServed; // Total amount of customers that have been served in this simulation
    int maxWaitTime; // The most 1 customer has had to wait.
    double avgWaitTime; // The avg waiting time for all of the customers. 


    public Simulation(
        int maxGroceries
        , int thresholdForNewRegister
        , int intensity
        , int amountRegister) {

        this.store = new Store(amountRegister);
        this.maxGroceries = maxGroceries; 
        this.thresholdForNewRegister = thresholdForNewRegister;
        this.intesity = intensity;
        this.time = 0;
        
        // create the first customer.
        store.newCustomer(new Customer(0, maxGroceries));
    }

    public void step() {
        this.time++; // increment time by 1.
        store.step(); // make the whole store step, so that it does its operations.

        Random rand = new Random();

        // determine if we should add a new customer.
        if (rand.nextInt(100) < intesity) {
            // what type of customer should we send to warehouse?
            int magicNumber = rand.nextInt(100);

            // 20% to be elderly customer.
            if (magicNumber < 20) {
               store.newCustomer(new ElderlyCustomer(time, rand.nextInt(maxGroceries)));
            }
            // 30% chance to self checking customer.
            else if (20 < magicNumber && magicNumber < 50) {
               store.newCustomer(new SelfScanningCustomer(time, rand.nextInt(maxGroceries)));
            } 
            else {
                // 50 % to get a regular customer.
            }
            
            store.newCustomer(new Customer(time, rand.nextInt(maxGroceries)));
          
        }

        // if avg queue time is longer than the threshold we have to open a new register.
        if (store.getAverageQueueLength() > thresholdForNewRegister) {
            store.openNewRegister();
        }

        // Write out the visuals.
        for (Register r : store.registers) {
            System.out.println(r + "\n");
        }

        // Will calculate the statistics.
        for (Customer c : store.getDoneCustomers()) {
            // get max wait time
            if (store.getDoneCustomers().isEmpty()) {
                break;
            }

            if (maxWaitTime < time - c.bornTime) {
                maxWaitTime = time - c.bornTime;
            }
            // contribute to avg wait time

            avgWaitTime = (avgWaitTime + time - c.bornTime) / 2;
            customersServed++;

        }

        /**
         * Write the statistics.
         */
        System.out.println("Passed time: " + time);
        System.out.println("Number of customers served: " + customersServed);
        System.out.println("Avg queue length: " + store.getAverageQueueLength());
        System.out.println("Max wait-time: " + maxWaitTime);
        System.out.println("Average wait-time: " + String.format("%.2f", avgWaitTime));
       
    }
}
