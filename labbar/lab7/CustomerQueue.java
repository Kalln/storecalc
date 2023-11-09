import java.util.LinkedList;

import Customers.Customer;

import java.util.*;

public class CustomerQueue {
    public class EmptyQueueException extends RuntimeException{}

    private Queue<Customer> queue;
    
    public CustomerQueue() {
        this.queue = new LinkedList<>();
    }

    public int length() {
        return queue.size();
    }

    public void enqueue(Customer c) {
        this.queue.add(c);
    }

    public Customer dequeue() {
        if (queue.isEmpty()) {
            throw new EmptyQueueException();
        }
        return this.queue.remove();
    }

    public boolean isEmpty() {
        return queue.isEmpty();
    }

    public Customer first() {
        return queue.peek();
    }
}
