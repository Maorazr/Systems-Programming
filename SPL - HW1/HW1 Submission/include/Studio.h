#ifndef STUDIO_H_
#define STUDIO_H_

#include <vector>
#include <string>
#include "Workout.h"
#include "Trainer.h"
#include "Action.h"


class Studio{
public:
    Studio();
    Studio(const std::string &configFilePath);
    Studio(const Studio &other);
    virtual ~Studio();
    Studio(Studio &&other);
    Studio& operator=(const Studio &other);
    Studio& operator=(Studio &&other);
    void start();
    int getNumOfTrainers() const;
    Trainer* getTrainer(int tid);
    const std::vector<BaseAction*>& getActionsLog() const; // Return a reference to the history of actions
    std::vector<Workout>& getWorkoutOptions();

private:
    Customer* createCustomerByType(std::string name, std::string strategy);
    bool open;
    std::vector<Trainer*> trainers;
    std::vector<Workout> workout_options;
    std::vector<BaseAction*> actionsLog;
    WorkoutType getType(std::string type);
    void goOpen(std::string details);
    std::string newMsg;
    int nextCustomerId;

};

#endif