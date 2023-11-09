package Customers;

public class Customer {
    public int bornTime;
    public int groceries;
    private int serveSpeed;

    public Customer(int bornTime, int groceries, int serveSpeed) {
        this.bornTime = bornTime;
        this.groceries = groceries;
        this.serveSpeed = serveSpeed;
    }

    public Customer(int bornTime, int groceries) {
        this.bornTime = bornTime;
        this.groceries = groceries; 
        this.serveSpeed = 2;
    }

    public void serve() {
        // We can't have served all groceries + more,
        // If the next scan will result in all groceries scanned we can set groceries to 0.
        if ((this.groceries - (1 * serveSpeed)) <= 0) {
            this.groceries = 0;
        } else {
            this.groceries -= (1 * serveSpeed);
        }
    }
    
    public boolean isDone() {
        return this.groceries <= 0;
    }
}
