//
// Created by GBT B450M-S2H on 14.04.2023.
//

#ifndef PG2PRAKTIKUM1_TRAVELAGENCY_H
#define PG2PRAKTIKUM1_TRAVELAGENCY_H

#include <iostream>
#include <vector>
#include <Qt>
#include <QMessageBox>
#include "RentalCarReservation.h"
#include "HotelBooking.h"
#include "FlightBooking.h"
#include "Customer.h"
#include "Travel.h"
#include "Airport.h"

class TravelAgency {
private:
    //QMessageBox* msgBox;
    std::vector<std::shared_ptr <Booking>> allBookings;
    std::vector<std::shared_ptr <Customer>> allCustomers;
    std::vector<std::shared_ptr <Travel>> allTravels;
    std::map<std::string, std::shared_ptr<Airport>> iataCodes;


public:
    ~TravelAgency();

    void readFile(std::string filePath, int startRow = 0, int flightCount = 0, int hotelCount = 0, int carCount = 0,
                  int travelCount = 0, int customerCount = 0, double totalPrice = 0);
    bool readIataCodes(std::string filePath);

    std::shared_ptr<Booking> findBooking(std::string id);

    std::shared_ptr<Travel> findTravel(long id);

    std::shared_ptr<Customer> findCustomer(long id);

    int getTravelCount(long customerId);

    int getBookingCount(long travelId);

    std::shared_ptr<Airport> getAirport(std::string iataCode);

    const std::vector<std::shared_ptr<Booking >> &getAllBookings() const;

    const std::vector<std::shared_ptr<Customer >> &getAllCustomers() const;

    const std::vector<std::shared_ptr<Travel >> &getAllTravels() const;

    const std::vector<std::shared_ptr<Booking >> &getBookings() const;



private slots:

    void changeInformativeText();

};

#endif //PG2PRAKTIKUM1_TRAVELAGENCY_H
