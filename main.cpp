//
// Created by GBT B450M-S2H on 14.04.2023.
//

#include "TravelAgency.h"
#include <QApplication>
#include <QTranslator>
#include "TravelAgencyUI.h"
//#include "Test.h"

using namespace std;

int main(int argc, char *argv[]) {


    TravelAgency travelAgency;
    //Test t(&travelAgency);
    //QTest::qExec(&t);

    QApplication a(argc, argv);
    TravelAgencyUI tu (std::make_shared<TravelAgency>(travelAgency));
    tu.show();
    return a.exec();


}
