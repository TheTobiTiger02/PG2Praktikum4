//
// Created by GBT B450M-S2H on 07.05.2023.
//



#ifndef PG2PRAKTIKUM3_TRAVEL_H
#define PG2PRAKTIKUM3_TRAVEL_H

#include <vector>
#include <QDate>
#include "Booking.h"


class Travel {

private:
    long id;
    long customerId;
    std::vector<Booking *> travelBookings;

public:
    Travel(long _id, long _customerId);

    void addBooking(Booking *booking);

    long getId() const;

    long getCustomerId() const;

    const std::vector<Booking *> &getTravelBookings() const;

    QDate getStartDate();

    QDate getEndDate();

};


#endif //PG2PRAKTIKUM3_TRAVEL_H
