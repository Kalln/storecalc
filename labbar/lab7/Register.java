import Customers.Customer;
import Customers.ElderlyCustomer;
import Customers.SelfScanningCustomer;

class Register {
    private boolean open;
    CustomerQueue queue;


    public Register(CustomerQueue queue) {
        this.queue = queue;
        this.open = false;
    }

    public void open() {
        open = true;
    }

    public void close() {
        open = false;
    }

    public boolean isOpen() {
        return open;
    }

    public void step() {
        queue.first().serve();
    }

    public boolean hasCustomer() {
        // if queue is NOT empty, we have customers.
        return !queue.isEmpty();
    }

    public boolean currentCustomerIsDone() {
        return queue.first().isDone();
        
    }

    public void addToQueue(Customer c) {
        queue.enqueue(c);
    }

    public void RemoveCurrentCustomer() {
        queue.dequeue();
    }

    public int getQueueLength() {
        return queue.length();
    }

    private String getCustomerIdentifier() {
        Customer firstInLineCustomer = queue.first();
        String customerIdentifier = String.format("%d" , firstInLineCustomer.groceries);

            if (firstInLineCustomer instanceof SelfScanningCustomer) {
                customerIdentifier += "F";
            } 
            else if (firstInLineCustomer instanceof ElderlyCustomer){
                customerIdentifier += "E";
            } 
            else {
                 customerIdentifier += "C";
            }

            return customerIdentifier;
    }

    private String getCustomerQueue() {
         String customerQueue = "";

        for (var i = 1; i < getQueueLength(); i++) {
            customerQueue += "@";
        }

        return customerQueue;
    }

    public String toString() {
        
        if (open) {
            if (!hasCustomer()) {
                return "  [ ]";
            } else {
                return "  [" + getCustomerIdentifier() + "]  " + getCustomerQueue();
            }
        }
        return "X [ ]";
    }

}
