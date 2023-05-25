//
// Created by GBT B450M-S2H on 07.05.2023.
//

#include "Customer.h"

Customer::Customer(long _id, std::string _name) : id(_id), name(_name) {

}

void Customer::addTravel(Travel *travel) {
    travelList.push_back(travel);
}

long Customer::getId() const {
    return id;
}

const std::string &Customer::getName() const {
    return name;
}

const std::vector<Travel *> &Customer::getTravelList() const {
    return travelList;
}
