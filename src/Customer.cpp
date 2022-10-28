//
// Created by spl211 on 16/11/2021.
//
#include "../include/Customer.h"
#include <algorithm>
#include <iostream>

Customer::Customer(std::string c_name, int c_id):name(c_name),id(c_id),payment(0){}
Customer::~Customer(){}//default destructor, because heavy and full body have resources
int Customer::getId() const{
    return id;
}
std::string Customer::getName() const{
    return name;
}
int Customer::getPayment() const{
    return payment;
}
void Customer::setPayment(int newPayment){
    payment=newPayment;
}

Customer *Customer::clone() {
    return nullptr;
}


//______________Sweaty____________
SweatyCustomer::SweatyCustomer(std::string name, int id):Customer(name,id){}
std::vector<int> SweatyCustomer::order(const std::vector<Workout> &workout_options){
    std::vector<int> myOrder;
    int workoutOptionSize = workout_options.size();
    for(int i=0;i < workoutOptionSize;i++){
        if(workout_options[i].getType()==CARDIO) {
            int newPayment= getPayment() + workout_options[i].getPrice();
            setPayment(newPayment);
            myOrder.push_back(i);
        }
    }
    return myOrder;
}
std::string SweatyCustomer::toString() const{
    return getName() + ",swt";
}
std::string SweatyCustomer::getType(){
    return "swt";
}
SweatyCustomer* SweatyCustomer:: clone(){
    return new SweatyCustomer(*this);
}


//__________________Cheap_______________
CheapCustomer::CheapCustomer(std::string name, int id):Customer(name,id){}
std::vector<int> CheapCustomer::order(const std::vector<Workout> &workout_options){
    std::vector<int> myOrder;
    int orderId=0;
    int workoutOptionSize = workout_options.size();
    for(int i=0;i < workoutOptionSize;i++){
        if(workout_options[i].getPrice() < workout_options[orderId].getPrice())
            orderId=i;
    }
    myOrder.push_back(orderId);
    int newPayment=getPayment() + workout_options[orderId].getPrice();
    setPayment(newPayment);
    return myOrder;
}
std::string CheapCustomer::toString() const{
    return getName() + ",chp";
}
std::string CheapCustomer::getType(){
    return "chp";
}
CheapCustomer* CheapCustomer:: clone(){
    return new CheapCustomer(*this);
}
//_______________________ muscle_________________
HeavyMuscleCustomer::HeavyMuscleCustomer(std::string name, int id):Customer(name,id), sorted_options() {}
HeavyMuscleCustomer::~HeavyMuscleCustomer(){
    sorted_options.clear();
}
std::vector<int> HeavyMuscleCustomer::order(const std::vector<Workout> &workout_options){
    std::vector<int> myOrder;
    //creating a pair vector that can be sorted
    int workoutOptionSize = workout_options.size();
    for(int i=0;i < workoutOptionSize;i++){
        int price = workout_options[i].getPrice();
        std::pair<int,int> currPair(i,price);
        sorted_options.push_back(currPair);
    }
    //sorting the vector with our specifications
    std::sort(sorted_options.begin(),sorted_options.end(),[](std::pair<int,int> lhs, std::pair<int,int> rhs){
        if(lhs.second == rhs.second) {
            return lhs.first < rhs.first;
        }
            return lhs.second > rhs.second;
                });
    for (int i = 0 ; i < workoutOptionSize ; i++){
        int workoutId = sorted_options[i].first;
        WorkoutType workoutType = workout_options[workoutId].getType();
        if(workoutType == ANAEROBIC) {
            myOrder.push_back(workoutId);
            int newPayment= getPayment() + workout_options[workoutId].getPrice();
            setPayment(newPayment);
        }
    }
    return myOrder;
}
std::string HeavyMuscleCustomer::toString() const{
    return getName() + ",mcl";
}
std::string HeavyMuscleCustomer::getType(){
    return "mcl";
}
HeavyMuscleCustomer* HeavyMuscleCustomer:: clone(){
    return new HeavyMuscleCustomer(*this);
}
//__________________full body_____________________

FullBodyCustomer::FullBodyCustomer(std::string name, int id):Customer(name,id), sorted_options() {}
FullBodyCustomer::~FullBodyCustomer(){
    sorted_options.clear();
}
std::vector<int> FullBodyCustomer::order(const std::vector<Workout> &workout_options){
    std::vector<int> myOrder;
    //loop to create a vector of pairs so that it could be sorted
    int workoutOptionSize = workout_options.size();
    for(int i=0;i < workoutOptionSize;i++){
        int price=workout_options[i].getPrice();
        std::pair<int,int> currPair(price,i);
        sorted_options.push_back(currPair);
    }
    std::sort(sorted_options.begin(),sorted_options.end());
    //initializing parameters to save the mixed workout with the highest price so that the one with the smallest id could be selected
    int theMixId=-1;
    int mixPrice=-1;
    bool foundAnyMix=false;
    for(int i=0;i < workoutOptionSize;i++){
        int workoutId=sorted_options[i].second;
        Workout currWO=workout_options[workoutId];
        WorkoutType workoutType=currWO.getType();
        if(workoutType == MIXED){
            if(!foundAnyMix) {
                theMixId = currWO.getId();
                mixPrice = currWO.getPrice();
                foundAnyMix=true;
            }
            else if(mixPrice<currWO.getPrice()) {
                theMixId = currWO.getId();
                mixPrice = currWO.getPrice();
            }
        }
    }
    int theAnaId=-1;
    bool orderedAna=false,orderedCardio=false;
    for (int i=0; (i < workoutOptionSize) && (!(orderedAna & orderedCardio));i++){
        int workoutId=sorted_options[i].second;
        WorkoutType workoutType=workout_options[workoutId].getType();
        if ((workoutType==ANAEROBIC) && (!orderedAna)) {
            theAnaId = workoutId;
            orderedAna=true;}
        if ((workoutType==CARDIO) && (!orderedCardio)) {
            int newPayment= getPayment() + workout_options[workoutId].getPrice();
            setPayment(newPayment);
            myOrder.push_back(workoutId);
            orderedCardio=true;
        }
    }
    //if there are no anaerobic or cardio exercises
    if(theMixId != -1) {
        myOrder.push_back(theMixId);
        int newPayment= getPayment() + workout_options[theMixId].getPrice();
        setPayment(newPayment);
    }
    if(theAnaId != -1) {
        myOrder.push_back(theAnaId);
        int newPayment= getPayment() + workout_options[theAnaId].getPrice();
        setPayment(newPayment);
    }

    return myOrder;
}
std::string FullBodyCustomer::toString() const{
    return getName() + ",fbd";
}
std::string FullBodyCustomer::getType(){
    return "fbd";
}
FullBodyCustomer* FullBodyCustomer::clone(){
    return new FullBodyCustomer(*this);
}


