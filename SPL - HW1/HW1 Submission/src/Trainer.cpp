#include "../include/Trainer.h"
#include <vector>
#include <iostream>
#include "../include/Customer.h"
#include "../include/Workout.h"


  Trainer::Trainer(int t_capacity) : capacity(t_capacity), open(false), customersList(), orderList(), salary(){} // Constructor

  Trainer::Trainer(const Trainer &other) : capacity(other.capacity), open(other.open),  // Copy constructor
  customersList(), orderList(other.orderList) , salary(other.salary) {
      for (auto i : other.customersList) {
          customersList.push_back(i-> clone());
      }
}
  Trainer::~Trainer() // destructor
  {
      for (auto &i : customersList){
          delete i;
      }

      customersList.clear();
}

  Trainer &Trainer::operator=(const Trainer &other) // Copy assignment operator
  {
    std::cout << "dafdaf\n";
      if(this == &other)
          return *this;
      capacity = other.capacity;
      open = other.open;
      salary = other.salary;
      for (auto &i : customersList) {
          delete i;
      }
      customersList.clear();
      for (auto j : other.customersList) {
          customersList.push_back(j->clone());
      }
      orderList.clear();
      for (auto k : other.orderList)
          orderList.push_back(k);
      return *this;
}
  Trainer::Trainer(Trainer &&other) : capacity(other.capacity), open(other.open), customersList(std::move(other.customersList)),
  orderList(std::move(other.orderList)), salary(other.salary) {

  }
 Trainer& Trainer::operator=(Trainer &&other){ //////sarsarsar
    if(this == &other)
        return *this;
    capacity = other.capacity;
    open = other.open;
    salary = other.salary;
    for (auto &i : customersList) {
        delete i;
    }
    customersList.clear();
    for (auto j : other.customersList) {
        customersList.push_back(j);
    }
    orderList = std::move(other.orderList);
    return *this;
 }


 int Trainer::getCapacity() const {
    return capacity;
}

  void Trainer::addCustomer(Customer *customer) {
      if (capacity > 0) {
          customersList.push_back(customer);
          capacity--;
      }
}
  void Trainer::removeCustomer(int id) {
      int size = customersList.size();
      for (int i = 0; i < size; i++) {
          if (customersList.at(i)->getId() == id) {
              customersList.erase(customersList.begin() + i);
          }
          std::vector<OrderPair> newPairOrderList;
          for (auto &j: orderList) {
              if (j.first != id) {
                  newPairOrderList.push_back(j);   ///  why
              }
          }
          orderList = std::move(newPairOrderList);
      }
  }

    Customer* Trainer::getCustomer(int id) {
        for (auto &i : customersList) {
            if (i->getId() == id)
                return i;
        }
        return nullptr;
    }

   std::vector<Customer*>& Trainer::getCustomers(){
       return customersList;
}

  std::vector<OrderPair>& Trainer::getOrders(){
      return orderList;
}

     void Trainer::order(const int customer_id, const std::vector<int> workout_ids, const std::vector<Workout>& workout_options){
         for(unsigned int i = 0; i < workout_ids.size(); i++){
            OrderPair temp(customer_id, workout_options[workout_ids[i]]);
            orderList.push_back(temp);
            salary = salary + workout_options[workout_ids[i]].getPrice();
    }
}

  void Trainer::openTrainer(){
    open = true;
}

  void Trainer::closeTrainer(){
      for (auto &i : customersList) {
          delete i;
          capacity++;
      }
      customersList.clear();
      orderList.clear();
      open=false;
      salary = 0;
}

  int Trainer::getSalary(){
      return salary;
}
  void Trainer::setSalary(int s) {
    salary = s;

}

  bool Trainer::isOpen(){
      return open;
}

