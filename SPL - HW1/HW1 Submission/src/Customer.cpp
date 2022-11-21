#include "../include/Customer.h"
#include <utility>
#include <vector>
#include <string>
#include "../include/Workout.h"
#include "algorithm"


    // Base Customer Class
    Customer::Customer(std::string c_name, int c_id) : name(std::move(c_name)), id(c_id) {} // Constructor

    std::string Customer::getName() const {return name;}

    int Customer::getId() const {return id;}

    // End Base Customer Class


    // SweatyCustomer Class
    SweatyCustomer::SweatyCustomer(std::string name, int id) : Customer(std::move(name), id) {} // Constructor

    std::vector<int> SweatyCustomer::order(const std::vector<Workout> &workout_options){
        std::vector<int> SweatyWorkouts;
        for (auto & temp: workout_options) {
            if (temp.getType() == CARDIO){
                SweatyWorkouts.push_back(temp.getId());
            }
        }
        return SweatyWorkouts;
}


    std::string SweatyCustomer::toString() const {
        return getName() + " is doing "  ;
}

    Customer *SweatyCustomer::clone() {
    return new SweatyCustomer(*this);
}

   std::string SweatyCustomer::getType() {
       return "swt";
   };
   // End of SweatyCustomer

   // Cheap Customer

   CheapCustomer::CheapCustomer(std::string name, int id) : Customer(std::move(name),id){}


    std::vector<int> CheapCustomer::order(const std::vector<Workout> &workout_options) {
        std::vector<int> CheapWorkouts;
        int cheapestId = -1;
        int price = INT16_MAX;
        for (auto &temp: workout_options) {
            if (temp.getType() == CARDIO) {
                if (temp.getPrice() < price){
                    price = temp.getPrice();
                    cheapestId = temp.getId();
                }
         }
     }
    if (cheapestId != -1)
        CheapWorkouts.push_back(cheapestId);
    return CheapWorkouts;
}

    std::string CheapCustomer::toString() const {
        return getName() + " is doing " ;
}

     Customer *CheapCustomer::clone() {
         return new CheapCustomer(*this);
}
   std::string CheapCustomer::getType() {
       return "chp";
   }
    HeavyMuscleCustomer::HeavyMuscleCustomer(std::string name, int id) : Customer(std::move(name), id){
   }


    std::vector<int> HeavyMuscleCustomer::order(const std::vector<Workout> &workout_options) {
        std::vector<int> HeavyWorkouts;
        std::vector<Workout*> AnaerobicWorkOuts;
        for (unsigned int i = 0; i < workout_options.size(); i++) {
            if (workout_options[i].getType() == ANAEROBIC){
                AnaerobicWorkOuts.push_back( new Workout(workout_options[i]));
            }
        }
        int currIndex;
        int currPrice;
        while (!AnaerobicWorkOuts.empty()){
            currIndex = 0;
            currPrice = AnaerobicWorkOuts[0]->getPrice();
            for (unsigned int i = 1; i < AnaerobicWorkOuts.size(); ++i) {
                if (AnaerobicWorkOuts[i]->getPrice() > currPrice) {
                    currPrice = AnaerobicWorkOuts[i]->getPrice();
                    currIndex = i;
                }
            }
            HeavyWorkouts.push_back(AnaerobicWorkOuts[currIndex]->getId());\
            delete AnaerobicWorkOuts[currIndex];
            AnaerobicWorkOuts.erase(AnaerobicWorkOuts.begin() + currIndex);
        }
        AnaerobicWorkOuts.clear();
        return HeavyWorkouts;
    }
    std::string HeavyMuscleCustomer::toString() const {
        return getName() + " is doing " ;
}
   Customer *HeavyMuscleCustomer::clone() {
       return new HeavyMuscleCustomer(*this);
}


   std::string HeavyMuscleCustomer::getType() {
       return "mcl";
   }

   FullBodyCustomer::FullBodyCustomer(std::string name, int id) : Customer(std::move(name), id) {
}


    std::vector<int> FullBodyCustomer::order(const std::vector<Workout> &workout_options){
    std::vector<int> fullBodyWorkoutOptions;
    int cheapCardio = INT16_MAX;
    int expnMix = INT16_MIN;
    int cheapAna = INT16_MAX;
    int CardioID = -1;
    int MixID = -1;
    int AnaID = -1;
    for (const auto & j : workout_options){
        if (j.getType() == CARDIO){
            if (j.getPrice() < cheapCardio) {
                CardioID = j.getId();
                cheapCardio = j.getPrice();
            }
        }
        else if (j.getType() == MIXED){
            if (j.getPrice() > expnMix) {
                MixID = j.getId();
                expnMix = j.getPrice();
            }
        } else
                if (j.getPrice() < cheapAna){
                    AnaID = j.getId();
                    cheapAna = j.getPrice();
                }
    }
    fullBodyWorkoutOptions.push_back(CardioID);
    fullBodyWorkoutOptions.push_back(MixID);
    fullBodyWorkoutOptions.push_back(AnaID);

    return fullBodyWorkoutOptions;
}


  std::string FullBodyCustomer::toString() const {
      return getName() + " is doing "  ;
}

   Customer* FullBodyCustomer::clone() {
       return new FullBodyCustomer(*this);
}
   std::string FullBodyCustomer::getType() {
       return "fbd";
   }









