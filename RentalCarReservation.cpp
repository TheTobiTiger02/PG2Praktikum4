//
// Created by GBT B450M-S2H on 14.04.2023.
//


#include "RentalCarReservation.h"
#include <iomanip>

using namespace std;

RentalCarReservation::RentalCarReservation(std::string id, double price, std::string fromDate, std::string toDate,
                                           long travelId,
                                           std::string _pickupLocation, std::string _returnLocation,
                                           std::string _company, std::string _vehicleClass,
                                           std::string pickupCoordinates, std::string returnCoordinates): Booking(id, price,fromDate, toDate,travelId),
                                           pickupLocation(_pickupLocation), returnLocation(_returnLocation), company(_company), vehicleClass(_vehicleClass),
                                           pickupCoordinates(pickupCoordinates), returnCoordinates(returnCoordinates){
}

RentalCarReservation::~RentalCarReservation() {
    std::cout << "RentalCarReservation object destroyed" << std::endl;
}

string RentalCarReservation::showDetails() {
    std::stringstream ss;
    ss << std::fixed << std::setprecision(2) << price;

    return "Mietwagenreservierung mit " + company + ". Abholung am " + fromDate + " in " + pickupLocation +
           ". Rueckgabe am " + toDate + " in " + returnLocation + " mit der Fahrzeugklasse " + vehicleClass +
           ". Preis: " + ss.str() + " Euro";
}


const string &RentalCarReservation::getPickupLocation() const {
    return pickupLocation;
}

const string &RentalCarReservation::getReturnLocation() const {
    return returnLocation;
}

const string &RentalCarReservation::getCompany() const {
    return company;
}

const string &RentalCarReservation::getVehicleClass() const {
    return vehicleClass;
}

void RentalCarReservation::setPickupLocation(const string &pickupLocation) {
    RentalCarReservation::pickupLocation = pickupLocation;
}

void RentalCarReservation::setReturnLocation(const string &returnLocation) {
    RentalCarReservation::returnLocation = returnLocation;
}

void RentalCarReservation::setCompany(const string &company) {
    RentalCarReservation::company = company;
}

void RentalCarReservation::setVehicleClass(const string &vehicleClass) {
    RentalCarReservation::vehicleClass = vehicleClass;
}

const string &RentalCarReservation::getPickupCoordinates() const {
    return pickupCoordinates;
}

const string &RentalCarReservation::getReturnCoordinates() const {
    return returnCoordinates;
}
