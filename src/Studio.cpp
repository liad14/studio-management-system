#include <sstream>
#include "Studio.h"
#include "fstream"
#include <iostream>

Studio::Studio(const std::string &configFilePath): open(true), wasOpenTrainer(false), trainers(), workout_options(), actionsLog(), customersId(0) {
  std::fstream confiFile;
    confiFile.open(configFilePath);
    std::string test = "test is not started";
    int counter = 0;
    int numTrainers = 0;
    if (confiFile.is_open()) {
       while (getline(confiFile, test)) {
            if (test.empty() || test[0] == '#')
                continue;
           //initialize the right number of trainers as in input - create a pointer of Trainers and add it to the trainers vector
            if (counter == 0) {
                std::stringstream intTrainers(test);
                intTrainers >> numTrainers;
                 }
            //for the second line of input - with the trainers capacity
            if (counter == 1) {
                std::string::size_type capEnd;;//index for the substrings
                for (int i = 0; i < numTrainers; i++) {
                    int trainerCap = std::stoi(test,&capEnd);
                    Trainer *tmpTrainer = new Trainer(trainerCap);
                    tmpTrainer->setId(i);
                    trainers.push_back(tmpTrainer);
                    if(test.length()>capEnd)
                        test = test.substr(capEnd+1);
                }
            }
            if (counter > 1) { //initialize the available workouts for this studio as in input - create a Workout accoriding to its type and add it to the workout_options vector
                int index = 0;
                int comIn = test.find(",");
                std::string workout_name=test.substr(index, comIn);
                index = comIn + 2;
                comIn = test.find(",",index);
                std::string workout_type=test.substr(index, comIn-index);
                WorkoutType workoutType;
                if (workout_type == "Anaerobic")
                    workoutType = ANAEROBIC;
                else if (workout_type == "Mixed")
                    workoutType = MIXED;
                else if (workout_type == "Cardio")
                    workoutType = CARDIO;
                index = comIn + 1;
                int price=std::stoi(test.substr(index));
                Workout tmpWork =Workout(counter - 2, workout_name, price, workoutType);
                workout_options.push_back(tmpWork);
            }
            counter++;
        }
    } else
        std::cout << "the file is not open" << std::endl;// TODO delete this
//_________________Copy Constructor______________
    //"deep copy" of the elements from the other's vector to the new studio constructed - copy each element at the vector

}
Studio::Studio(Studio &other):open(other.getOpen()),wasOpenTrainer(other.getWasOpenTrainer()), trainers(), workout_options(),
actionsLog(),customersId(other.getCustomersId()){
    int otherTrainersSize= other.getNumOfTrainers();
    for (int i=0;i<otherTrainersSize;i++){
        Trainer *curTrain=other.getTrainer(i);
        Trainer *newTrainer=new Trainer(*curTrain);
        trainers.push_back(newTrainer);
    }
    int workoutSize=other.getWorkoutOptions().size();
    for (int i=0;i<workoutSize;i++){
        workout_options.push_back(other.getWorkoutOptions()[i]);
    }
    int actionsSize=other.getActionsLog().size();
    for(int i=0;i<actionsSize;i++){
        BaseAction *newAction=other.getActionsLog()[i];
        actionsLog.push_back(newAction->clone()); //add each Action of other to the copy created Studio actionsLog vector
     // by creating a new pointer of each action, according to its type, using the method clone()
    }
}
//_________________Move Constructor______________
Studio::Studio(Studio &&other):open(other.open), wasOpenTrainer(other.wasOpenTrainer), trainers(), workout_options(),
    actionsLog(),customersId(other.customersId) {
    //"deep move" - copy and clear of the others vectors to this constructed studio
    int otherTrainersSize= other.getNumOfTrainers();
    for (int i=0;i<otherTrainersSize;i++){
        Trainer *curTrainer=other.getTrainer(i);
        trainers.push_back(curTrainer);
    }
    other.trainers.clear();
    int actionsSize=other.getActionsLog().size();
    for(int i=0;i<actionsSize;i++){
        BaseAction *newAction=other.getActionsLog()[i];
        actionsLog.push_back(newAction);
    }
    other.actionsLog.clear();
}
//_________________Copy Assignment Operator______________

Studio& Studio::operator = (const Studio &other) {
    if(this != &other) { //if this studio is not other, delete this studio and copy the others fields and vector
        deleteStudio(); //otherwise return a pointer to this
        customersId = other.customersId;
        open = other.open;
        wasOpenTrainer = other.wasOpenTrainer;
        for (Trainer* trainerToCopy : other.trainers) {
            Trainer *curTrain=trainerToCopy;
            Trainer *newTrainer=new Trainer(*curTrain);
            trainers.push_back(newTrainer);
        }
        for (BaseAction* actionToCopy : other.actionsLog) {
            actionsLog.push_back(actionToCopy->clone());
        }
        int workoutSize=other.workout_options.size();
        for (int i=0;i<workoutSize;i++){
            workout_options.push_back(other.workout_options[i]);
        }
    }
    return *this;
}
//________Move Assignment Operator___________
Studio& Studio::operator = (Studio &&other) {
    if (this != &other) {//if this studio is not other, delete this studio and move - copy&delete the others fields and vector
        //otherwise just return a pointer to this
        deleteStudio();
        open = other.open ;
        wasOpenTrainer = other.wasOpenTrainer;
        customersId = other.customersId;
        int otherTrainersSize= other.getNumOfTrainers();
        for (int i=0;i<otherTrainersSize;i++){
            Trainer *curTrainer=other.getTrainer(i);
            trainers.push_back(curTrainer);
        }
        other.trainers.clear();
        other.open = false;
        other.wasOpenTrainer = false;
        other.customersId = -1;
        for (Workout &workoutToMove: other.getWorkoutOptions()) {
            workout_options.push_back(workoutToMove);
        }
        int actionsSize=other.getActionsLog().size();
        for(int i=0;i<actionsSize;i++){
            BaseAction *newAction=other.getActionsLog()[i];
            actionsLog.push_back(newAction);
        }
        other.actionsLog.clear();
    }
    return *this;
}
//_________________Destructor______________
Studio::~Studio() { //the destructor delete by itself the primitive fields of this studio,
        deleteStudio(); //and calls to the function deleteStudio for the deep deletions

}

void Studio::deleteStudio(){ //deep delete of the vectors of studio: delete each trainer pointer, workout, and action pointer, and clears the vectors
    clearTrainers();
    trainers.clear();
    workout_options.clear();
    clearActionsLog();
    actionsLog.clear();
}
void Studio::clearTrainers() {
    for (Trainer *trainerToDelete: trainers) {
            delete trainerToDelete;
            trainerToDelete = nullptr;
    }
}
    void Studio::clearActionsLog() {
        for (BaseAction *actionToDelete : actionsLog) {
            delete actionToDelete;
            actionToDelete = nullptr;
        }

    }

//______________AFTER RULE OF FIVE____________________

    int Studio::getNumOfTrainers() const{
    return trainers.size();
}
    Trainer* Studio::getTrainer(int tid){
    int trainersSize = trainers.size();
    if(tid >= trainersSize)
        return nullptr;
    return trainers[tid];
}
const std::vector<BaseAction*>& Studio::getActionsLog() const{
    return actionsLog;
}

    std::vector<Workout>& Studio::getWorkoutOptions(){
    return workout_options;
}
    Workout Studio::getWorkout(int workoutId){
    return workout_options[workoutId];
}

    void Studio::start() { // read the start of the input, calls the right Action or create it, act it and push to actionsLog
        std::cout << "Studio is now open!" << std::endl;
        std::string curr_command = "";
        getline(std::cin,curr_command);
        while (curr_command != "closeall") {
            int indexStart = 0;
            int indexEnd = curr_command.find(" ");
            if (indexEnd == -1)
                indexEnd = curr_command.length();
            std::string command(curr_command.substr(indexStart, indexEnd - indexStart));
            if (command == "open") {
                curr_command = curr_command.substr(indexEnd + 1);
                openTrainerReader(curr_command);
            } else if (command == "order") {
                int trainerIdInt= std::stoi(curr_command.substr(indexEnd+1));
                BaseAction *action=  new Order(trainerIdInt);
                action->act(*this);
                actionsLog.push_back(action);
            } else if (command == "move") {
                curr_command = curr_command.substr(indexEnd + 1);
                moveReader(curr_command);
            } else if (command == "close") {
                int trainerId= std::stoi(curr_command.substr(indexEnd));
                BaseAction * action=new Close(trainerId);
                action->act(*this);
                actionsLog.push_back(action);
            } else if (command == "workout_options") {
                BaseAction * action=new  PrintWorkoutOptions();
                action->act(*this);
                actionsLog.push_back(action);
            } else if (command == "status") {
                int trainerId= std::stoi(curr_command.substr(indexEnd));
                BaseAction *action=new PrintTrainerStatus(trainerId);
                action->act(*this);
                actionsLog.push_back(action);
            } else if (command == "log") {
                BaseAction * action=new PrintActionsLog();
                action->act(*this);
                actionsLog.push_back(action);
            } else if (command == "backup") {
                BaseAction * action=new BackupStudio();
                action->act(*this);
                actionsLog.push_back(action);
            } else if (command == "restore") {
                BaseAction * action=new RestoreStudio();
                action->act(*this);
                actionsLog.push_back(action);
            } else
                std::cout << "the command you typed is not valid" << std::endl;
            getline(std::cin,curr_command);
        }
        if (curr_command == "closeall") {
            BaseAction *action = new CloseAll();
            actionsLog.push_back(action);
            action->act(*this);
        }

    }
        void Studio::openTrainerReader(std::string input) { // read an input of open a trainer, create the customers according to its type,
            std::vector<Customer*> customersList;
            unsigned int index = 0;
            int nextCom = 0;
            std::string::size_type nextCustomer;
            int intTrainerId;
            intTrainerId = std::stoi(input, &nextCustomer);
            int nextSpace = nextCustomer;
            index = nextSpace + 1;
            if(index < input.length()){
                input = input.substr(index);
                index = 0;
                while (input.length() > 3) {
                    nextCom = input.find(',');
                    nextSpace = input.find(' ');
                    std::string customerName = input.substr(index, nextCom);
                    std::string customerType = input.substr(nextCom + 1, 3);
                    Customer *newCustomer;
                    if (customerType == "swt")
                        newCustomer = new SweatyCustomer(customerName, customersId);
                    else if (customerType == "chp")
                        newCustomer = new CheapCustomer(customerName, customersId);
                    else if (customerType == "mcl")
                        newCustomer = new HeavyMuscleCustomer(customerName, customersId);
                    else if (customerType == "fbd")
                        newCustomer = new FullBodyCustomer(customerName, customersId);
                    customersList.push_back(newCustomer);
                    customersId++;
                    nextSpace = input.find(' ');
                    nextSpace++;
                    input = input.substr(nextSpace);
                    if (nextSpace < 1)
                        break;
                } // call to the action OpenTrainer, act it and push to the actionsLog
                BaseAction * action = new OpenTrainer(intTrainerId, customersList);
                action->act(*this);
                actionsLog.push_back(action);
                wasOpenTrainer = true;
                customersList.clear();
            }
        }
// read an input of move customer, calls correctly to the action MoveCustomer, act it and add it to actoionsLog vector
        void Studio::moveReader(std::string &input){
            int origTrainerId,destTrainerId,customerId;
            std::string::size_type numEnd;
            origTrainerId=std::stoi(input,&numEnd);
            input=input.substr(numEnd);
            destTrainerId= std::stoi(input,&numEnd);
            input=input.substr(numEnd);
            customerId= std::stoi(input,&numEnd);
            BaseAction * action = new MoveCustomer(origTrainerId,destTrainerId,customerId);
            action->act(*this);
            actionsLog.push_back(action);
        }


        int Studio::getCustomersId(){
            return customersId;
        }
bool Studio::getWasOpenTrainer(){
    return wasOpenTrainer;
}
bool Studio::getOpen(){
    return open;
}
void Studio::closeStudio(){
    open=false;
}
void Studio::decramentCustomersId(){
    customersId--;
}


// Created by spl211 on 16/11/2021.
//

