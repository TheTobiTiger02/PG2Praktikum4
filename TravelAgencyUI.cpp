//
// Created by GBT B450M-S2H on 21.04.2023.
//

// You may need to build the project (run Qt uic code generator) to get "ui_TravelAgencyUI.h" resolved

#include "TravelAgencyUI.h"
#include <QFileDialog>
#include <iostream>
#include <QMessageBox>
#include <QAbstractButton>
#include <QTranslator>
#include <QInputDialog>

TravelAgencyUI::TravelAgencyUI(TravelAgency *_travelAgency, QWidget *parent) : QMainWindow(parent),
                                                                               ui(new Ui::TravelAgencyUI),
                                                                               travelAgency(_travelAgency) {
    ui->setupUi(this);
    connect(ui->actionEinlesen, SIGNAL(triggered()), this, SLOT(onReadBookings()));
    connect(ui->actionIata_Codes_Einlesen, SIGNAL(triggered(bool)), this, SLOT(onReadIata()));
    connect(ui->actionSuchen, SIGNAL(triggered(bool)), this, SLOT(onCustomerSearch()));
    connect(ui->customerTravelsTableWidget, SIGNAL(itemDoubleClicked(QTableWidgetItem * )), this,
            SLOT(onCustomerTravelListDoubleClicked()));
    connect(ui->travelBookingsTableWidget, SIGNAL(itemDoubleClicked(QTableWidgetItem * )), this,
            SLOT(onTravelBookingListDoubleClicked()));
    connect(ui->saveBookingButton, SIGNAL(clicked(bool)), this, SLOT(onSaveBookingsButtonClicked()));
    connect(ui->cancelBookingButton, SIGNAL(clicked(bool)), this, SLOT(onCancelBookingsButtonClicked()));
    ui->customerGroupBox->setVisible(false);
    ui->customerTravelsTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->customerTravelsTableWidget->setColumnWidth(0, 100);
    ui->customerTravelsTableWidget->setColumnWidth(1, 200);
    ui->customerTravelsTableWidget->setColumnWidth(2, 200);

    ui->travelGroupBox->setVisible(false);
    ui->travelBookingsTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->travelBookingsTableWidget->setColumnWidth(0, 90);
    ui->travelBookingsTableWidget->setColumnWidth(1, 160);
    ui->travelBookingsTableWidget->setColumnWidth(2, 160);
    ui->travelBookingsTableWidget->setColumnWidth(3, 90);

    ui->bookingGroupBox->setVisible(false);


}


TravelAgencyUI::~TravelAgencyUI() {
    delete ui;
}

void TravelAgencyUI::onReadBookings() {
    QString filePath = QFileDialog::getOpenFileName(this, "Datei auswählen", "*.json");
    travelAgency->readFile(filePath.toStdString());
}
void TravelAgencyUI::onReadIata() {
    QString filePath = QFileDialog::getOpenFileName(this, "Datei auswählen", "*.json");
    travelAgency->readIataCodes(filePath.toStdString());


}

void TravelAgencyUI::onCustomerSearch() {
    msgBox = new QMessageBox();
    long customerId = QInputDialog::getInt(this, "Kund*Innensuche", "ID");
    Customer *customer = travelAgency->findCustomer(customerId);
    if (customer == nullptr) {
        msgBox->setWindowTitle("Kunde nicht gefunden");
        msgBox->setText("Der Kunde mit der ID " + QString::fromStdString(std::to_string(customerId)) +
                        " konnte nicht gefunden werden");
        msgBox->exec();
        return;
    }
    ui->customerGroupBox->setVisible(true);
    ui->customerIdLineEdit->setText(QString::fromStdString(std::to_string(customer->getId())));
    ui->customerNameLineEdit->setText(QString::fromStdString(customer->getName()));

    loadTravels(customerId);
    ui->travelGroupBox->setVisible(false);
    ui->bookingGroupBox->setVisible(false);


}

void TravelAgencyUI::loadTravels(long customerId) {
    ui->customerTravelsTableWidget->clearContents();
    QLocale locale(QLocale::German);
    int currentRow = 0;
    for (auto t: travelAgency->getAllTravels()) {
        if (t->getCustomerId() == customerId) {

            ui->customerTravelsTableWidget->setRowCount(currentRow + 1);
            ui->customerTravelsTableWidget->setItem(currentRow, TRAVEL_ID, new QTableWidgetItem(
                    QString::fromStdString(std::to_string(t->getId()))));
            ui->customerTravelsTableWidget->setItem(currentRow, TRAVEL_BEGIN, new QTableWidgetItem(
                    locale.toString(t->getStartDate(), "dddd, d. MMMM yyyy")));
            ui->customerTravelsTableWidget->setItem(currentRow, TRAVEL_END, new QTableWidgetItem(
                    locale.toString(t->getEndDate(), "dddd, d. MMMM yyyy")));
            currentRow = ui->customerTravelsTableWidget->rowCount();
        }
    }
}

void TravelAgencyUI::onCustomerTravelListDoubleClicked() {
    long travelId = ui->customerTravelsTableWidget->item(ui->customerTravelsTableWidget->currentRow(),
                                                         0)->text().toLong();
    loadBookings(travelId);
    ui->bookingGroupBox->setVisible(false);
}

void TravelAgencyUI::loadBookings(long travelId) {
    QLocale locale(QLocale::German);
    int currentRow = 0;
    ui->travelGroupBox->setVisible(true);
    ui->travelBookingsTableWidget->clearContents();
    ui->travelIdLineEdit->setText(QString::number(travelId));
    bookingIndices.clear();
    int index = 0;
    for (auto b: travelAgency->getAllBookings()) {
        if (b->getTravelId() == travelId) {
            ui->travelBookingsTableWidget->setRowCount(currentRow + 1);
            QTableWidgetItem *imageItem = new QTableWidgetItem();
            imageItem->setIcon(getBookingIcon(b));
            ui->travelBookingsTableWidget->setItem(currentRow, BOOKING_TYPE, imageItem);
            ui->travelBookingsTableWidget->setItem(currentRow, BOOKING_START, new QTableWidgetItem(
                    locale.toString(b->getFromDate(), "dddd, d. MMMM yyyy")));
            ui->travelBookingsTableWidget->setItem(currentRow, BOOKING_END, new QTableWidgetItem(
                    locale.toString(b->getToDate(), "dddd, d. MMMM yyyy")));
            ui->travelBookingsTableWidget->setItem(currentRow, BOOKING_PRICE,
                                                   new QTableWidgetItem(QString::number(b->getPrice())));
            currentRow = ui->travelBookingsTableWidget->rowCount();

            bookingIndices[index] = b;
            index++;
        }
    }
}

QIcon TravelAgencyUI::getBookingIcon(Booking *booking) {

    if (FlightBooking *flightBooking = dynamic_cast<FlightBooking *>(booking)) {
        return QIcon("../Images/Flight.png");
    } else if (HotelBooking *hotelBooking = dynamic_cast<HotelBooking *>(booking)) {
        return QIcon("../Images/Hotel.png");
    } else if (RentalCarReservation *rentalCarReservation = dynamic_cast<RentalCarReservation *>(booking)) {
        return QIcon("../Images/Car.png");
    }


}

void TravelAgencyUI::onTravelBookingListDoubleClicked() {
    activeBooking = bookingIndices[ui->travelBookingsTableWidget->currentRow()];
    loadBookingDetails();
}

void TravelAgencyUI::loadBookingDetails() {
    ui->bookingGroupBox->setVisible(true);
    ui->bookingIdLineEdit->setText(QString::fromStdString(activeBooking->getId()));
    ui->bookingStartDateEdit->setDate(activeBooking->getFromDate());
    ui->bookingEndDateEdit->setDate(activeBooking->getToDate());
    ui->bookingPriceEdit->setValue(activeBooking->getPrice());

    if (FlightBooking *flightBooking = dynamic_cast<FlightBooking *>(activeBooking)) {
        ui->bookingTabWidget->setCurrentWidget(ui->flightTab);
        ui->flightStartCodeLineEdit->setText(QString::fromStdString(flightBooking->getFromDestination()));
        ui->flightStartLineEdit->setText(QString::fromStdString(travelAgency->getAirport(flightBooking->getFromDestination())->getName()));
        ui->flightEndCodeLineEdit->setText(QString::fromStdString(flightBooking->getToDestination()));
        ui->flightEndLineEdit->setText(QString::fromStdString(travelAgency->getAirport(flightBooking->getToDestination())->getName()));
        ui->flightAirlineLineEdit->setText(QString::fromStdString(flightBooking->getAirline()));
        ui->flightClassLineEdit->setText(QString::fromStdString(flightBooking->getBookingClass()));
    } else if (HotelBooking *hotelBooking = dynamic_cast<HotelBooking *>(activeBooking)) {
        ui->bookingTabWidget->setCurrentWidget(ui->hotelTab);
        ui->hotelNameLineEdit->setText(QString::fromStdString(hotelBooking->getHotel()));
        ui->hotelTownLineEdit->setText(QString::fromStdString(hotelBooking->getTown()));
        ui->hotelRoomLineEdit->setText(QString::fromStdString(hotelBooking->getRoomType()));
    } else if (RentalCarReservation *rentalCarReservation = dynamic_cast<RentalCarReservation *>(activeBooking)) {
        ui->bookingTabWidget->setCurrentWidget(ui->rentalTab);
        ui->rentalPickupLineEdit->setText(QString::fromStdString(rentalCarReservation->getPickupLocation()));
        ui->rentalReturnLineEdit->setText(QString::fromStdString(rentalCarReservation->getReturnLocation()));
        ui->rentalCompanyLineEdit->setText(QString::fromStdString(rentalCarReservation->getCompany()));
        ui->rentalClassLineEdit->setText(QString::fromStdString(rentalCarReservation->getVehicleClass()));
    }
}


void TravelAgencyUI::onSaveBookingsButtonClicked() {
    Booking *booking = travelAgency->findBooking(ui->bookingIdLineEdit->text().toStdString());
    booking->setFromDate(ui->bookingStartDateEdit->date().toString("yyyyMMdd").toStdString());
    booking->setToDate(ui->bookingEndDateEdit->date().toString("yyyyMMdd").toStdString());
    booking->setPrice(ui->bookingPriceEdit->value());

    if (FlightBooking *flightBooking = dynamic_cast<FlightBooking *>(booking)) {
        flightBooking->setFromDestination(ui->flightStartCodeLineEdit->text().toStdString());
        flightBooking->setToDestination(ui->flightEndCodeLineEdit->text().toStdString());
        flightBooking->setAirline(ui->flightAirlineLineEdit->text().toStdString());
        flightBooking->setBookingClass(ui->flightClassLineEdit->text().toStdString());
    } else if (HotelBooking *hotelBooking = dynamic_cast<HotelBooking *>(booking)) {
        hotelBooking->setHotel(ui->hotelNameLineEdit->text().toStdString());
        hotelBooking->setTown(ui->hotelTownLineEdit->text().toStdString());
        hotelBooking->setRoomType(ui->hotelRoomLineEdit->text().toStdString());
    } else if (RentalCarReservation *rentalCarReservation = dynamic_cast<RentalCarReservation *>(booking)) {
        rentalCarReservation->setPickupLocation(ui->rentalPickupLineEdit->text().toStdString());
        rentalCarReservation->setReturnLocation(ui->rentalReturnLineEdit->text().toStdString());
        rentalCarReservation->setCompany(ui->rentalCompanyLineEdit->text().toStdString());
        rentalCarReservation->setVehicleClass(ui->rentalClassLineEdit->text().toStdString());
    }
    loadTravels(ui->customerIdLineEdit->text().toLong());
    loadBookings(ui->travelIdLineEdit->text().toLong());
    loadBookingDetails();

}


void TravelAgencyUI::onCancelBookingsButtonClicked() {
    loadBookingDetails();
}


