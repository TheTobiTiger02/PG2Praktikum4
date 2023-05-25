//
// Created by GBT B450M-S2H on 14.04.2023.
//

#include "HotelBooking.h"
#include <iomanip>

using namespace std;

HotelBooking::HotelBooking(std::string id, double price, std::string fromDate, std::string toDate, long travelId,
                           std::string _hotel,
                           std::string _town, std::string _roomType, std::string hotelCoordinates) : Booking(id, price, fromDate, toDate, travelId),
                                                                       hotel(_hotel), town(_town), roomType(_roomType), hotelCoordinates(hotelCoordinates) {
    roomTypes["EZ"] = "Einzelzimmer";
    roomTypes["DZ"] = "Doppelzimmer";
    roomTypes["AP"] = "Appartment";
    roomTypes["SU"] = "Suite";

    roomType = roomTypes[roomType];

}

HotelBooking::~HotelBooking() {
    std::cout << "HotelBooking object destroyed" << std::endl;
}

string HotelBooking::showDetails() {
    std::stringstream ss;
    ss << std::fixed << std::setprecision(2) << price;

    return "Hotelreservierung im " + hotel + " in " + town + " vom " + fromDate +
           " bis zum " + toDate + " mit " + roomType + ". Preis: " + ss.str() + " Euro";
}

const string &HotelBooking::getHotel() const {
    return hotel;
}

const string &HotelBooking::getTown() const {
    return town;
}

const string &HotelBooking::getRoomType() const {
    return roomType;
}

void HotelBooking::setHotel(const string &hotel) {
    HotelBooking::hotel = hotel;
}

void HotelBooking::setTown(const string &town) {
    HotelBooking::town = town;
}

void HotelBooking::setRoomType(const string &roomType) {
    HotelBooking::roomType = roomType;
}
