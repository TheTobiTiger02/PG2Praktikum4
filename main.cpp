//
// Created by GBT B450M-S2H on 14.04.2023.
//

#include "TravelAgency.h"
#include <QApplication>
#include <QTranslator>
#include "TravelAgencyUI.h"

using namespace std;

int main(int argc, char *argv[]) {
    TravelAgency travelAgency;

    QApplication a(argc, argv);
    TravelAgencyUI tu(&travelAgency);
    tu.show();
    return a.exec();


}
