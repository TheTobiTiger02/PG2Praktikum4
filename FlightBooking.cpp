//
// Created by GBT B450M-S2H on 14.04.2023.
//

#include "FlightBooking.h"
#include <iomanip>

using namespace std;

FlightBooking::FlightBooking(std::string id, double price, std::string fromDate, std::string toDate, long travelId,
                             std::string _fromDestination, std::string _toDestination, std::string _airline,
                             std::string bookingClass, std::string fromDestCoordinates, std::string toDestCoordinates) : Booking(id, price, fromDate, toDate, travelId),
                                                 fromDestination(_fromDestination), toDestination(_toDestination),
                                                 airline(_airline), bookingClass(bookingClass), fromDestCoordinates(fromDestCoordinates), toDestCoordinates(toDestCoordinates) {


}

FlightBooking::~FlightBooking() {
    std::cout << "FlightBooking object destroyed" << std::endl;
}

string FlightBooking::showDetails() {
    std::stringstream ss;
    ss << std::fixed << std::setprecision(2) << price;

    return "Flugbuchung von " + fromDestination + " nach " + toDestination + " mit " + airline +
           " am " + fromDate + " mit der Buchungsklasse " + bookingClass + ". Preis: " + ss.str() + " Euro";

}

const string &FlightBooking::getFromDestination() const {
    return fromDestination;
}

const string &FlightBooking::getToDestination() const {
    return toDestination;
}

const string &FlightBooking::getAirline() const {
    return airline;
}

std::string FlightBooking::getBookingClass() const {
    return bookingClass;
}

void FlightBooking::setFromDestination(const string &fromDestination) {
    FlightBooking::fromDestination = fromDestination;
}

void FlightBooking::setToDestination(const string &toDestination) {
    FlightBooking::toDestination = toDestination;
}

void FlightBooking::setAirline(const string &airline) {
    FlightBooking::airline = airline;
}

void FlightBooking::setBookingClass(const string &bookingClass) {
    FlightBooking::bookingClass = bookingClass;
}

const string &FlightBooking::getFromDestCoordinates() const {
    return fromDestCoordinates;
}

const string &FlightBooking::getToDestCoordinates() const {
    return toDestCoordinates;
}




