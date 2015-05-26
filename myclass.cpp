#include "myclass.h"
#include <QQuickView>
#include <QObject>
#include <QDebug>
#include <QQmlEngine>
#include <QQmlComponent>
#include "ballcalc.h"
#include <string>
#include "auxiliary.h"

MyClass::MyClass(QObject *parent) :
    QObject(parent)
{
    str = "sssss";
}

void MyClass::run()
{
//    QQuickView view;
//    view.setSource(QUrl::fromLocalFile("c:/Qt/Tools/QtCreator/projects/maps/main.qml"));
//    view.show();
//    QObject *object = (QObject*) (view.rootObject());
//    getRes(object);
}

QString MyClass::getStr()
{
    return QString("asdasda");
}

double MyClass::getAngle(double lat1, double lon1,double lat2,double lon2, bool north)
{
 //*_angle = 1800.0;
 //*_dist = 10550.0;

 double P, M;

 P = 111111.0 * fabs(lat1 - lat2);   // відстань між широтами
 M = 111111.0 * fabs(lon1 - lon2) * cos(lat1 * 0.0175); // відстань між довготами

 return atan(M/P) / 0.001047;

 {
  double P, M;

  P = 111111.0 * fabs(lat1 - lat2);      // відстань між широтами
  M = 111111.0 * fabs(lon1 - lon2) * cos(lat1 * 0.0175); // відстань між довготами

  if (lat2 - lat1 >= 0 && lon2 - lon1 >= 0)
  {
   return  (north) ? (atan(M/P) / 0.001047) : (atan(P/M) / 0.001047 + 1500.0);
  }
  else
  if (lat2 - lat1 < 0 && lon2 - lon1 >= 0)
  {
   return  (north) ? (atan(P/M) / 0.001047 + 1500.0) : (atan(M/P) / 0.001047);
  }
  else
  if (lat2 - lat1 < 0 && lon2 - lon1 < 0)
  {
   return (north) ? (atan(M/P) / 0.001047 + 3000.0) : (atan(P/M) / 0.001047 + 4500.0);
  }
  else
  if (lat2 - lat1 >= 0 && lon2 - lon1 < 0)
  {
   return (north) ? (atan(P/M) / 0.001047 + 4500.0) : (atan(M/P) / 0.001047 + 3000.0);
  }
 }

}

double MyClass:: getResA(double hM,double deltaHM,double deltaTM,double hVP,double TZ,double deltaV0,
                         double dist, double lat1, double lon1,double lat2,double lon2)
{
//    double dist = 10550.0;	// відстань до цілі
//    double angle = 1800.0;	// напрямок на ціль
    double angle = getAngle(lat1,lon1,lat2,lon2,true);
    qDebug()<<angle<<endl;
    double a, b, c;

    // зчитка таблиць
    TABLE* t = readTable("c:/Qt/Tools/QtCreator/projects/libs/table2.txt", 104, 16);
    TABLE* m = readTable("c:/Qt/Tools/QtCreator/projects/libs/meteo2.txt", 10, 5);

//    QObject *hM = obj->findChild<QObject*>("hM");
//    QObject *deltaHM = obj->findChild<QObject*>("deltaHM");
//    QObject *deltaTM = obj->findChild<QObject*>("deltaTM");
//    QObject *hVP = obj->findChild<QObject*>("hVP");
//    QObject *TZ = obj->findChild<QObject*>("TZ");
//    QObject *deltaV0 = obj->findChild<QObject*>("deltaV0");
//    QObject *distance = obj->findChild<QObject*>("dist");
//    QObject *ar1 = obj->findChild<QObject*>("area1");
//    QObject *ar2 = obj->findChild<QObject*>("area2");

//    BALLPARMS bp;
//    bp.hM = 150.0;
//    bp.deltaHM = 12.0;
//    bp.deltaTM = -33.0;
//    bp.hVP = 110.0;
//    bp.TZ = -18.0;
//    bp.deltaV0 = -1.2;

    BALLPARMS bp;
    bp.hM = hM;
    bp.deltaHM = deltaHM;
    bp.deltaTM = deltaTM;
    bp.hVP = hVP;
    bp.TZ = TZ;
    bp.deltaV0 = deltaV0;

//    double dist = distance->property("text").toDouble();	// відстань до цілі
//    bp.hM = hM->property("text").toDouble();
//    bp.deltaHM = deltaHM->property("text").toDouble();
//    bp.deltaTM = deltaTM->property("text").toDouble();
//    bp.hVP = hVP->property("text").toDouble();
//    bp.TZ = TZ->property("text").toDouble();
//    bp.deltaV0 = deltaV0->property("text").toDouble();


    BALLCALC* bc = new BALLCALC(bp);


        bc->setTS(t, 75);	// 75 - використовуються перші 75 рядків
                            // (кількість рядків з нахилом до 45 градусів (750 тис.) у відповідній таблиці)
        bc->setMeteo(m);

        // межі обириються згідно зі значеннями таблиць
        bc->setGrpDist(600.0, 14000.0, 100.0);

        bc->setPCoords(angle, dist);


        if (!bc->getGrpCorrection(&a, &b, &c))
        {
            printf("Impossible to count corrections.");
//            ar1->setProperty("text","Impossible to count corrections.");
        }
//        else
//        {
////            QString str = " _alphaOHCor = %-20f \n _dCor = %-20f ",a,b;
//            ar1->setProperty("text",(a));
//            ar2->setProperty("text",(b));
////            bc->getInitPCoords(&d, &e);
////            printf("%-20f %-20f %-20f", a + d, b, c + e);
////            printf("\n");
//        }

        delete bc;
        return a;
}

double MyClass:: getResB(double hM,double deltaHM,double deltaTM,double hVP,double TZ,double deltaV0,
                         double dist, double lat1, double lon1,double lat2,double lon2)
{
//    double dist = 10550.0;	// відстань до цілі
//    double angle = 1800.0;	// напрямок на ціль
    double angle = getAngle(lat1,lon1,lat2,lon2,true);
    qDebug()<<angle<<endl;
    double a, b, c;

    // зчитка таблиць
    TABLE* t = readTable("c:/Qt/Tools/QtCreator/projects/libs/table2.txt", 104, 16);
    TABLE* m = readTable("c:/Qt/Tools/QtCreator/projects/libs/meteo2.txt", 10, 5);

//    QObject *hM = obj->findChild<QObject*>("hM");
//    QObject *deltaHM = obj->findChild<QObject*>("deltaHM");
//    QObject *deltaTM = obj->findChild<QObject*>("deltaTM");
//    QObject *hVP = obj->findChild<QObject*>("hVP");
//    QObject *TZ = obj->findChild<QObject*>("TZ");
//    QObject *deltaV0 = obj->findChild<QObject*>("deltaV0");
//    QObject *distance = obj->findChild<QObject*>("dist");
//    QObject *ar1 = obj->findChild<QObject*>("area1");
//    QObject *ar2 = obj->findChild<QObject*>("area2");

//    BALLPARMS bp;
//    bp.hM = 150.0;
//    bp.deltaHM = 12.0;
//    bp.deltaTM = -33.0;
//    bp.hVP = 110.0;
//    bp.TZ = -18.0;
//    bp.deltaV0 = -1.2;


    BALLPARMS bp;
    bp.hM = hM;
    bp.deltaHM = deltaHM;
    bp.deltaTM = deltaTM;
    bp.hVP = hVP;
    bp.TZ = TZ;
    bp.deltaV0 = deltaV0;

//    double dist = distance->property("text").toDouble();	// відстань до цілі
//    bp.hM = hM->property("text").toDouble();
//    bp.deltaHM = deltaHM->property("text").toDouble();
//    bp.deltaTM = deltaTM->property("text").toDouble();
//    bp.hVP = hVP->property("text").toDouble();
//    bp.TZ = TZ->property("text").toDouble();
//    bp.deltaV0 = deltaV0->property("text").toDouble();


    BALLCALC* bc = new BALLCALC(bp);


        bc->setTS(t, 75);	// 75 - використовуються перші 75 рядків
                            // (кількість рядків з нахилом до 45 градусів (750 тис.) у відповідній таблиці)
        bc->setMeteo(m);

        // межі обириються згідно зі значеннями таблиць
        bc->setGrpDist(600.0, 14000.0, 100.0);

        bc->setPCoords(angle, dist);

        if (!bc->getGrpCorrection(&a, &b, &c))
        {
            printf("Impossible to count corrections.");
//            ar1->setProperty("text","Impossible to count corrections.");
        }
//        else
//        {
////            QString str = " _alphaOHCor = %-20f \n _dCor = %-20f ",a,b;
//            ar1->setProperty("text",(a));
//            ar2->setProperty("text",(b));
////            bc->getInitPCoords(&d, &e);
////            printf("%-20f %-20f %-20f", a + d, b, c + e);
////            printf("\n");
//        }

        delete bc;
        return b;
}
