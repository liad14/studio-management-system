#ifndef STUDIO_H_
#define STUDIO_H_

#include <vector>
#include <string>
#include "Workout.h"
#include "Trainer.h"
#include "Action.h"
#include <algorithm>




class Studio{
public:
	Studio();//Constructor
    Studio(const std::string &configFilePath);
    Studio( Studio &other);//Copy Constructor
    Studio(Studio &&other);//Move Constructor
    Studio& operator=(const Studio &other); //Copy Assignment Operator
    Studio& operator = (Studio &&other); //Move Assignment Operator
    virtual ~Studio(); //Destructor
    void deleteStudio(); //delete function that called from destructr
    void clearTrainers(); //deep delete - delete each pointer of trainer at trainers vector
    void clearActionsLog(); //deep delete - delete each pointer of action at actionsLog vector
    void start(); //open the studio and activate the defined actions according to the input from the user
    int getNumOfTrainers() const;
    Trainer* getTrainer(int tid);
	const std::vector<BaseAction*>& getActionsLog() const; // Return a reference to vector that contains the the history of actions
    std::vector<Workout> &getWorkoutOptions(); //Return a reference to vector that contains the available workouts at this studio
    void openTrainerReader(std::string input); //get the input from the method start() and call to Action::OpenTrainer
    //active the open of the right trainer
    void moveReader(std::string &curr_command); //call to the action MoveCustomer according to the input
    void closeReader(std::string &curr_command); //call to the action Close according to the input
    void printTrainerStatusReader(std::string &curr_command); //call to the action PrintTrainerStatus according to the input
    Workout getWorkout(int workoutId);
    int getCustomersId();
    bool getWasOpenTrainer();
    bool getOpen();
    void closeStudio();
    void decramentCustomersId();


private:
    bool open;
    bool wasOpenTrainer;
    std::vector<Trainer*> trainers;
    std::vector<Workout> workout_options;
    std::vector<BaseAction*> actionsLog;
    int customersId;
};

#endif