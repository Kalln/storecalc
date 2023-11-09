import java.util.ArrayList;

import Customers.Customer;

public class Store {
    Register[] registers;

    public Store(int amountOfRegisters) {
        registers = new Register[amountOfRegisters];

        for (var i = 0; i < amountOfRegisters; i++) {
            Register r = new Register(new CustomerQueue());
            registers[i] = r;
        }

        registers[0].open();
    }
    private int amountOpenRegisters() {
        int amount = 0;

        for (Register r : registers) {
            if (r.isOpen()) { amount++; }
        }

        return amount;
    }

    public int getAverageQueueLength() {
        int totalCustomer = 0;

        for (Register register : this.registers) {
            totalCustomer += register.getQueueLength();
        }

        return totalCustomer / amountOpenRegisters();

    }

    public void newCustomer(Customer c) {
        // get shortest queue.
        Register registerShortestQueue = registers[0];
       // System.out.println("registers:" + registers.length);

        for (Register register : this.registers) {
           
            if ((registerShortestQueue.queue.length() > register.queue.length())
                && register.isOpen()) {

                registerShortestQueue = register;
               
            }
        }

        registerShortestQueue.addToQueue(c);

    }

    public void step() {
        // kunden längst fram får en av sina varor registrerade. 
        for (Register r : this.registers) {
            if (r.isOpen() && r.hasCustomer()) {
                // registrera en av varorna.
                Customer c = r.queue.first();
                if (c.isDone()) {
                    // if done we dequeue
                    r.queue.dequeue();
                } else {
                    r.queue.first().serve();
                }
            }
        }
        // kunden som står längst fram och inte har vara kvar lämnar varuhus.
        // om en kund skapas, ställer den sig i en kö-
        // om det 
    }

    public void openNewRegister() {
        for (Register register : this.registers) {
            if (!register.isOpen()) {
                register.open();
                break;
            }
        }

    }

    public ArrayList<Customer> getDoneCustomers() {
        ArrayList<Customer> doneCustomers = new ArrayList<>();

        for (Register r : this.registers) {
            if (r.isOpen()
            && r.queue.first() != null
            && r.queue.first().isDone()) {
                    doneCustomers.add(r.queue.first());
                }
        }
        return doneCustomers;
    }
}
