#include "../include/Workout.h"
#include <string>
#include <utility>

    Workout::Workout(int w_id, std::string w_name, int w_price, WorkoutType w_type) :
                              id(w_id), name(w_name), price(w_price), type(w_type) {};

    std::string Workout::getName() const {
        return name;
    };

    int Workout::getId() const {
        return id;
    }

    int Workout::getPrice() const {
        return price;
    };

    WorkoutType Workout::getType() const {
        return type;
    };






