//
// Created by GBT B450M-S2H on 25.05.2023.
//

#ifndef PG2PRAKTIKUM4_AIRPORT_H
#define PG2PRAKTIKUM4_AIRPORT_H
#include <string>

class Airport {
private:
    std::string name;
    std::string isoCountry;
    std::string municipality;

public:
    Airport(std::string name, std::string isoCountry, std::string municipality);

    const std::string &getName() const;

    const std::string &getIsoCountry() const;

    const std::string &getMunicipality() const;
};


#endif //PG2PRAKTIKUM4_AIRPORT_H
