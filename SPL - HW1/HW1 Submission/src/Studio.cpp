#include "../include/Studio.h"
#include <vector>
#include <string>
#include "../include/Workout.h"
#include "../include/Trainer.h"
#include "../include/Action.h"
#include <iostream>
#include <fstream>
#include <sstream>

   Studio::Studio() :  open(false), trainers(), workout_options(),actionsLog(),newMsg(),nextCustomerId(0) {}; // Constructor

   Studio::Studio(const std::string &configFilePath) : open(false),trainers(std::vector<Trainer*>()),workout_options(std::vector<Workout>()), actionsLog(std::vector<BaseAction*>()) ,newMsg(), nextCustomerId(0){
       std::ifstream file(configFilePath);

       std::string line;
       std::string temp;
       int count = 0;
       int ids = 0;

       if (file.is_open()) {

           while (std::getline(file, line)) {

               if (line[0] == '#')
                   continue;
               if (line.empty())
                   continue;
               std::stringstream curr_line(line);
           if (count == 0){
               int numOfTrainers = std::stoi(line);
               trainers.reserve(numOfTrainers);
               count++;
           } else
               if (count == 1) {
                   while (curr_line.good()) {
                       std::getline(curr_line, temp, ',');
                       int capacity = std::stoi(temp);
                       Trainer *trainer = new Trainer(capacity);
                       trainers.push_back(trainer);
                   }
                   count++;
               } else {
                   WorkoutType type;
                   std::string workoutName;
                   int workoutPrice;
                   int wordCount = 0;
                   while (curr_line.good()) {
                       std::getline(curr_line, temp, ',');
                       if (temp[0] == ' ')
                           temp = temp.substr(1, temp.length() - 1);
                       if (wordCount == 0) {
                           workoutName = temp;
                           wordCount++;
                       } else if (wordCount == 1) {
                           type = getType(temp);
                           wordCount++;
                       } else {
                           workoutPrice = std::stoi(temp);
                       }
                   }
                       Workout newWorkout = Workout(ids, workoutName, workoutPrice, type);
                       workout_options.push_back(newWorkout);
                       count++;
                       ids++;
                   }
               }
           }

         file.close();
//
   }
   Studio::Studio(const Studio &other) : open(other.open), trainers(), workout_options(other.workout_options), actionsLog(),newMsg(other.newMsg), nextCustomerId(other.nextCustomerId) {
       for (auto trainer : other.trainers){
           Trainer* tempTrainer = new Trainer(*trainer);
           trainers.push_back(tempTrainer);
       }
       for (auto i : other.actionsLog){
           actionsLog.push_back(i ->clone());
       }
   }

   Studio::~Studio() {
       for (auto &Trainer : trainers) {
           delete Trainer;
       }
       for (auto &j: actionsLog) {
           delete j;
       }
       trainers.clear();
       actionsLog.clear();
       workout_options.clear();
   }

   Studio::Studio(Studio &&other) : open(other.open),trainers(std::move(other.trainers)),workout_options(std::move(other.workout_options)),
   actionsLog(std::move(other.actionsLog)),newMsg(other.newMsg), nextCustomerId(other.nextCustomerId){
       other.trainers.clear();
       other.workout_options.clear();
       other.actionsLog.clear();
   }

   Studio &Studio::operator=(const Studio &other) {
       if (this == &other)
           return *this;
       open = other.open;

       for (auto &trainer : trainers){
           delete trainer;
       }
       trainers.clear();
       for (auto trainer : other.trainers){
           auto *newTrainer = new Trainer(*trainer);
           trainers.push_back(newTrainer);
       }

       for (auto &j : actionsLog){
           delete j;
       }
       actionsLog.clear();

       for (auto i : other.actionsLog){
           actionsLog.push_back(i->clone());
       }

       workout_options.clear();
       for (const auto &k : other.workout_options){
           workout_options.push_back(k);
       }
       return *this;
   }

   Studio &Studio::operator=(Studio &&other) {
       if (this == &other)
           return *this;
       open = other.open;
       workout_options = std::move(other.workout_options);
       for (auto &trainer : trainers){
           delete trainer;
       }
       trainers.clear();
       trainers = other.trainers;

       for (auto &k : actionsLog){
           delete k;
       }
       actionsLog.clear();
       actionsLog = other.actionsLog;
       return *this;
   }

   void Studio::start() {
       std::cout << "Studio is now open!\n";
       open = true;
//       std::string temp;
       std::string delimiter = " ";
       size_t pos = 0;
       std::string token;
       while(open){
           std::string action;
           std::getline(std::cin, action);
           action += " ";
           pos = action.find(delimiter);
           token = action.substr(0, pos);
           action.erase(0, pos + delimiter.length());
           if (token == "open"){
               goOpen(action);
           }
           else if (token == "order"){
               int trainerId = std::stoi(action.substr(0,1));
               Order *ord = new Order(trainerId);
               ord->act(*this);
               actionsLog.push_back(ord);
           }
           else if (token == "move"){
               int orgId = std::stoi(action.substr(0,1));
               pos = action.find(delimiter);
               token = action.substr(0, pos);
               action.erase(0, pos + delimiter.length());
               int dstId = std::stoi(action.substr(0,1));
               pos = action.find(delimiter);
               token = action.substr(0, pos);
               action.erase(0, pos + delimiter.length());
               int customerId = std::stoi(action.substr(0,1));
               MoveCustomer *move = new MoveCustomer(orgId, dstId, customerId);
               move->act(*this);
               actionsLog.push_back(move);
           }
           else if (token == "close"){
               int trainerId = std::stoi(action.substr(0,1));
               Close *close = new Close(trainerId);
               close->act(*this);
               actionsLog.push_back(close);
           }
           else if (token == "closeAll"){
               CloseAll * newCloseAll = new CloseAll();
               newCloseAll->act(*this);
               actionsLog.push_back(newCloseAll);
               open = false;
           }
           else if (token == "workout_options"){
               PrintWorkoutOptions *print = new PrintWorkoutOptions();
               print->act(*this);
               actionsLog.push_back(print);
           }
           else if (token == "status"){
               int trainerId = std::stoi(action.substr(0,1));
               PrintTrainerStatus *print = new PrintTrainerStatus(trainerId);
               print->act(*this);
               actionsLog.push_back(print);
           }
           else if (token == "log"){
               PrintActionsLog *print = new PrintActionsLog();
               print->act(*this);
               actionsLog.push_back(print);
           }
           else if (token == "backup"){
               BackupStudio *backup = new BackupStudio();
               backup->act(*this);
               actionsLog.push_back(backup);
           }
           else if (token == "restore"){
               RestoreStudio *restore = new RestoreStudio();
               restore->act(*this);
               actionsLog.push_back(restore);
           }
       }
   }

   int Studio::getNumOfTrainers() const {
       return trainers.size();
   }

   Trainer* Studio::getTrainer(int tid) {
       unsigned int i = tid;
       if (i > trainers.size()) return nullptr;
       else return trainers[tid];
   }

   const std::vector<BaseAction*>& Studio::getActionsLog() const {
       return actionsLog;
   }

   std::vector<Workout>& Studio::getWorkoutOptions(){
       return workout_options;
   }

   WorkoutType Studio::getType(std::string type) {
       if (type == "Cardio")
           return CARDIO;
       else if (type == "Mixed")
           return MIXED;
       else return ANAEROBIC;
   }

    void Studio::goOpen(std::string details)
    {

    std::vector<Customer*> customers;
    int trainerId;
    std::string delimiterSpace = " ";
    std::string delimiterComa = ",";
    int pos1 = 0;
    std::string token1;  /// customer name
    std::string token2; /// customer type
    pos1 = details.find(delimiterSpace);
    token1 = details.substr(0,pos1);
    trainerId = std::stoi(token1);//get the id
    details.erase(0, pos1 + delimiterSpace.length());

    //get the customers
    pos1 = 0;
    int numOfCus = 0;
    while(getTrainer(trainerId) != nullptr && !details.empty() && numOfCus < getTrainer(trainerId)->getCapacity())
    {
        pos1 = details.find(delimiterComa);
        token1 = details.substr(0, pos1);
        details.erase(0, pos1 + delimiterComa.length());
        pos1 = details.find(delimiterSpace);
        token2 = details.substr(0, pos1);
        details.erase(0, pos1 + delimiterSpace.length());
        if(!token1.empty() && !token2.empty() && !trainers.at((trainerId))->isOpen())
        {
            Customer *newCustomer = createCustomerByType(token1, token2);
            customers.push_back(newCustomer);
            nextCustomerId++;
            token1="";
            token2="";
            numOfCus++;
        }
    }
    OpenTrainer *openTrainer = new OpenTrainer((trainerId), customers);
    openTrainer->act(*this);
    actionsLog.push_back(openTrainer);
}

    Customer* Studio::createCustomerByType(std::string name, std::string strategy)
    {
    if (strategy == "swt")
        return new SweatyCustomer(name, nextCustomerId);
    if (strategy == "chp")
        return new CheapCustomer(name, nextCustomerId);
    if (strategy == "mcl")
        return new HeavyMuscleCustomer(name, nextCustomerId);
    else
        return new FullBodyCustomer(name, nextCustomerId);
}