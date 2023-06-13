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
#include <QDesktopServices>
#include <QUuid>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QDateEdit>

TravelAgencyUI::TravelAgencyUI(std::shared_ptr<TravelAgency> _travelAgency, QWidget *parent) : QMainWindow(parent),
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
    connect(ui->flightMapButton, SIGNAL(clicked(bool)), this, SLOT(onOpenMapButtonClicked()));
    connect(ui->hotelMapButton, SIGNAL(clicked(bool)), this, SLOT(onOpenMapButtonClicked()));
    connect(ui->rentalMapButton, SIGNAL(clicked(bool)), this, SLOT(onOpenMapButtonClicked()));
    connect(ui->actionHinzuf_gen, SIGNAL(triggered(bool)), this, SLOT(onAddCustomer()));
    connect(ui->actionHinzuf_gen_2, SIGNAL(triggered(bool)), this, SLOT(onAddBooking()));


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

    QString test = QUuid::createUuid().toString();

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
    msgBox = new QMessageBox();
    if(travelAgency->readIataCodes(filePath.toStdString())){
        readIataCodes = true;
        msgBox->setWindowTitle("Erfolgreich eingelesen");
        msgBox->setText("Die Iata-Codes wurden erfolgreich eingelesen");
    }
    else{
        msgBox->setWindowTitle("Fehler");
        msgBox->setText("Es ist ein Fehler beim Einlesen der Iata-Codes aufgetreten");
    }
    msgBox->exec();
}

void TravelAgencyUI::onCustomerSearch() {

    msgBox = new QMessageBox();
    if(!readIataCodes){
        msgBox->setWindowTitle("Iata-Codes nicht eingelesen");
        msgBox->setText("Sie müssen zuerst die Datei mit den Iata-Codes einlesen");
        msgBox->exec();
        return;
    }
    long customerId = QInputDialog::getInt(this, "Kund*Innensuche", "ID");
    std::shared_ptr<Customer> customer = travelAgency->findCustomer(customerId);
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

QIcon TravelAgencyUI::getBookingIcon(std::shared_ptr<Booking> booking) {

    if (std::shared_ptr<FlightBooking> flightBooking = std::dynamic_pointer_cast<FlightBooking>(booking)) {
        return QIcon("../Images/Flight.png");
    } else if (std::shared_ptr<HotelBooking> hotelBooking = std::dynamic_pointer_cast<HotelBooking>(booking)) {
        return QIcon("../Images/Hotel.png");
    } else if (std::shared_ptr<RentalCarReservation> rentalCarReservation = dynamic_pointer_cast<RentalCarReservation>(booking)) {
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

    if (std::shared_ptr<FlightBooking> flightBooking = dynamic_pointer_cast<FlightBooking>(activeBooking)) {
        ui->bookingTabWidget->setCurrentWidget(ui->flightTab);

        std::shared_ptr<Airport> fromAirport = travelAgency->getAirport(flightBooking->getFromDestination());
        std::shared_ptr<Airport> toAirport = travelAgency->getAirport(flightBooking->getToDestination());

        ui->flightStartCodeLineEdit->setText(QString::fromStdString(flightBooking->getFromDestination()));
        ui->flightStartLineEdit->setText(QString::fromStdString(travelAgency->getAirport(flightBooking->getFromDestination())->getName()));
        ui->flightStartLineEdit->setStyleSheet("color: white");
        ui->flightEndCodeLineEdit->setText(QString::fromStdString(flightBooking->getToDestination()));
        ui->flightEndLineEdit->setText(QString::fromStdString(travelAgency->getAirport(flightBooking->getToDestination())->getName()));
        ui->flightEndLineEdit->setStyleSheet("color: white");
        ui->flightAirlineLineEdit->setText(QString::fromStdString(flightBooking->getAirline()));
        ui->flightClassLineEdit->setText(QString::fromStdString(flightBooking->getBookingClass()));

        /*QDesktopServices::openUrl(QUrl("http://townsendjennings.com/geo/?geojson={"
                                       "\"type\":\"LineString\","
                                       "\"coordinates\":[ "
                                       "[30, 10], [10, 30]"
                                       "], "
                                       "\"title\":\"Test\""
                                       "}" ));
                                       */


    } else if (std::shared_ptr<HotelBooking> hotelBooking = dynamic_pointer_cast<HotelBooking>(activeBooking)) {
        ui->bookingTabWidget->setCurrentWidget(ui->hotelTab);
        ui->hotelNameLineEdit->setText(QString::fromStdString(hotelBooking->getHotel()));
        ui->hotelTownLineEdit->setText(QString::fromStdString(hotelBooking->getTown()));
        ui->hotelRoomLineEdit->setText(QString::fromStdString(hotelBooking->getRoomType()));
    } else if (std::shared_ptr<RentalCarReservation> rentalCarReservation = dynamic_pointer_cast<RentalCarReservation>(activeBooking)) {
        ui->bookingTabWidget->setCurrentWidget(ui->rentalTab);
        ui->rentalPickupLineEdit->setText(QString::fromStdString(rentalCarReservation->getPickupLocation()));
        ui->rentalReturnLineEdit->setText(QString::fromStdString(rentalCarReservation->getReturnLocation()));
        ui->rentalCompanyLineEdit->setText(QString::fromStdString(rentalCarReservation->getCompany()));
        ui->rentalClassLineEdit->setText(QString::fromStdString(rentalCarReservation->getVehicleClass()));
    }
}


void TravelAgencyUI::onSaveBookingsButtonClicked() {
    std::shared_ptr<Booking> booking = travelAgency->findBooking(ui->bookingIdLineEdit->text().toStdString());


    if (std::shared_ptr<FlightBooking> flightBooking = dynamic_pointer_cast<FlightBooking>(booking)) {
        if(travelAgency->getAirport(ui->flightStartCodeLineEdit->text().toStdString()) == nullptr){
            ui->flightStartLineEdit->setText("Ungültiger Iata-Code");
            ui->flightStartLineEdit->setStyleSheet("color: red");
        }
        else{
            flightBooking->setFromDestination(ui->flightStartCodeLineEdit->text().toStdString());
        }
        if(travelAgency->getAirport(ui->flightEndCodeLineEdit->text().toStdString()) == nullptr){
            ui->flightEndLineEdit->setText("Ungültiger Iata-Code");
            ui->flightEndLineEdit->setStyleSheet("color: red");
        }
        else{
            flightBooking->setToDestination(ui->flightEndCodeLineEdit->text().toStdString());
        }
        if(travelAgency->getAirport(ui->flightStartCodeLineEdit->text().toStdString()) == nullptr || travelAgency->getAirport(ui->flightEndCodeLineEdit->text().toStdString()) == nullptr){
            return;
        }
        flightBooking->setAirline(ui->flightAirlineLineEdit->text().toStdString());
        flightBooking->setBookingClass(ui->flightClassLineEdit->text().toStdString());

    } else if (std::shared_ptr<HotelBooking> hotelBooking = dynamic_pointer_cast<HotelBooking>(booking)) {
        hotelBooking->setHotel(ui->hotelNameLineEdit->text().toStdString());
        hotelBooking->setTown(ui->hotelTownLineEdit->text().toStdString());
        hotelBooking->setRoomType(ui->hotelRoomLineEdit->text().toStdString());
    } else if (std::shared_ptr<RentalCarReservation> rentalCarReservation = dynamic_pointer_cast<RentalCarReservation>(booking)) {
        rentalCarReservation->setPickupLocation(ui->rentalPickupLineEdit->text().toStdString());
        rentalCarReservation->setReturnLocation(ui->rentalReturnLineEdit->text().toStdString());
        rentalCarReservation->setCompany(ui->rentalCompanyLineEdit->text().toStdString());
        rentalCarReservation->setVehicleClass(ui->rentalClassLineEdit->text().toStdString());
    }
    booking->setFromDate(ui->bookingStartDateEdit->date().toString("yyyyMMdd").toStdString());
    booking->setToDate(ui->bookingEndDateEdit->date().toString("yyyyMMdd").toStdString());
    booking->setPrice(ui->bookingPriceEdit->value());

    loadTravels(ui->customerIdLineEdit->text().toLong());
    loadBookings(ui->travelIdLineEdit->text().toLong());
    loadBookingDetails();

}


void TravelAgencyUI::onCancelBookingsButtonClicked() {
    loadBookingDetails();
}

void TravelAgencyUI::onOpenMapButtonClicked() {

    QString geoJson;


    if (std::shared_ptr<FlightBooking> flightBooking = dynamic_pointer_cast<FlightBooking>(activeBooking)){
        geoJson =  "http://townsendjennings.com/geo/?geojson={"
                           "\"type\":\"LineString\","
                           "\"coordinates\":["
                           "[" + QString::fromStdString(flightBooking->getToDestCoordinates()) + "], [" + QString::fromStdString(flightBooking->getFromDestCoordinates()) + "]"
                                                                                                                                                                             "]"
                                                                                                                                                                             "}";

    }
    else if(std::shared_ptr<HotelBooking> hotelBooking = dynamic_pointer_cast<HotelBooking>(activeBooking)){
        geoJson =  "http://townsendjennings.com/geo/?geojson={"
                   "\"type\":\"Point\","
                   "\"coordinates\":["
                    + QString::fromStdString(hotelBooking->getHotelCoordinates()) + "]"
                                                                                                                                                                    "}";
    }
    else if(std::shared_ptr<RentalCarReservation> rentalCarReservation = dynamic_pointer_cast<RentalCarReservation>(activeBooking)){
        if(rentalCarReservation->getReturnLocation() == rentalCarReservation->getPickupLocation()){
            geoJson =  "http://townsendjennings.com/geo/?geojson={"
                       "\"type\":\"Point\","
                       "\"coordinates\":["
                       + QString::fromStdString(rentalCarReservation->getPickupCoordinates()) + "]"
                                                                                       "}";
        }
        else{
            geoJson =  "http://townsendjennings.com/geo/?geojson={"
                       "\"type\":\"LineString\","
                       "\"coordinates\":["
                       "[" + QString::fromStdString(rentalCarReservation->getPickupCoordinates()) + "], [" + QString::fromStdString(rentalCarReservation->getReturnCoordinates()) + "]"
                                                                                                                                                                              "]"
                                                                                                                                                                              "}";
        }

    }
    std::cout << geoJson.toStdString();
    QDesktopServices::openUrl(QUrl(geoJson));

}

void TravelAgencyUI::onAddCustomer() {
    std::string customerName = QInputDialog::getText(this, "Kunde hinzufügen", "Name des Kunden").toStdString();
    if(customerName == ""){
        QMessageBox::information(this, "Fehler", "Geben Sie bitte einen Namen ein");
        return;
    }
    long id = travelAgency->getNextCustomerId();
    std::shared_ptr<Customer> customer = std::shared_ptr<Customer>(new Customer(id, customerName));
    travelAgency->addCustomer(customer);
    QMessageBox::information(this, "Kunde hinzugefügt", "Der Kunde mit dem Namen " + QString::fromStdString(customerName) + " wurde mit der id " + QString::fromStdString(std::to_string(id)) + " hinzugefügt");

}

void TravelAgencyUI::onAddBooking() {
    QLineEdit* fromDest;
    QLineEdit* toDest;
    QLineEdit* airline;
    QLineEdit* bookingClass;
    QLineEdit* pickupLocation;
    QLineEdit* returnLocation;
    QLineEdit* company;
    QLineEdit* vehicleClass;
    QLineEdit* hotel;
    QLineEdit* town;
    QLineEdit* roomType;

    QDialog dialog(this);
    QFormLayout form(&dialog);

    form.addRow(new QLabel("Buchungsdetails"));

    QDoubleSpinBox * price = new QDoubleSpinBox(&dialog);
    price->setMaximum(999999);
    form.addRow("Preis", price);

    QDateEdit* fromDate = new QDateEdit(&dialog);
    fromDate->setCalendarPopup(true);
    form.addRow("Start", fromDate);

    QDateEdit* toDate = new QDateEdit(&dialog);
    toDate->setCalendarPopup(true);
    form.addRow("Ende", toDate);

    msgBox = new QMessageBox();
    msgBox->setWindowTitle("Buchung hinzufügen");
    msgBox->setText("Soll die Reise zu einer bestehenden Reise hinzugefügt werden, oder soll eine neue Reise erstellt werden?");
    QAbstractButton *newTravel = reinterpret_cast<QAbstractButton *>(msgBox->addButton("Neue Reise",
                                                                                        QMessageBox::ActionRole));
    QAbstractButton *travel = reinterpret_cast<QAbstractButton *>(msgBox->addButton("Zu Reise hinzufügen",
                                                                                          QMessageBox::ActionRole));
    QAbstractButton *cancel = reinterpret_cast<QAbstractButton *>(msgBox->addButton("Abbrechen",
                                                                                    QMessageBox::ActionRole));

    msgBox->exec();

    if(msgBox->clickedButton() == newTravel){

        msgBox = new QMessageBox();
        msgBox->setWindowTitle("Buchung hinzufügen");
        msgBox->setText("Soll die Reise zu einer bestehenden Reise hinzugefügt werden, oder soll eine neue Reise erstellt werden?");
        QAbstractButton *flightBooking = reinterpret_cast<QAbstractButton *>(msgBox->addButton("Flugbuchung",
                                                                                               QMessageBox::ActionRole));
        QAbstractButton *hotelBooking = reinterpret_cast<QAbstractButton *>(msgBox->addButton("Hotelbuchung",
                                                                                              QMessageBox::ActionRole));
        QAbstractButton *rentalBooking = reinterpret_cast<QAbstractButton *>(msgBox->addButton("Mietwagenbuchung",
                                                                                               QMessageBox::ActionRole));
        QAbstractButton *cancel = reinterpret_cast<QAbstractButton *>(msgBox->addButton("Abbrechen",
                                                                                        QMessageBox::ActionRole));

        msgBox->exec();

        if(msgBox->clickedButton() == cancel){
            return;
        }
        if(msgBox->clickedButton() == flightBooking){
            fromDest = new QLineEdit(&dialog);
            form.addRow("Start Flughafen", fromDest);

            toDest = new QLineEdit(&dialog);

            form.addRow("Ziel Flughafen", toDest);

            airline = new QLineEdit(&dialog);
            form.addRow("Fluggesellschaft", airline);

            bookingClass = new QLineEdit(&dialog);
            form.addRow("Buchungsklasse", bookingClass);
        }
        if(msgBox->clickedButton() == hotelBooking){
            hotel = new QLineEdit(&dialog);
            form.addRow("Hotel", hotel);

            town = new QLineEdit(&dialog);
            form.addRow("Stadt", town);

            roomType = new QLineEdit(&dialog);
            form.addRow("Zimmertyp", roomType);
        }
        if(msgBox->clickedButton() == rentalBooking){
            pickupLocation = new QLineEdit(&dialog);
            form.addRow("Abholort", pickupLocation);

            returnLocation = new QLineEdit(&dialog);
            form.addRow("Rückgabeort", returnLocation);

            company = new QLineEdit(&dialog);
            form.addRow("Firma", company);

            vehicleClass = new QLineEdit(&dialog);
            form.addRow("Fahrzeugklasse", vehicleClass);
        }

        QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
                                   Qt::Horizontal, &dialog);
        form.addRow(&buttonBox);
        QObject::connect(&buttonBox, SIGNAL(accepted()), &dialog, SLOT(accept()));
        QObject::connect(&buttonBox, SIGNAL(rejected()), &dialog, SLOT(reject()));

        if (dialog.exec() == QDialog::Accepted ) {

            //std::shared_ptr<Booking> booking = std::shared_ptr<Booking>(new Booking(QUuid::createUuid().toString().toStdString(), price->value(), fromDate->date().toString(), toDate->date().toString()));
            long customerId = QInputDialog::getInt(this, "KundenId", "Zu welchem Kunden soll die Reise hinzugefügt werden");

            if(travelAgency->findCustomer(customerId) == nullptr){
                QMessageBox::information(this, "Fehler", "Kunde mit id " + QString::number(customerId) + " nicht gefunden");
                return;
            }

            long travelId = travelAgency->getNextTravelId();
            std::shared_ptr<Travel> travel = std::shared_ptr<Travel>(new Travel(travelId, customerId));


            travelAgency->addTravel(travel);
        }

    }
    if(msgBox->clickedButton() == travel){

    }
    if(msgBox->clickedButton() == cancel){
        return;
    }
}


