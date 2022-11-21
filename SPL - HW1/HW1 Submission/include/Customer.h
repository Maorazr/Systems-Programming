#ifndef CUSTOMER_H_
#define CUSTOMER_H_

#include <vector>
#include <string>
#include "Workout.h"

class Customer{
public:
    Customer(std::string c_name, int c_id);
    virtual ~Customer() = default;
    virtual std::vector<int> order(const std::vector<Workout> &workout_options)=0;
    virtual std::string toString() const = 0;
    std::string getName() const;
    int getId() const;
    virtual Customer* clone() = 0;
    virtual std::string getType() = 0;
private:
    const std::string name;
    const int id;
};



class SweatyCustomer : public Customer {
public:
    SweatyCustomer(std::string name, int id);
    virtual ~SweatyCustomer() = default;
    std::vector<int> order(const std::vector<Workout> &workout_options);
    std::string toString() const;
    Customer* clone();
    std::string getType();
private:
};


class CheapCustomer : public Customer {
public:
    CheapCustomer(std::string name, int id);
    virtual ~CheapCustomer() = default;
    std::vector<int> order(const std::vector<Workout> &workout_options);
    std::string toString() const;
    Customer* clone();
    std::string getType();
private:
};


class HeavyMuscleCustomer : public Customer {
public:
    HeavyMuscleCustomer(std::string name, int id);
    virtual ~HeavyMuscleCustomer() = default;
    std::vector<int> order(const std::vector<Workout> &workout_options);
    std::string toString() const;
    Customer* clone();
    std::string getType();
private:
};


class FullBodyCustomer : public Customer {
public:
    FullBodyCustomer(std::string name, int id);
    virtual ~FullBodyCustomer() = default;
    std::vector<int> order(const std::vector<Workout> &workout_options);
    std::string toString() const;
    Customer* clone();
    std::string getType();
private:
};


#endif