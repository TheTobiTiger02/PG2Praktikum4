//
// Created by TheTo on 14.06.2023.
//

#include "SortFunktor.h"

SortFunktor::SortFunktor(SortFunktor::SortParameter parameter) : sortParameter(parameter) {

}

void SortFunktor::setSortParameter(SortFunktor::SortParameter parameter) {
    sortParameter = parameter;
}

bool SortFunktor::operator()(std::shared_ptr<Booking> booking1, std::shared_ptr<Booking> booking2){
    if(sortParameter == PRICE){
        return booking1->getPrice() < booking2->getPrice();
    }
    else if(sortParameter == FROMDATE){
        return booking1->getFromDate() < booking2->getFromDate();
    }
    else if(sortParameter == TODATE){
        return booking1->getToDate() < booking2->getToDate();
    }
    else if(sortParameter == TRAVELID){
        return booking1->getTravelId() < booking2->getTravelId();
    }
    else{
        return false;
    }
}


