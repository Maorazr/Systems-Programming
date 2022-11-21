#include "../include/Action.h"
#include <string>
#include <iostream>
#include "../include/Customer.h"
#include "../include/Trainer.h"
#include "../include/Studio.h"
    extern Studio* backup;

    // Start Of BaseAction Abstract Class
    BaseAction::BaseAction() : errorMsg(""), status() {} // Constructor

    ActionStatus BaseAction::getStatus() const {return status;}

    void BaseAction::complete() {status = COMPLETED;}

    void BaseAction::error(std::string errorMsg) {
        status = ERROR;
        this->errorMsg = std::move(errorMsg);
}
    std::string BaseAction::getErrorMsg() const {return errorMsg;}

    std::string BaseAction::typeToString(int i) {
    if (i == 0)
        return "Anaerobic";
    else if (i == 1)
        return "Mixed";
    else return "Cardio";
}



    OpenTrainer::OpenTrainer(int id, std::vector<Customer *> &customersList) :BaseAction(), trainerId(id), customers(customersList), msg(){}

    void OpenTrainer::act(Studio &studio) {
        Trainer* trainer = studio.getTrainer(trainerId);

        if (trainer == nullptr || trainer->isOpen()) {
            std::cout << "Trainer does not exist or is not open\n";   // print it in error
            error("Trainer does not exist or is not open");
        }
        else {
            trainer->openTrainer();
            for(unsigned int i = 0; i < customers.size() && trainer->getCapacity() > 0; i++){
                trainer->addCustomer(customers[i]);
                msg.append(customers[i]->getName() + "," + customers[i]->getType() + " ");
            }
            complete();
        }
    }
    OpenTrainer::~OpenTrainer() {

    }

   std::string OpenTrainer::toString() const {
       std::string openMsg( "open " + std::to_string(trainerId) + " " + msg);
       if (getStatus() == COMPLETED)
           openMsg.append("Completed");
       else openMsg.append("Error: " + getErrorMsg());
       return openMsg;
    }

    BaseAction *OpenTrainer::clone() {
        return (new OpenTrainer(*this));
    }

    Order::Order(int id) : BaseAction(), trainerId(id){}

    void Order::act(Studio &studio) {

        Trainer* trainer = studio.getTrainer(trainerId);
        if (trainer == nullptr || !trainer->isOpen()) {
            std::cout << "Trainer does not exist or is not open\n";
            error("Trainer does not exist or is not open");
        }
        else {
            std::vector<Customer> names;
            std::vector<Workout> temp = studio.getWorkoutOptions(); ////
            for (auto customer : trainer->getCustomers()){
                trainer->order(customer->getId(),customer->order(temp), temp);
            }
            for(auto t : trainer->getOrders()){
                std::cout << trainer->getCustomer(t.first)->toString() + t.second.getName() + "\n";
            }

        }
    }

    std::string Order::toString() const {
        std::string msg("order " + std::to_string(trainerId) + " ");
        if (getStatus() == COMPLETED){
            msg.append("Completed");
        }
        else if(getStatus() == ERROR){
            msg.append("Error: " + getErrorMsg());
        }
        return msg;
    }

    BaseAction *Order::clone() {
        return (new Order(*this));
    }

    MoveCustomer::MoveCustomer(int src, int dst, int customerId) : BaseAction(),srcTrainer(src),dstTrainer(dst),id(customerId){};

    void MoveCustomer::act(Studio &studio) {  ///  Check the implement is full
        Trainer* trainerSrc = studio.getTrainer(srcTrainer);
        Trainer* trainerDst = studio.getTrainer(dstTrainer);


        if ((trainerSrc != nullptr && trainerDst != nullptr) && (trainerSrc->isOpen() && trainerDst->isOpen()) && (trainerSrc->getCustomer(id) !=
                nullptr && trainerDst->getCapacity() > 0)){
            trainerDst->addCustomer(trainerSrc->getCustomer(id));
            std::vector<OrderPair>& tempSrc = trainerSrc->getOrders();
            std::vector<OrderPair>& tempDst = trainerDst->getOrders();
            for (size_t i = 0; i < tempSrc.size() ; i++){
                if (tempSrc[i].first == id) {
                    tempDst.push_back(tempSrc[i]);

                    trainerSrc->setSalary(trainerSrc->getSalary() - tempSrc[i].second.getPrice());
                    trainerDst->setSalary(trainerDst->getSalary() + tempSrc[i].second.getPrice());
                }
            }
            trainerSrc->removeCustomer(id);
            if (trainerSrc->getCustomers().empty()) trainerSrc->closeTrainer();
        }
        else {
            std::cout << "Cannot move customer" << std::endl;
            error("Cannot move customer");
        }
    }

    std::string MoveCustomer::toString() const {
        std::string msg("move " + std::to_string(srcTrainer) + " " + std::to_string(dstTrainer) + " " + std::to_string(id) + " ");
        if (getStatus() == COMPLETED){
            msg.append("Completed");
        } else if (getStatus() == ERROR){
            msg.append("Error: " + getErrorMsg());
        }
        return msg;
    }

    BaseAction *MoveCustomer::clone() {
        return (new MoveCustomer(*this));
    }
    Close::Close(int id) : BaseAction(), trainerId(id){}

    void Close::act(Studio &studio) {
        Trainer *trainer = studio.getTrainer(trainerId);
        if (trainer == nullptr || !trainer->isOpen()) {
        std::cout << "Trainer does not exist or is not open\n";
            error("Trainer does not exist or is not open");
    }
        else {
            std::cout << "Trainer "  << trainerId <<  " closed. Salary " << trainer->getSalary() << "NIS\n";
            trainer->closeTrainer();
        }
    }

    std::string Close::toString() const {
        std::string msg("close " + std::to_string(trainerId)+ " ");
        if (getStatus() == ActionStatus::COMPLETED){
            msg.append("Completed");
        }
        else if(getStatus() == ActionStatus::ERROR){
            msg.append("Error: " + getErrorMsg());
        }
        return msg;
    }

    BaseAction *Close::clone() {
        return (new Close(*this));
    }


    CloseAll::CloseAll() : BaseAction(){}

    void CloseAll::act(Studio &studio) {
        for (int i = 0; i < studio.getNumOfTrainers(); ++i) {
            Trainer* trainer = studio.getTrainer(i);
            if (trainer != nullptr && trainer->isOpen()){
                std::cout << "Trainer "  << i <<  " closed. Salary " << trainer->getSalary() << "NIS\n";
                trainer->closeTrainer();
            }
        }
    }

    std::string CloseAll::toString() const {
        return "closeall";
    }

    BaseAction *CloseAll::clone() {
        return (new CloseAll(*this));
    }

    PrintWorkoutOptions::PrintWorkoutOptions() :BaseAction() {}

    void PrintWorkoutOptions::act(Studio &studio) {
        std::vector<Workout>& temp = studio.getWorkoutOptions();
        for (unsigned int i = 0; i < temp.size(); i++){
            std::cout << temp[i].getName() + ", " + typeToString(temp[i].getType())  + ", " <<temp[i].getPrice()<< +"\n";
        }
        complete();
    }

    std::string PrintWorkoutOptions::toString() const {
        return "workout_options Completed";
    }

    BaseAction *PrintWorkoutOptions::clone() {
        return (new PrintWorkoutOptions(*this));
    }


    PrintTrainerStatus::PrintTrainerStatus(int id) : BaseAction() ,trainerId(id){}

    void PrintTrainerStatus::act(Studio &studio) {
        Trainer *trainer = studio.getTrainer(trainerId);
        std::vector<Customer *> &CustList = trainer->getCustomers();
        std::vector<OrderPair> &OrdeList = trainer->getOrders();
        if (!trainer->isOpen()) {
            std::cout << "Trainer " + std::to_string(trainerId) + " status: closed\n";
        } else {
            std::cout << "Trainer " + std::to_string(trainerId) + " status: open \n";

            std::cout << "Customers: " << std::endl;
            for (auto &j: CustList) {
                std::cout << std::to_string(j->getId()) + " " + j->getName() + "\n";
            }
            std::cout << "Orders: \n";
            for(auto &j : OrdeList){
                std::cout << j.second.getName() << " " << j.second.getPrice() << "NIS " << j.first << " \n";
            }
            std::cout << "Current Trainer's Salary: " << trainer->getSalary() << +"\n";
        }
        complete();
    }

    std::string PrintTrainerStatus::toString() const {
      return "status " + std::to_string(trainerId) + " Completed";
    }

    BaseAction *PrintTrainerStatus::clone() {
        return (new PrintTrainerStatus(*this));
    }

    PrintActionsLog::PrintActionsLog() : BaseAction() {}

    void PrintActionsLog::act(Studio &studio) {
        std::vector<BaseAction*> actLog = studio.getActionsLog();
        for (auto j: actLog) {
            std::cout << j->toString() << "\n";
        }
        complete();
    }

    std::string PrintActionsLog::toString() const {
        return "log Completed";
    }

    BaseAction *PrintActionsLog::clone() {
        return (new PrintActionsLog(*this));
    }


    BackupStudio::BackupStudio() : BaseAction() {}

    void BackupStudio::act(Studio &studio) {
        if (backup != nullptr){
            delete backup;
            backup = nullptr;
            backup = new Studio(studio);
        }
        else
            backup = new Studio(studio);
        complete();
    }

    std::string BackupStudio::toString() const {
        return "backup Completed";
    }

    BaseAction *BackupStudio::clone() {
        return (new BackupStudio(*this));
    }


    RestoreStudio::RestoreStudio() : BaseAction() {}

    void RestoreStudio::act(Studio &studio) {
        if (backup != nullptr){
            studio = *backup;
            complete();
        }
        else{
            std::cout << "No backup available\n";
            error("No backup available");
        }
    }

    std::string RestoreStudio::toString() const {
        std::string msg("restore");
        if (getStatus() == ActionStatus::COMPLETED)
            msg.append("Completed");
        else if(getStatus() == ActionStatus::ERROR)
            msg.append(" Error: " + getErrorMsg());
        return msg;
    }

    BaseAction *RestoreStudio::clone() {
        return (new RestoreStudio(*this));
    }

