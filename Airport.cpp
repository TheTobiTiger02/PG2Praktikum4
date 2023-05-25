//
// Created by GBT B450M-S2H on 25.05.2023.
//

#include "Airport.h"

Airport::Airport(std::string name, std::string isoCountry, std::string municipality) : name(name), isoCountry(isoCountry), municipality(municipality){
}

const std::string &Airport::getName() const {
    return name;
}

const std::string &Airport::getIsoCountry() const {
    return isoCountry;
}

const std::string &Airport::getMunicipality() const {
    return municipality;
}

