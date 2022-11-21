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
    virtual void act(Studio& studio) = 0;
    virtual std::string toString() const = 0;
    virtual BaseAction* clone() = 0;
    virtual ~BaseAction() = default;
protected:
    void complete();
    void error(std::string errorMsg);
    std::string getErrorMsg() const;
    std::string typeToString(int i);
private:
    std::string errorMsg;
    ActionStatus status;
};


class OpenTrainer : public BaseAction {
public:
    OpenTrainer(int id, std::vector<Customer *> &customersList);
    virtual ~OpenTrainer();
    BaseAction* clone();
    void act(Studio &studio);
    std::string toString() const;
private:
    const int trainerId;
    std::vector<Customer *> customers;
    std::string msg;
};


class Order : public BaseAction {
public:
    Order(int id);
    BaseAction* clone();
    void act(Studio &studio);
    std::string toString() const;
private:
    const int trainerId;
};


class MoveCustomer : public BaseAction {
public:
    MoveCustomer(int src, int dst, int customerId);
    BaseAction* clone();
    void act(Studio &studio);
    std::string toString() const;
private:
    const int srcTrainer;
    const int dstTrainer;
    const int id;
};


class Close : public BaseAction {
public:
    Close(int id);
    BaseAction* clone();
    void act(Studio &studio);
    std::string toString() const;
private:
    const int trainerId;
};


class CloseAll : public BaseAction {
public:
    CloseAll();
    BaseAction* clone();
    void act(Studio &studio);
    std::string toString() const;
private:
};


class PrintWorkoutOptions : public BaseAction {
public:
    PrintWorkoutOptions();
    BaseAction* clone();
    void act(Studio &studio);
    std::string toString() const;
private:
};


class PrintTrainerStatus : public BaseAction {
public:
    PrintTrainerStatus(int id);
    BaseAction* clone();
    void act(Studio &studio);
    std::string toString() const;
private:
    const int trainerId;
};


class PrintActionsLog : public BaseAction {
public:
    PrintActionsLog();
    BaseAction* clone();
    void act(Studio &studio);
    std::string toString() const;
private:
};


class BackupStudio : public BaseAction {
public:
    BackupStudio();
    BaseAction* clone();
    void act(Studio &studio);
    std::string toString() const;
private:
};


class RestoreStudio : public BaseAction {
public:
    RestoreStudio();
    BaseAction* clone();
    void act(Studio &studio);
    std::string toString() const;

};


#endif