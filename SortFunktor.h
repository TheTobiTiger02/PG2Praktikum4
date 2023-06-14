//
// Created by TheTo on 14.06.2023.
//

#ifndef PG2PRAKTIKUM4_SORTFUNKTOR_H
#define PG2PRAKTIKUM4_SORTFUNKTOR_H
#include "Booking.h"


class SortFunktor {
public:
    enum SortParameter{
        PRICE = 0, FROMDATE = 1, TODATE = 2, TRAVELID = 3
    };
    SortFunktor(SortFunktor::SortParameter parameter = TRAVELID);
    bool operator()(std::shared_ptr<Booking> booking1, std::shared_ptr<Booking> booking2);
    void setSortParameter(SortFunktor::SortParameter parameter);

private:
    SortFunktor::SortParameter sortParameter = TRAVELID;
};


#endif //PG2PRAKTIKUM4_SORTFUNKTOR_H
