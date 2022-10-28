#ifndef TRAINER_H_
#define TRAINER_H_

#include <vector>
#include "Customer.h"
#include "Workout.h"

typedef std::pair<int, Workout> OrderPair;

class Trainer{
public:
    Trainer(int t_capacity); //Constructor
    Trainer(Trainer &other); //Copy Constructor
    Trainer(Trainer &&other); //Move Constructor
    Trainer& operator = (const Trainer &other); //Copy Assignment Operator
    Trainer& operator = (Trainer &&other); //Move Assignment Operator
    virtual ~Trainer(); //Destructor
    void closeTrainer();
    int getCapacity() const;
    void addCustomer(Customer* customer);
    void removeCustomer(int id);
    Customer* getCustomer(int id);
    std::vector<Customer*>& getCustomers();
    std::vector<OrderPair>& getOrders();
    void order(const int customer_id, const std::vector<int> workout_ids, const std::vector<Workout>& workout_options);
    void openTrainer();
    int getSalary();
    void setSalary(int newSalary);
    bool isOpen();
    bool wasEveropened(); //return whether this trainer did ever open
    void setId(int id);
    int getId();
    void addToOrderList(OrderPair newPair); // a method that add the given order pair to the order list of this trainer
    // OrderPair segments: first segment - the customer's order id, second segment - the ordered workout)




private:
    int id;
    int capacity;
    int salary;
    bool everOpened;
    bool open;
    std::vector<Customer*> customersList;
    std::vector<OrderPair> orderList; //A list of pairs for each order for the trainer - (customer_id, Workout)
};


#endif