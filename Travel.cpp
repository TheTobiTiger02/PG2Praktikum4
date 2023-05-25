//
// Created by GBT B450M-S2H on 07.05.2023.
//

#include "Travel.h"

Travel::Travel(long _id, long _customerId) : id(_id), customerId(_customerId) {

}

void Travel::addBooking(Booking *booking) {
    travelBookings.push_back(booking);
}

long Travel::getId() const {
    return id;
}

long Travel::getCustomerId() const {
    return customerId;
}

const std::vector<Booking *> &Travel::getTravelBookings() const {
    return travelBookings;
}

QDate Travel::getStartDate() {
    QDate startDate = travelBookings.at(0)->getFromDate();
    for (auto b: travelBookings) {
        QDate newDate = b->getFromDate();
        if (newDate < startDate) {
            startDate = newDate;
        }
    }
    return startDate;
}

QDate Travel::getEndDate() {
    QDate endDate = travelBookings.at(0)->getToDate();
    for (auto b: travelBookings) {
        QDate newDate = b->getToDate();
        if (newDate > endDate) {
            endDate = newDate;
        }
    }
    return endDate;
}
