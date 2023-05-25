//
// Created by GBT B450M-S2H on 14.04.2023.
//

#ifndef PG2PRAKTIKUM1_HOTELBOOKING_H
#define PG2PRAKTIKUM1_HOTELBOOKING_H

#include <iostream>
#include "Booking.h"
#include <map>

class HotelBooking : public Booking {

private:
    std::string hotel;
    std::string town;
    std::string roomType;
    std::string hotelCoordinates;
    std::map<std::string, std::string> roomTypes;

public:
    HotelBooking(std::string id, double price, std::string fromDate, std::string toDate, long travelId,
                 std::string _hotel, std::string _town, std::string _roomType, std::string hotelCoordinates);

    ~HotelBooking();

    std::string showDetails();

    const std::string &getHotel() const;

    const std::string &getTown() const;

    void setHotel(const std::string &hotel);

    void setTown(const std::string &town);

    void setRoomType(const std::string &roomType);

    const std::string &getRoomType() const;
};


#endif //PG2PRAKTIKUM1_HOTELBOOKING_H
