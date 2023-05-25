//
// Created by GBT B450M-S2H on 14.04.2023.
//

#include "TravelAgency.h"
#include "json.hpp"
#include <fstream>
#include <QMessageBox>
#include <QPushButton>
#include <QDialogButtonBox>
#include <QCoreApplication>
#include <QObject>

using namespace std;
using json = nlohmann::json;

TravelAgency::~TravelAgency() {
    for (auto b: allBookings) {
        delete b;
    }

    allBookings.clear();
}

void
TravelAgency::readFile(string filePath, int startRow, int flightCount, int hotelCount, int carCount, int travelCount,
                       int customerCount, double totalPrice) {
    ifstream file(filePath);
    //msgBox = new QMessageBox;
    static QMessageBox msgBox;

    //msgBox = new QMessageBox();
    if (!file) {

        msgBox.setWindowTitle("Fehler beim Einlesen der Buchungen");
        msgBox.setText("Datei konnte nicht gelesen werden");
        msgBox.exec();
        return;
    }
    json data;
    file >> data;
    string type, id, fromDate, toDate;
    double price;
    long travelId;

    Booking *booking;
    for (int i = startRow; i < data.size(); i++) {

        try {
            if (data.at(i)["type"].is_null() || data.at(i)["type"].empty() ||
                data.at(i)["type"].get<string>().length() == 0) {
                throw runtime_error("Leeres Attribut 'type' in Zeile " + to_string(i + 1));
            }
            if (data.at(i)["id"].is_null() || data.at(i)["id"].empty() ||
                data.at(i)["id"].get<string>().length() == 0) {
                throw runtime_error("Leeres Attribut 'id' in Zeile " + to_string(i + 1));
            }
            if (data.at(i)["fromDate"].is_null() || data.at(i)["fromDate"].empty() ||
                data.at(i)["fromDate"].get<string>().length() == 0) {
                throw runtime_error("Leeres Attribut 'fromDate' in Zeile " + to_string(i + 1));
            }
            if (data.at(i)["toDate"].is_null() || data.at(i)["toDate"].empty() ||
                data.at(i)["toDate"].get<string>().length() == 0) {
                throw runtime_error("Leeres Attribut 'toDate' in Zeile " + to_string(i + 1));
            }
            if (data.at(i)["price"].is_null() || data.at(i)["price"].empty()) {
                throw runtime_error("Leeres Attribut 'price' in Zeile " + to_string(i + 1));
            }
            if (data.at(i)["travelId"].is_null() || data.at(i)["travelId"].empty()) {
                throw runtime_error("Leeres Attribut 'travelId' in Zeile " + to_string(i + 1));
            }
            type = data.at(i)["type"];
            id = data.at(i)["id"];
            if(findBooking(id) != nullptr){
                continue;
            }
            price = data.at(i)["price"];
            fromDate = data.at(i)["fromDate"];
            toDate = data.at(i)["toDate"];
            travelId = data.at(i)["travelId"];

            if (type == "Flight") {
                if (data.at(i)["fromDest"].is_null() || data.at(i)["fromDest"].empty() ||
                    data.at(i)["fromDest"].get<string>().length() == 0) {
                    throw runtime_error("Leeres Attribut 'fromDest' in Zeile " + to_string(i + 1));
                }
                if (data.at(i)["toDest"].is_null() || data.at(i)["toDest"].empty() ||
                    data.at(i)["toDest"].get<string>().length() == 0) {
                    throw runtime_error("Leeres Attribut 'toDest' in Zeile " + to_string(i + 1));
                }
                if (data.at(i)["airline"].is_null() || data.at(i)["airline"].empty() ||
                    data.at(i)["airline"].get<string>().length() == 0) {
                    throw runtime_error("Leeres Attribut 'airline' in Zeile " + to_string(i + 1));
                }
                if (data.at(i)["bookingClass"].is_null() || data.at(i)["bookingClass"].empty() ||
                    data.at(i)["bookingClass"].get<string>().length() == 0) {
                    throw runtime_error("Leeres Attribut 'bookingClass' in Zeile " + to_string(i + 1));
                }
                if (data.at(i)["fromDest"].get<string>().length() != 3 ||
                    data.at(i)["toDest"].get<string>().length() != 3) {
                    throw runtime_error("Falsches Flughafenkuerzel in Zeile " + to_string(i + 1));
                }
                if (data.at(i)["fromDestLatitude"].is_null() || data.at(i)["fromDestLatitude"].empty() ||
                    data.at(i)["fromDestLatitude"].get<string>().length() == 0) {
                    throw runtime_error("Leeres Attribut 'fromDestLatitude' in Zeile " + to_string(i + 1));
                }
                if (data.at(i)["fromDestLongitude"].is_null() || data.at(i)["fromDestLongitude"].empty() ||
                    data.at(i)["fromDestLongitude"].get<string>().length() == 0) {
                    throw runtime_error("Leeres Attribut 'fromDestLongitude' in Zeile " + to_string(i + 1));
                }
                if (data.at(i)["toDestLatitude"].is_null() || data.at(i)["toDestLatitude"].empty() ||
                    data.at(i)["toDestLatitude"].get<string>().length() == 0) {
                    throw runtime_error("Leeres Attribut 'toDestLatitude' in Zeile " + to_string(i + 1));
                }
                if (data.at(i)["toDestLongitude"].is_null() || data.at(i)["toDestLongitude"].empty() ||
                    data.at(i)["toDestLongitude"].get<string>().length() == 0) {
                    throw runtime_error("Leeres Attribut 'toDestLongitude' in Zeile " + to_string(i + 1));
                }

                //cout << data.at(i)["bookingClass"][0];
                booking = new FlightBooking(id, price, fromDate, toDate, travelId, data.at(i)["fromDest"],
                                            data.at(i)["toDest"], data.at(i)["airline"],
                                            data.at(i)["bookingClass"].get<string>()[0], data.at(i)["fromDestLatitude"].get<string>() + "," + data.at(i)["fromDestLongitude"].get<string>(),
                                            data.at(i)["toDestLatitude"].get<string>() + "," + data.at(i)["toDestLongitude"].get<string>());
                flightCount++;
                totalPrice += price;

            } else if (type == "Hotel") {
                if (data.at(i)["hotel"].is_null() || data.at(i)["hotel"].empty() ||
                    data.at(i)["hotel"].get<string>().length() == 0) {
                    throw runtime_error("Leeres Attribut 'hotel' in Zeile " + to_string(i + 1));
                }
                if (data.at(i)["town"].is_null() || data.at(i)["town"].empty() ||
                    data.at(i)["town"].get<string>().length() == 0) {
                    throw runtime_error("Leeres Attribut 'town' in Zeile " + to_string(i + 1));
                }
                if (data.at(i)["roomType"].is_null() || data.at(i)["roomType"].empty() ||
                    data.at(i)["roomType"].get<string>().length() == 0) {
                    throw runtime_error("Leeres Attribut 'roomType' in Zeile " + to_string(i + 1));
                }
                if (data.at(i)["hotelLatitude"].is_null() || data.at(i)["hotelLatitude"].empty() ||
                    data.at(i)["hotelLatitude"].get<string>().length() == 0) {
                    throw runtime_error("Leeres Attribut 'hotelLatitude' in Zeile " + to_string(i + 1));
                }
                if (data.at(i)["hotelLongitude"].is_null() || data.at(i)["hotelLongitude"].empty() ||
                    data.at(i)["hotelLongitude"].get<string>().length() == 0) {
                    throw runtime_error("Leeres Attribut 'hotelLongitude' in Zeile " + to_string(i + 1));
                }
                booking = new HotelBooking(id, price, fromDate, toDate, travelId, data.at(i)["hotel"],
                                           data.at(i)["town"], data.at(i)["roomType"], data.at(i)["hotelLatitude"].get<string>() + "," + data.at(i)["hotelLongitude"].get<string>());
                hotelCount++;
                totalPrice += price;
            } else if (type == "RentalCar") {
                if (data.at(i)["pickupLocation"].is_null() || data.at(i)["pickupLocation"].empty() ||
                    data.at(i)["pickupLocation"].get<string>().length() == 0) {
                    throw runtime_error("Leeres Attribut 'pickupLocation' in Zeile " + to_string(i + 1));
                }
                if (data.at(i)["returnLocation"].is_null() || data.at(i)["returnLocation"].empty() ||
                    data.at(i)["returnLocation"].get<string>().length() == 0) {
                    throw runtime_error("Leeres Attribut 'returnLocation' in Zeile " + to_string(i + 1));
                }
                if (data.at(i)["company"].is_null() || data.at(i)["company"].empty() ||
                    data.at(i)["company"].get<string>().length() == 0) {
                    throw runtime_error("Leeres Attribut 'company' in Zeile " + to_string(i + 1));
                }
                if (data.at(i)["vehicleClass"].is_null() || data.at(i)["vehicleClass"].empty() ||
                    data.at(i)["vehicleClass"].get<string>().length() == 0) {
                    throw runtime_error("Leeres Attribut 'vehicleClass' in Zeile " + to_string(i + 1));
                }
                if (data.at(i)["pickupLatitude"].is_null() || data.at(i)["pickupLatitude"].empty() ||
                    data.at(i)["pickupLatitude"].get<string>().length() == 0) {
                    throw runtime_error("Leeres Attribut 'pickupLatitude' in Zeile " + to_string(i + 1));
                }
                if (data.at(i)["pickupLongitude"].is_null() || data.at(i)["pickupLongitude"].empty() ||
                    data.at(i)["pickupLongitude"].get<string>().length() == 0) {
                    throw runtime_error("Leeres Attribut 'pickupLongitude' in Zeile " + to_string(i + 1));
                }
                if (data.at(i)["returnLatitude"].is_null() || data.at(i)["returnLatitude"].empty() ||
                    data.at(i)["returnLatitude"].get<string>().length() == 0) {
                    throw runtime_error("Leeres Attribut 'returnLatitude' in Zeile " + to_string(i + 1));
                }
                if (data.at(i)["returnLongitude"].is_null() || data.at(i)["returnLongitude"].empty() ||
                    data.at(i)["returnLongitude"].get<string>().length() == 0) {
                    throw runtime_error("Leeres Attribut 'returnLongitude' in Zeile " + to_string(i + 1));
                }
                booking = new RentalCarReservation(id, price, fromDate, toDate, travelId, data.at(i)["pickupLocation"],
                                                   data.at(i)["returnLocation"], data.at(i)["company"],
                                                   data.at(i)["vehicleClass"], data.at(i)["pickupLatitude"].get<string>() + "," + data.at(i)["pickupLongitude"].get<string>(),
                                                   data.at(i)["returnLatitude"].get<string>() + "," + data.at(i)["returnLongitude"].get<string>());


                carCount++;
                totalPrice += price;
            }

            if (data.at(i)["customerId"].is_null() || data.at(i)["customerId"].empty()) {
                throw runtime_error("Leeres Attribut 'customerId' in Zeile " + to_string(i + 1));
            }
            long customerId = data.at(i)["customerId"];
            allBookings.push_back(booking);

            Travel *travel;
            if (findTravel(travelId) == nullptr) {
                travel = new Travel(travelId, customerId);
                travel->addBooking(booking);
                allTravels.push_back(travel);
                travelCount++;
            } else {
                findTravel(travelId)->addBooking(booking);
            }

            if (findCustomer(customerId) == nullptr) {
                Customer *customer = new Customer(customerId, data.at(i)["customerName"]);
                customer->addTravel(travel);
                allCustomers.push_back(customer);
                customerCount++;
            } else {
                findCustomer(customerId)->addTravel(travel);
            }


        }
        catch (runtime_error r) {
            cout << r.what() << endl << "Haben Sie die Datei korrigiert? (j/n)" << endl;
            msgBox.setWindowTitle("Fehler beim Einlesen der Buchungen");
            msgBox.setText(QString::fromStdString(r.what()));
            msgBox.setInformativeText(
                    "Wenn Sie die Datei bereits korrigiert haben, wählen Sie 'Wiederholen'. Wählen Sie 'Verwerfen', um alle Buchungen zu löschen "
                    "und 'Abbrechen', um die vorhandenen Buchungen stehenzulassen und diesen Dialog zu verlassen");


            QAbstractButton *retryButton = reinterpret_cast<QAbstractButton *>(msgBox.addButton("Wiederholen",
                                                                                                QMessageBox::ActionRole));
            QAbstractButton *discardButton = reinterpret_cast<QAbstractButton *>(msgBox.addButton("Verwerfen",
                                                                                                  QMessageBox::ActionRole));

            QAbstractButton *cancelButton = reinterpret_cast<QAbstractButton *>(msgBox.addButton("Abbrechen",
                                                                                                 QMessageBox::ActionRole));
            QAbstractButton *detailsButton = msgBox.addButton("Details ausblenden", QMessageBox::ActionRole);


            detailsButton->disconnect();
            QObject::connect(detailsButton, &QAbstractButton::clicked, &msgBox, [=]() {
                if (detailsButton->text() == "Details ausblenden") {
                    detailsButton->setText("Details anzeigen");
                    msgBox.setInformativeText("");
                    //msgBox.exec();
                } else {
                    detailsButton->setText("Details ausblenden");
                    msgBox.setInformativeText(
                            "Wenn Sie die Datei bereits korrigiert haben, wählen Sie 'Wiederholen'. Wählen Sie 'Verwerfen', um alle Buchungen zu löschen "
                            "und 'Abbrechen', um die vorhandenen Buchungen stehenzulassen und diesen Dialog zu verlassen");
                    msgBox.setFixedSize(400, 200);
                    //msgBox.exec();

                }
            });
            msgBox.exec();


            if (msgBox.clickedButton() == retryButton) {
                readFile(filePath, i, flightCount, hotelCount, carCount, travelCount, customerCount, totalPrice);
                return;
            }
            if (msgBox.clickedButton() == discardButton) {
                for (auto b: allBookings) {
                    delete b;
                }
                return;
            }
            if (msgBox.clickedButton() == cancelButton) {
                return;
            }



            /*char corrected {};
            while(tolower(corrected) != 'j'){
                cin >> corrected;
            }
            readFile(filePath, i, flightCount, hotelCount, carCount, totalPrice, totalPrice, totalPrice);
             */
            //return;
        }
        catch (json::type_error) {
            cout << "1234";
            msgBox.setWindowTitle("Fehler beim Einlesen der Buchungen");
            msgBox.setText(
                    "Attribut price hat keinen numerischen Wert in Zeile " + QString::fromStdString(to_string(i + 1)));
            msgBox.setInformativeText(
                    "Wenn Sie die Datei bereits korrigiert haben, wählen Sie 'Wiederholen'. Wählen Sie 'Verwerfen', um alle Buchungen zu löschen "
                    "und 'Abbrechen', um die vorhandenen Buchungen stehenzulassen und diesen Dialog zu verlassen");


            QAbstractButton *retryButton = reinterpret_cast<QAbstractButton *>(msgBox.addButton("Wiederholen",
                                                                                                QMessageBox::ActionRole));
            QAbstractButton *discardButton = reinterpret_cast<QAbstractButton *>(msgBox.addButton("Verwerfen",
                                                                                                  QMessageBox::ActionRole));


            QAbstractButton *cancelButton = reinterpret_cast<QAbstractButton *>(msgBox.addButton("Abbrechen",
                                                                                                 QMessageBox::ActionRole));
            discardButton->disconnect();

            QAbstractButton *doNotCloseButton = msgBox.addButton("This button will not close anything",
                                                                 QMessageBox::ActionRole);
            doNotCloseButton->disconnect();


            QObject::connect(discardButton, &QAbstractButton::clicked, &msgBox, [=]() {
                discardButton->setText("Test");
            });


            msgBox.exec();


            if (msgBox.clickedButton() == retryButton) {
                readFile(filePath, i, flightCount, hotelCount, carCount, travelCount, customerCount, totalPrice);
                return;
            }
            if (msgBox.clickedButton() == discardButton) {
                for (auto b: allBookings) {
                    delete b;
                }
                return;
            }

            if (msgBox.clickedButton() == cancelButton) {
                return;
            }

            /*cout << "Attribut 'price' hat keinen numerischen Wert" << endl;
            cout << "Haben Sie die Datei korrigiert? (j/n)" << endl;
            char corrected {};
            while(tolower(corrected) != 'j'){
                cin >> corrected;
            }
            readFile(filePath, i, flightCount, hotelCount, carCount, totalPrice, totalPrice, totalPrice);
            return;
             */
        }


    }
    file.close();


    msgBox.setWindowTitle("Datei erfolgreich eingelesen");
    msgBox.setText(QString::fromStdString(
            "Es wurden " + to_string(flightCount) + " Flugbuchungen, " + to_string(hotelCount) +
            " Hotelbuchungen und " +
            to_string(carCount) + " Mietwagenreservierungen im Gesamtwert von " + to_string(totalPrice) +
            " Euro eingelesen. Es wurden " + to_string(travelCount) + " Reisen und "
            + to_string(customerCount) + " Kunden angelegt. Der Kunde mit der ID 1 hat " +
            to_string(getTravelCount(1)) + " Reisen gebucht. Zur Reise mit der ID "
                                           "17 gehören " + to_string(
                    getBookingCount(17)) + " Buchungen."));
    msgBox.exec();
    return;


}

void TravelAgency::readIataCodes(string filePath) {
    ifstream file(filePath);
    static QMessageBox msgBox;

    if (!file) {

        msgBox.setWindowTitle("Fehler beim Einlesen der Iata-Codes");
        msgBox.setText("Datei konnte nicht gelesen werden");
        msgBox.exec();
        return;
    }
    json data;
    file >> data;
    std::string name{}, isoCountry{}, municipality{}, iataCode{};
    for(int i = 0; i < data.size(); i++){
        /*if (data.at(i)["name"].is_null() || data.at(i)["name"].empty() ||
            data.at(i)["name"].get<string>().length() == 0) {
            throw runtime_error("Leeres Attribut 'name' in Zeile " + to_string(i + 1));
        }
        if (data.at(i)["iso_country"].is_null() || data.at(i)["iso_country"].empty() ||
            data.at(i)["iso_country"].get<string>().length() == 0) {
            throw runtime_error("Leeres Attribut 'iso_country' in Zeile " + to_string(i + 1));
        }
        if (data.at(i)["municipality"].is_null() || data.at(i)["municipality"].empty() ||
            data.at(i)["municipality"].get<string>().length() == 0) {
            std::cout << data.at(i)["name"];
            throw runtime_error("Leeres Attribut 'municipality' in Zeile " + to_string(i + 1));
        }
        if (data.at(i)["iata_code"].is_null() || data.at(i)["iata_code"].empty() ||
            data.at(i)["iata_code"].get<string>().length() == 0) {
            throw runtime_error("Leeres Attribut 'iata_code' in Zeile " + to_string(i + 1));
        }
         */
        name = data.at(i)["name"];
        //std::cout << name << endl;
        isoCountry = data.at(i)["iso_country"];
        municipality = data.at(i)["municipality"];
        iataCode = data.at(i)["iata_code"];

        iataCodes[iataCode] = new Airport(name, isoCountry, municipality);

    }
}

Booking *TravelAgency::findBooking(std::string id) {
    for (Booking *b: allBookings) {
        if (b->getId() == id) {
            return b;
        }
    }
    return nullptr;
}

Travel *TravelAgency::findTravel(long id) {
    for (Travel *t: allTravels) {
        if (t->getId() == id) {
            return t;
        }
    }
    return nullptr;
}

Customer *TravelAgency::findCustomer(long id) {
    for (Customer *c: allCustomers) {
        if (c->getId() == id) {
            return c;
        }
    }
    return nullptr;
}

Airport *TravelAgency::getAirport(std::string iataCode) {
    return iataCodes[iataCode];
}


const vector<Booking *> &TravelAgency::getBookings() const {
    return allBookings;
}

int TravelAgency::getTravelCount(long customerId) {
    int travelCount{};
    for (auto t: allTravels) {
        if (t->getCustomerId() == customerId) {
            travelCount++;
        }
    }
    return travelCount;
}

int TravelAgency::getBookingCount(long travelId) {
    int bookingCount{};
    for (auto b: allBookings) {
        if (b->getTravelId() == travelId) {
            bookingCount++;
        }
    }
    return bookingCount;
}

const vector<Booking *> &TravelAgency::getAllBookings() const {
    return allBookings;
}

const vector<Customer *> &TravelAgency::getAllCustomers() const {
    return allCustomers;
}

const vector<Travel *> &TravelAgency::getAllTravels() const {
    return allTravels;
}

