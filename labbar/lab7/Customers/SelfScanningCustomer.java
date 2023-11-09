package Customers;

public class SelfScanningCustomer extends Customer {

    public SelfScanningCustomer(int bornTime, int groceries) {
        super(bornTime, groceries, groceries);
    }
    
}
