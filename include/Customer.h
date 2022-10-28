#ifndef CUSTOMER_H_
#define CUSTOMER_H_

#include <vector>
#include <string>
#include "Workout.h"

class Customer{
public:
    Customer(std::string c_name, int c_id);
    virtual ~Customer();
    virtual std::vector<int> order(const std::vector<Workout> &workout_options)=0;
    virtual std::string toString() const = 0;
    std::string getName() const;
    int getId() const;
    int getPayment() const;
    void setPayment(int newPayment);
    virtual std::string getType()=0;
    virtual Customer* clone();
private:
    const std::string name;
    const int id;
    int payment;

};

class SweatyCustomer : public Customer {
public:
	SweatyCustomer(std::string name, int id);
    std::vector<int> order(const std::vector<Workout> &workout_options);
    std::string toString() const;
    std::string getType();
    SweatyCustomer* clone();
private:
};


class CheapCustomer : public Customer {
public:
	CheapCustomer(std::string name, int id);
    std::vector<int> order(const std::vector<Workout> &workout_options);
    std::string toString() const;
    std::string getType();
    CheapCustomer* clone();
private:
};


class HeavyMuscleCustomer : public Customer {
public:
	HeavyMuscleCustomer(std::string name, int id);
    virtual ~HeavyMuscleCustomer();
    std::vector<int> order(const std::vector<Workout> &workout_options);
    std::string toString() const;
    std::string getType();
    HeavyMuscleCustomer* clone();
private:
    std::vector<std::pair<int,int>> sorted_options;//TODO delete
};


class FullBodyCustomer : public Customer {
public:
	FullBodyCustomer(std::string name, int id);
    virtual ~FullBodyCustomer();
    std::vector<int> order(const std::vector<Workout> &workout_options);
    std::string toString() const;
    std::string getType();
    FullBodyCustomer* clone();
private:
    std::vector<std::pair<int,int>> sorted_options;
};


#endif