#ifndef MYCLASS_H
#define MYCLASS_H

#include <QObject>


class MyClass : public QObject
{
    Q_OBJECT

public:
    explicit MyClass(QObject *parent = 0);
    Q_INVOKABLE void run();
    Q_INVOKABLE double getResA(double hM,double deltaHM,double deltaTM,double hVP,double TZ,double deltaV0, double dist,
                               double lat1, double lon1,double lat2,double lon2);
    Q_INVOKABLE double getResB(double hM,double deltaHM,double deltaTM,double hVP,double TZ,double deltaV0, double dist,
                               double lat1, double lon1,double lat2,double lon2);
    double getAngle(double lat1, double lon1,double lat2,double lon2, bool north);
public slots:
    QString getStr();
private:
    QString str;

};
#endif // MYCLASS_H
