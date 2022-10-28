#include "../include/Action.h"
#include "../include/Studio.h"
#include <iostream>
#include <string>
#include <vector>
extern Studio* backup;
BaseAction::BaseAction(): errorMsg(""), status(){}

ActionStatus BaseAction::getStatus() const {
    return status;
}
void BaseAction::complete(){
    status=COMPLETED;
}
void BaseAction::error(std::string errorMsgR){
    errorMsg = errorMsgR;
    status=ERROR;
}
std::string BaseAction::getErrorMsg() const{
    return errorMsg;
}

BaseAction::~BaseAction() {}//initializing the destructor because of the open trainer vector

//_________________open______________
OpenTrainer::OpenTrainer(int id, std::vector<Customer *>& customersListInput):trainerId(id),customers(),customersString(""){
    /*
     * open trainer receives a vector with pointers to customers, the vector is created in studio
     * open trainer copys the vector (2 pointers are pointing to the same object).
     * after open trainer is finished it will clear the vecor and delete every customer in it
     */
    for (Customer * customerToAdd : customersListInput){
        customers.push_back(customerToAdd);
    }
}
    void OpenTrainer::act(Studio &studio) {
        Trainer * openThisTrainer = studio.getTrainer(trainerId);
        if ((openThisTrainer == nullptr) || (openThisTrainer->isOpen())) {
            for (Customer *customerToAdd: customers) {
                customersString=customersString+" " + customerToAdd->toString();
                studio.decramentCustomersId();
                delete customerToAdd;
                customerToAdd = nullptr;
            }
            error("Error:Trainer does not exist or is already open\n");
            std::cout << getErrorMsg();
        }
        else {
            openThisTrainer->openTrainer();
            for (Customer *customerToAdd : customers) {
                if (openThisTrainer->getCapacity() > 0) {
                    //creatin a sting for each customer that will be inserted into a trainer, will be used in toString
                    customersString=customersString+" " + customerToAdd->getName() + "," + customerToAdd->getType();
                    //inserting a close of the customer into the trainer so the current customer that open trainer is handeling can be deleted
                    openThisTrainer->addCustomer(customerToAdd->clone());
                    delete customerToAdd;
                    customerToAdd=nullptr;
                }
                else {
                    //if the trainer cannot accept any more customers, the id's need to be decremented(the were incremented in studio open reader)
                    studio.decramentCustomersId();
                    delete customerToAdd;
                    customerToAdd = nullptr;
                }
            }
            complete();
        }
        customers.clear();//clearing the vector of pointers (in this stage, the pointers are null pointers
    }
    std::string OpenTrainer::toString() const{
            std::string returnS="open ";
            std::string trainS=std::to_string(trainerId);
            returnS=returnS+trainS+customersString;
        if(getStatus() == COMPLETED)
            returnS=returnS+" Completed\n";
        else
            returnS=returnS+" "+getErrorMsg();
        return returnS;
    }
    OpenTrainer * OpenTrainer::clone() const {
        return new OpenTrainer(*this);
    }

    OpenTrainer::~OpenTrainer() {// destructor just in case the customers were not deleted in "act"
        for (Customer *customerToDelete: customers) {
            delete customerToDelete;
        }
        customers.clear();
    }

//-----------ORDER:--------------

Order:: Order(int trainerId):trainerId(trainerId),log(""){}
void Order::act(Studio &studio){
    Trainer *trainer=studio.getTrainer(trainerId);
    if ((trainer== nullptr) || (!trainer->isOpen())){
        error("Error: Trainer does not exist or is not open\n");
        std::cout<<getErrorMsg();
    }
    else {
        std::vector < Customer * > cusVec = trainer->getCustomers();
        std::vector<int> workoutsId;
        std::vector <Workout> workoutsPTR = studio.getWorkoutOptions();
        for (std::vector<Customer *>::iterator it = cusVec.begin(); it != cusVec.end(); ++it) {
            Customer *currCus = *it;
            int customerId = currCus->getId();
            std::string customerName = currCus->getName();
            workoutsId = currCus->order(workoutsPTR);//getting the vector of workout id's from the customer
            //the loop will enter the orders from the customer to the trainer and update his salary
            for (std::vector<int>::iterator itWO = workoutsId.begin(); itWO != workoutsId.end(); ++itWO) {
                int workoutId = *itWO;
                Workout currWorkout = studio.getWorkout(workoutId);
                int price = currWorkout.getPrice();
                std::string workoutName = currWorkout.getName();
                log = log + customerName + " Is Doing " + workoutName + "\n";
                int trainerSal = trainer->getSalary();
                trainer->setSalary(trainerSal + price);
            }
            trainer->order(customerId, workoutsId, workoutsPTR);
        }
        std::cout << log;
        complete();
    }
}
std::string Order::toString() const{
    std::string trainS=std::to_string(trainerId);
    std::string returnS="order "+trainS;
    if(this->getStatus()==COMPLETED)
        returnS=returnS+" Completed\n";
    else
        returnS=returnS+" "+ getErrorMsg();
    return returnS;
}
Order *Order::clone() const {
    return new Order(*this);
}


//________________MOVE________________________

MoveCustomer::MoveCustomer(int src, int dst, int customerId):srcTrainer(src),dstTrainer(dst),id(customerId){}
void MoveCustomer::act(Studio &studio){
    //This action can only occur after the workout session has started (OpenTrainer)
    Trainer* srcT=studio.getTrainer(srcTrainer);
    Trainer* dstT=studio.getTrainer(dstTrainer);
    Customer *movedCus= nullptr;
    if(srcT != nullptr)
        movedCus=srcT->getCustomer(id);
    //checking all the requaerments for making the transfer between the trainers
    if((movedCus != nullptr) && (dstT != nullptr) && (srcT->isOpen()) & (dstT->isOpen()) & (dstT->getCapacity()>0) & (studio.getWasOpenTrainer())){
        int srcSal=srcT->getSalary();
        int dstSal=dstT->getSalary();
        int custPay=movedCus->getPayment();
        std::vector<OrderPair> srcOrders=srcT->getOrders();
        int index=0;
        int ordersSourceSize = srcOrders.size();
        //te loop will update the destination trainer's orderList
        while(index < ordersSourceSize) {
            if (srcOrders[index].first == id) {
                Workout currWorkout = srcOrders[index].second;
                OrderPair newPair(id, currWorkout);
                dstT->addToOrderList(newPair);
            }
            index++;
        }
        //updating the salarys and customers list(remove customer will take care of the orders list)
        srcT->setSalary(srcSal-custPay);
        srcT->removeCustomer(id);
        dstT->setSalary(dstSal+custPay);
        dstT->addCustomer(movedCus);
        complete();
    }
    else {
        error("Error: Cannot move customer\n");
        std :: cout << getErrorMsg();
    }
}
std::string MoveCustomer::toString() const {
    std:: string srcS=std::to_string(srcTrainer);
    std:: string dstS=std::to_string(dstTrainer);
    std:: string idS=std::to_string(id);
    std::string returnS="move "+srcS+" "+dstS+" "+idS+" ";
    if(this->getStatus()==COMPLETED)
        returnS=returnS+" Completed\n";
    else
        returnS=returnS+" "+getErrorMsg();
    return returnS;
}
MoveCustomer * MoveCustomer::clone() const{
    return new MoveCustomer(*this);
}
//____________________CLOSE_________________
Close::Close(int id): trainerId(id){}
void Close::act(Studio &studio) {
    Trainer *trainerToClose = studio.getTrainer(trainerId);
    if ((trainerToClose != nullptr) && (trainerToClose->isOpen())) {
        trainerToClose->closeTrainer();
        complete();
    }
    else {
        error("Error: Trainer does not exist or is not open\n");
        std::cout << getErrorMsg();
    }
}
std::string Close::toString() const{
    std::string output ="";
    if(this->getStatus()==COMPLETED)
        output = "Close " +std::to_string(trainerId) + " Completed\n";
    else
        output = getErrorMsg();
    return output;
}

Close *Close::clone() const{
    return new Close(*this);
}
//________________CloseAll________________________


CloseAll::CloseAll(){}
void CloseAll::act(Studio &studio) {
    int n=studio.getNumOfTrainers();
    for(int i=0; i < n; i++){
        Trainer* deleteThisTrainer=studio.getTrainer(i);
        if(deleteThisTrainer->isOpen()){
            deleteThisTrainer->closeTrainer();
        }
    }
    studio.closeStudio();
    complete();
}
std::string CloseAll::toString() const{
    std::string output ="";
    if(this->getStatus()==COMPLETED)
        output = "CloseAll Completed\n";
    return output;
}

CloseAll * CloseAll::clone() const{
    return new CloseAll(*this);
}

//______________________PrintWorkoutOptions______________
PrintWorkoutOptions::PrintWorkoutOptions(): log(""){}

void PrintWorkoutOptions::act(Studio &studio) {
    std::vector<Workout> workoutOptions=studio.getWorkoutOptions();
    int workoutOptionsSize = workoutOptions.size();
    for(int i=0; i < workoutOptionsSize; i++){
        std::string name=workoutOptions[i].getName();
        int price=workoutOptions[i].getPrice();
        std::string priceString=std::to_string(price);
        WorkoutType type=workoutOptions[i].getType();
        std::string typeString;
        if(type==ANAEROBIC)
            typeString="Anaerobic";
        else if(type==CARDIO)
            typeString="Cardio";
        else
            typeString="Mixed";
        log=log+name+", "+typeString+", "+priceString+"\n";
    }
    std::cout<<log;
        workoutOptions.clear();
}

std::string PrintWorkoutOptions::toString() const {
    return "workout_options\n";
}
PrintWorkoutOptions * PrintWorkoutOptions::clone() const{
    return new PrintWorkoutOptions(*this);
}

//_______________PrintTrainerStatus___________
PrintTrainerStatus::PrintTrainerStatus(int id): trainerId(id),log(""){}

void PrintTrainerStatus::act(Studio &studio) {
        Trainer *curTrainer = studio.getTrainer(trainerId);
        std::string idString = std::to_string(trainerId);
        log = "Trainer " + idString + " status:";
    if((studio.getWasOpenTrainer()) && (curTrainer!= nullptr)) {
        if (!curTrainer->isOpen())
            log = log + " closed";
        else {
            log = log + " open";
            log = log + "\n" + "Customers:" + "\n";
            std::vector < Customer * > customers = curTrainer->getCustomers();
            int customersSize = customers.size();
            for (int i = 0; i < customersSize; i++) {
                std::string idString = std::to_string(customers[i]->getId());
                std::string name = customers[i]->getName();
                log = log + idString + " " + name + "\n";
            }
            log = log + "Orders:" + "\n";
            std::vector <OrderPair> orders = curTrainer->getOrders();
            int ordersSize = orders.size();
            for (int i = 0; i < ordersSize; i++) {
                if (orders[i].first == -1)
                    continue;
                std::string workoutName = orders[i].second.getName();
                std::string priceString = std::to_string(orders[i].second.getPrice());
                std::string idString = std::to_string(orders[i].first);
                log = log + workoutName + " " + priceString + "NIS " + idString + "\n";
            }
            std::string salarySting = std::to_string(curTrainer->getSalary());
            log = log + "Current Trainer's Salary: " + salarySting + "NIS";
        }
        std::cout << log << std::endl;
        complete();
    }
    else {
        error("the workout session has not started, or trainer does not exist\n");
        std :: cout << getErrorMsg();
    }
}

std::string PrintTrainerStatus::toString() const {
    std::string returnS="status";
    std:: string idString=std::to_string(trainerId);
    returnS=returnS+idString;
    if(this->getStatus()==COMPLETED)
        returnS=returnS+" Completed\n";
    else
        returnS=returnS+" "+getErrorMsg();
    return returnS;
}
PrintTrainerStatus * PrintTrainerStatus::clone() const{
    return new PrintTrainerStatus(*this);
}


//_____________print action log_______________

PrintActionsLog::PrintActionsLog(){}

void PrintActionsLog::act(Studio &studio) {
    std::vector<BaseAction*> printThis=studio.getActionsLog();
    int printSize = printThis.size();
    for(int i=0;i < printSize;i++){
        std::string printLine=printThis[i]->toString();
        std::cout<<printLine;
    }
    printThis.clear();//clearing the vector of pointers(not the objects they are pointing to!)
}

std::string PrintActionsLog::toString() const {
return "log Completed\n";
}
PrintActionsLog * PrintActionsLog::clone() const{
    return new PrintActionsLog(*this);
}
//_____________________BACKUP_______________
BackupStudio::BackupStudio():BaseAction(){}

void BackupStudio::act(Studio &studio) {
    delete backup;
    backup = nullptr;
    backup = new Studio (studio);
    complete();

}
std::string BackupStudio::toString() const{
    std::string output = "backup ";
    if (getStatus() == COMPLETED)
        output = output + "Completed"+"\n";
    else
        output =getErrorMsg()+"\n";
    return output;
}
BackupStudio * BackupStudio::clone() const{
    return new BackupStudio(*this);
}
//________________RESTORE_____________________
RestoreStudio::RestoreStudio():BaseAction(){}

void RestoreStudio::act(Studio &studio) {
    if(backup == nullptr) {
        error("Error: No backup available\n");
        std :: cout << getErrorMsg();
    }
    else {
        studio = *backup;
        complete();
    }
}
std::string RestoreStudio::toString() const{
    std::string output = "RestoreStudio ";
    if (getStatus() == COMPLETED)
        output = output + "Completed\n";
    else
        output =output + getErrorMsg();
    return output;
}

RestoreStudio * RestoreStudio::clone() const{
    return new RestoreStudio(*this);
}




