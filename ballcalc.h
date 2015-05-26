#ifndef __BALLCALC_H
#define __BALLCALC_H

#include <stdio.h>
#include <math.h>

#define Pi 3.1415926535897932384626433832795


/*
    BALLPARMS - структура для збереження
    параметрів стрільби та метеопараметрів:
        hM			- висота метеостанції (АМС) над рівнем моря
        deltaHM		- відхилення (від 750 мм.рт.ст.) наземного тиску на висоті АМС
        deltaTM		- відхилення наземної віртуальної температури на рівні АМС
        hVP			- висота вогневої позиції в метрах
        TZ			- температура заряда в град. Цельсія
        deltaV0		- відхилення початкової швидкості в %
*/

struct BALLPARMS
{
    double hM, deltaHM, deltaTM;
    double hVP, TZ, deltaV0;
};


/*
    TABLE - клас для збереження даних таблиці
*/

class TABLE
{
    private:
        const double** pTable;
        int n, m;

    public:
        TABLE(double** _pTable, int _n, int _m);
        ~TABLE();

        const double** getTable();
        double getCell(int i, int j);
        int getN();
        int getM();
};


/*
    BALLCALC - клас для обчислень корекції нахилу ствола,
    напрямку та дальності
        TS			- таблиця стрільби
        Meteo		- таблиця метеоумов:
                      [[висота в метрах, відхілення густини повітря, напрямок вітру в тисячних, швидкість вітру в м/c],
                       [...] ,
                       [...]]
        GRP			- таблиця відхилень:
                      [[дальність при нормальних умовах, поправка на дальність в метрах, поправка на напрямок в тисячних],
                       [...] ,
                       [...]]
        minDist		- ліва границя проміжку інтерполяції
        maxDist		- права границя проміжку інтерполяції
        step		- крок між вузлами інтерполяції
        hM			- висота метеостанції (АМС) на рівнем моря
        deltaHM		- відхилення (від 750 мм.рт.ст.) наземного тиску на висоті АМС
        deltaTM		- відхилення наземної віртуальної температури на рівні АМС
        hVP			- висота вогневої позиції в метрах
        TZ			- температура заряда в град. Цельсія
        deltaV0		- відхилення початкової швидкості в %
        alphaOH		- основний напрямок в тисячних
        dTop		- топографічна відстань до цілі
        alphaOHCor	- поправка на напрямок
        dCor		- поправка на дальність
        gunAngleCor	- поправка на нахил ствола
*/

class BALLCALC
{
    private:
        TABLE* TS;
        TABLE* Meteo;
        TABLE* GRP;

        int max;

        const int* InterpolatedColsTS;
        const int* InterpolatedColsMeteo;
        int sizeInterpolatedCollsTS;
        int sizeInterpolatedCollsMeteo;

        double minDist, maxDist, step;

        double hM, deltaHM, deltaTM;
        double hVP, TZ, deltaV0;

        double alphaOH, dTop;
        double alphaOHCor, dCor, gunAngleCor;

        bool changed, changedGRP;	// прапорці контролю зміни стану об'єкта


        int linearInterpolation(double* _P,
                                const double** tab,
                                int collX,
                                int collY,
                                int max,
                                double val);
        int calcCorrection(double dist, double* _dCor, double* _alphaOH);
        int calcGRP();
        int calcGRPCorrection();

    public:
        BALLCALC();
        BALLCALC(BALLPARMS _bp);
        ~BALLCALC();


        void setTS(TABLE* _TS);
        void setTS(TABLE* _TS, int rows);
        TABLE* getTS();


        void setMeteo(TABLE* _Meteo);
        TABLE* getMeteo();


        TABLE* getGRP();
        void setGrpDist(double _minDist, double _maxDist, double _step);
        void setMinDist(double _minDist);
        void setMaxDist(double _maxDist);
        void setStep(double _step);
        double getMinDist();
        double getMaxDist();
        double getStep();


        const int* getInterpolatedColsTS();
        const int* getInterpolatedColsMeteo();
        int getInterpolatedCollsTsSize();
        int getInterpolatedCollsMeteoSize();


        void setBallParms(double _hM,
                          double _deltaHM,
                          double _deltaTM,
                          double _hVP,
                          double _TZ,
                          double _deltaV0);
        void setBallParms(BALLPARMS _bp);
        BALLPARMS getBallParms();


        void setPCoords(double _alphaOH, double _dTop);
        void setAlphaOH(double _alphaOH);
        void setDTop(double _dTop);
        void getInitPCoords(double* _alphaOH, double* _dTop);


        int getGrpCorrection(double* _alphaOHCor, double* _dCor);
        int getGrpCorrection(double* _alphaOHCor, double* _gunAngleCor, double* _dCor);
        int getGrpRightPCoords(double* direction, double* distance);
};
#endif __BALLCALC_H
