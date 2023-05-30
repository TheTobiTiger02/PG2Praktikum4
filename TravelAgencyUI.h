//
// Created by GBT B450M-S2H on 21.04.2023.
//

#ifndef PG2PRAKTIKUM2_MAINWINDOW_H
#define PG2PRAKTIKUM2_MAINWINDOW_H

#include <QMainWindow>
#include <QListWidgetItem>
#include "TravelAgency.h"
#include "ui_TravelAgencyUI.h"


QT_BEGIN_NAMESPACE
namespace Ui { class TravelAgencyUI; }
QT_END_NAMESPACE

class TravelAgencyUI : public QMainWindow {
Q_OBJECT

public:
    TravelAgencyUI(std::shared_ptr<TravelAgency> _travelAgency, QWidget *parent = nullptr);

    ~TravelAgencyUI() override;


    QIcon getBookingIcon(std::shared_ptr<Booking> booking);

    void loadTravels(long customerId);

    void loadBookings(long travelId);

    void loadBookingDetails();

private:

    enum travelsColumnIndices {
        TRAVEL_ID, TRAVEL_BEGIN, TRAVEL_END
    };
    enum bookingsColumnIndices {
        BOOKING_TYPE, BOOKING_START, BOOKING_END, BOOKING_PRICE
    };
    bool readIataCodes;
    std::map<int, std::shared_ptr<Booking>> bookingIndices;
    std::shared_ptr<Booking> activeBooking;
    QMessageBox* msgBox;
    std::shared_ptr<TravelAgency> travelAgency;
    Ui::TravelAgencyUI *ui;

private slots:

    void onReadBookings();
    void onReadIata();

    void onCustomerSearch();

    void onCustomerTravelListDoubleClicked();

    void onTravelBookingListDoubleClicked();

    void onSaveBookingsButtonClicked();

    void onCancelBookingsButtonClicked();

};


#endif //PG2PRAKTIKUM2_MAINWINDOW_H
