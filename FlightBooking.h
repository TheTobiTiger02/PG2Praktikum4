//
// Created by GBT B450M-S2H on 14.04.2023.
//

#ifndef PG2PRAKTIKUM1_FLIGHTBOOKING_H
#define PG2PRAKTIKUM1_FLIGHTBOOKING_H

#include <iostream>
#include "Booking.h"
#include <map>
#include "Airport.h"

class FlightBooking : public Booking {

private:
    std::string fromDestination;
    std::string toDestination;
    std::string airline;
    std::string bookingClass;
    std::string fromDestCoordinates;
    std::string toDestCoordinates;
    std::map<char, std::string> bookingClasses;


public:
    FlightBooking(std::string id, double price, std::string fromDate, std::string toDate, long travelId,
                  std::string _fromDestination, std::string _toDestination, std::string _airline, char bookingChar,
                  std::string fromDestCoordinates, std::string toDestCoordinates);

    ~FlightBooking();

    std::string showDetails();

    void setFromDestination(const std::string &fromDestination);

    void setToDestination(const std::string &toDestination);

    void setAirline(const std::string &airline);

    void setBookingClass(const std::string &bookingClass);

    const std::string &getFromDestination() const;

    const std::string &getToDestination() const;

    const std::string &getAirline() const;

    std::string getBookingClass() const;

};


#endif //PG2PRAKTIKUM1_FLIGHTBOOKING_H
