#ifndef ACTION_H_
#define ACTION_H_

#include <string>
#include <iostream>
#include "Customer.h"


enum ActionStatus{
    COMPLETED, ERROR
};
//Forward declaration
class Studio;

class BaseAction{
public:
    BaseAction();
    ActionStatus getStatus() const;
    virtual void act(Studio& studio)=0;
    virtual std::string toString() const=0;
    virtual BaseAction* clone() const=0;
    virtual ~BaseAction();
protected:
    void complete();
    void error(std::string errorMsg);
    std::string getErrorMsg() const;
private:
    std::string errorMsg;
    ActionStatus status;
};


class OpenTrainer : public BaseAction {
public:
    OpenTrainer(int id, std::vector<Customer *> &customersList);
    void act(Studio &studio);
    std::string toString() const;
    OpenTrainer* clone() const;
    virtual ~OpenTrainer();
private:
	const int trainerId;
	std::vector<Customer *> customers;
    std::string customersString;
};


class Order : public BaseAction {
public:
    Order(int id);
    void act(Studio &studio);
    std::string toString() const;
    Order* clone() const;
private:
    const int trainerId;
    std::string log;//TODO should delete?
};


class MoveCustomer : public BaseAction {
public:
    MoveCustomer(int src, int dst, int customerId);
    void act(Studio &studio);
    std::string toString() const;
    MoveCustomer* clone() const;

private:
    const int srcTrainer;
    const int dstTrainer;
    const int id;
};


class Close : public BaseAction {
public:
    Close(int id);
    void act(Studio &studio);
    std::string toString() const;
    Close* clone() const;

private:
    const int trainerId;
};


class CloseAll : public BaseAction {
public:
    CloseAll();
    void act(Studio &studio);
    std::string toString() const;
    CloseAll* clone() const;
private:
};


class PrintWorkoutOptions : public BaseAction {
public:
    PrintWorkoutOptions();
    void act(Studio &studio);
    std::string toString() const;
    PrintWorkoutOptions* clone() const;

private:
    std::string log;
};


class PrintTrainerStatus : public BaseAction {
public:
    PrintTrainerStatus(int id);
    void act(Studio &studio);
    std::string toString() const;
    PrintTrainerStatus* clone() const;
private:
    const int trainerId;
    std::string log;
};


class PrintActionsLog : public BaseAction {
public:
    PrintActionsLog();
    void act(Studio &studio);
    std::string toString() const;
    PrintActionsLog* clone() const;
private:
};


class BackupStudio : public BaseAction {
public:
    BackupStudio();
    void act(Studio &studio);
    std::string toString() const;
    BackupStudio* clone() const;
private:
};


class RestoreStudio : public BaseAction {
public:
    RestoreStudio();
    void act(Studio &studio);
    std::string toString() const;
    RestoreStudio* clone() const;

};


#endif