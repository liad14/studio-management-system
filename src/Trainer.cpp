#include "../include/Trainer.h"
#include <iostream>
#include <vector>
#include <Studio.h>

typedef std::pair<int, Workout> OrderPair;
//_________Constructor______________
    Trainer::Trainer(int t_capacity): id(0), capacity(t_capacity), salary(0), everOpened(false), open(false), customersList(), orderList(){}
//_________________Copy Constructor______________
Trainer::Trainer(Trainer &other): id(other.getId()), capacity(other.getCapacity()), salary(other.getSalary()), everOpened(other.wasEveropened()), open(other.isOpen()), customersList(), orderList(){
   //"deep copy" of the elements from the other's vector to the new trainer constructed:
   int otherCustomersSize = other.getCustomers().size();
   for(int i = 0;i < otherCustomersSize; i++){
        Customer *newCus = other.getCustomers()[i];
        customersList.push_back(newCus->clone()); //add each customer of other to the copy created trainer customers list
   } // by creating a new pointer of each customer, according to its type, using the method clone()
    int otherOrdersSize = other.getOrders().size();
    for(int j = 0; j < otherOrdersSize; j++){
            OrderPair newPair=other.getOrders()[j];
            orderList.push_back(newPair);
        }
    }
//_________________Move Constructor______________
Trainer::Trainer(Trainer &&other): id(other.getId()), capacity(other.getCapacity()), salary(other.getSalary()), everOpened(other.wasEveropened()), open(other.isOpen()), customersList(), orderList(other.orderList) {
    for (Customer* customerToCopy : other.customersList) { //"deep copy" of the customersList vector of other as in the copy constructor
        customersList.push_back(customerToCopy->clone());
    }
    other.customersList.clear();
}
//_________________Copy Assignment Operator______________
Trainer& Trainer::operator = (const Trainer &other) {
    if (this != &other) {
        closeTrainer();
        capacity = other.capacity;
        id = other.id;
        salary = other.salary;
        everOpened = other.everOpened;
        open = other.open;

        for (Customer* customerToCopy : other.customersList) { //deep copy of other customers
            customersList.push_back(customerToCopy->clone());
        }
        orderList.clear();
        for (const OrderPair& newPair : other.orderList) { //deep copy of other orders
            orderList.emplace_back(newPair.first, newPair.second);
        }
    }
    return *this;
}
//_________________Move Assignment Operator______________
Trainer& Trainer::operator = (Trainer &&other) {
    closeTrainer();
    capacity = other.getCapacity();
    other.capacity = -1;
    id = other.getId();
    other.id = -1;
    salary = other.getSalary();
    other.salary = -1;
    everOpened = other.wasEveropened();
    other.everOpened = false;
    open = other.isOpen();
    other.open = false;
    for (Customer* customerToCopy : other.customersList) { //"deep move" - copy & delete of each customer pointer of other
        customersList.push_back(customerToCopy->clone());
        delete customerToCopy;
    }
    other.customersList.clear();
    int otherOrdersSize = other.getOrders().size();
    for(int j=0;j < otherOrdersSize;j++){ //"deep move" - copy of each order of other, and clear the vector
        OrderPair newPair=other.getOrders()[j]; //(no deep delete because orderPair is not a pointer)
        orderList.push_back(newPair);
    }
    other.orderList.clear();
    return *this;
}
//_________________Destructor______________
Trainer::~Trainer() { //deep delete of the pointers (the customers pointers) that Trainer maintained at the customers vector (and delete the primitive fields)
    for (Customer *customer: customersList) {
        delete customer;
    }
}
void Trainer::closeTrainer() { //close this trainer, make his vectors and capacity empty (deep delete for the customers pointers)
        open = false; //so get ready for the next open or for closing. the salary accumulated
        capacity = capacity + customersList.size();
        for (Customer *customerToDelete: customersList) {
                delete customerToDelete;
        }
        customersList.clear();
        orderList.clear();
        std::cout << "Trainer " << getId() << " closed. Salary " << getSalary() << "NIS" << std::endl;

}

int Trainer::getCapacity() const{
        return capacity;
    }
    void Trainer::addCustomer(Customer* customer){
        Trainer::customersList.push_back(customer);
        capacity--;
    }
    void Trainer::removeCustomer(int id){ //remove this customer from this trainer - from customersList, his order from ordersList
        int otherCustomersSize = customersList.size();
        for (int index = 0; index < otherCustomersSize; index ++){
            if (customersList[index]->getId() == id)
                customersList.erase(customersList.begin() + index);
        } //update the capacity
        capacity++;
        int ordersSize = getOrders().size();
        for(int i=0;i < ordersSize;i++){
            if(orderList[i].first==id)
                orderList[i].first=-1;
        }
        if(customersList.size()==0) //if this trainer has no more customers - close this trainer.
            closeTrainer();
    }
    Customer* Trainer::getCustomer(int id){
        Customer* customer;
        int i=0;
        int CustomersSize = customersList.size();
        while(i<CustomersSize){
            if(customersList[i]->getId()==id){
                customer=customersList[i];
                break;
            }
            i++;
        }
        if (i==CustomersSize)
            return nullptr;
        return customer;
    }
    std::vector<Customer*>& Trainer::getCustomers(){
        return customersList;
    }
    std::vector<OrderPair>& Trainer::getOrders(){
        return orderList;
    }
    void Trainer::order(const int customer_id, const std::vector<int> workout_ids, const std::vector<Workout>& workout_options) { //
        int workutsIds = workout_ids.size();
        for (int i = 0; i < workutsIds; i++) {
            int nextWorkoutId = workout_ids[i];
            Workout newWorkout = workout_options[nextWorkoutId];
            OrderPair newPair(customer_id, newWorkout);
            orderList.push_back(newPair);
        }
    }
    void Trainer::openTrainer() {
        open = true;
        everOpened = true;
    }
    int Trainer::getSalary(){
        return salary;
    }
    void Trainer::setSalary(int newSalary){
        salary = newSalary;
    }
    bool Trainer::isOpen(){
        return open;
    }
    bool Trainer::wasEveropened() {
        return everOpened;
    }

    void Trainer::setId(int newId) {
        id = newId;
    }
    int Trainer::getId() {
        return id;
    }
    void Trainer::addToOrderList(OrderPair newPair) {
        orderList.push_back(newPair);
    }