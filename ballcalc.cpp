#include "ballcalc.h"

static const int _InterpolatedCollsTS[] = {1, 4, 5, 6, 7, 8, 9, 10, 14};	// номери стовбц≥в таблиц≥ TS, значенн€ €ких необх≥дн≥ дл€ ≥нтерпол€ц≥њ
static const int _InterpolatedCollsMeteo[] = {2, 3, 4};						// номери стовбц≥в таблиц≥ Meteo, значенн€ €ких необх≥дн≥ дл€ ≥нтерпол€ц≥њ
static const int _sizeInterpolatedCollsTS = 9;								// розм≥рн≥сть _InterpolatedCollsTS
static const int _sizeInterpolatedCollsMeteo = 3;							// розм≥рн≥сть _sizeInterpolatedCollsMeteo

/*
 *	class TABLE	********************************************************************
*/

TABLE::TABLE(double** _pTable, int _n, int _m)
{
	pTable = (const double**)_pTable;
	n = _n;
	m = _m;
}

TABLE::~TABLE()
{
	if (pTable)
	{
		for (int i = 0; i < n; i++)
		{
			delete[] pTable[i];
		}
		delete[] pTable;
	}
}


const double** TABLE::getTable()
{
	return pTable;
}

double TABLE::getCell(int i, int j)
{
	return pTable[i][j];
}

int TABLE::getN()
{
	return n;
}

int TABLE::getM()
{
	return m;
}

/*
 *	!class TABLE	********************************************************************
*/


/*
 *	class BALLCALC	********************************************************************
*/

int BALLCALC::linearInterpolation(double* _P,
								  const double** tab,
								  int collX,
								  int collY,
								  int max,
								  double val)
{
	int i;

	if (val < tab[0][collX] || val > tab[max - 1][collX])
	{
		return 0;
	}

	for (i = 0; tab[i][collX] < val; i++) ;

	if (tab[i][collX] == val)
		*_P = tab[i][collY];
	else
		*_P = (val - tab[i - 1][collX]) * (tab[i][collY] - tab[i - 1][collY]) / (tab[i][collX] - tab[i - 1][collX]) + tab[i - 1][collY];

	return 1;
}


/*
	calcCorrection - п≥драховуЇ поправки на напр€мок ≥ дальн≥сть 
	дл€ кожного вузла ≥нтерпол€ц≥њ таблиц≥ GRP
		II			- кут нахилу в тис€чних 
		Z			- деривац≥€ в тис€чних
		deltaZW		- поправка на боковий в≥тер
		deltaXW		- поправка на повздовжний в≥тер
		deltaXH		- поправка на атмосферний тиск
		deltaXT		- поправка на атмосферну температуру
		deltaXTZ	- поправка на температуру зар€да
		deltaXV0	- поправка на початкову швидк≥сть
		YB			- висота входу в бюлетень в метрах

		MT			- темепартура на висот≥ YB
		alphaW		- напр€мок в≥тру в тис€чних
		W			- швидк≥сть в≥тру в м/c

		deltaXM		- поправка на в≥дстань метереолог≥чна
		deltaXB		- поправка на в≥дстань бал≥стична
*/

int BALLCALC::calcCorrection(double dist, double* _dCor, double* _alphaOH)
{
	if (TS == NULL || Meteo == NULL) return 0;


	const double** tab = TS->getTable();
	double II, Z, deltaZW, deltaXW, deltaXH, deltaXT, deltaXTZ, deltaXV0, YB;
	
	if (!linearInterpolation(&II, tab, 0, InterpolatedColsTS[0], max, dist)) return 0;
	if (!linearInterpolation(&Z, tab, 0, InterpolatedColsTS[1], max, dist)) return 0;
	if (!linearInterpolation(&deltaZW, tab, 0, InterpolatedColsTS[2], max, dist)) return 0;
	if (!linearInterpolation(&deltaXW, tab, 0, InterpolatedColsTS[3], max, dist)) return 0;
	if (!linearInterpolation(&deltaXH, tab, 0, InterpolatedColsTS[4], max, dist)) return 0;
	if (!linearInterpolation(&deltaXT, tab, 0, InterpolatedColsTS[5], max, dist)) return 0;
	if (!linearInterpolation(&deltaXTZ, tab, 0, InterpolatedColsTS[6], max, dist)) return 0;
	if (!linearInterpolation(&deltaXV0, tab, 0, InterpolatedColsTS[7], max, dist)) return 0;
	if (!linearInterpolation(&YB, tab, 0, InterpolatedColsTS[8], max, dist)) return 0;
	
	tab = Meteo->getTable();
	double MT, alphaW, W;

	int maxMeteo = Meteo->getN();
	
	if (!linearInterpolation(&MT, tab, 0, InterpolatedColsMeteo[0], maxMeteo, YB)) return 0;
	if (!linearInterpolation(&alphaW, tab, 0, InterpolatedColsMeteo[1], maxMeteo, YB)) return 0;
	if (!linearInterpolation(&W, tab, 0, InterpolatedColsMeteo[2], maxMeteo, YB)) return 0;
		
	double WX = -1.0 * W * cos((alphaOH - alphaW) * Pi / 3000.0);
	double WZ = W * sin((alphaOH - alphaW) * Pi /3000.0);
	double deltaHVP = deltaHM + (hM - hVP) / 10.0;
	double deltaXM = 0.1 * deltaXH * deltaHVP - 0.1 * deltaXT * MT - 0.1 * deltaXW * WX;
	double deltaXB = -0.1 * deltaXTZ * (TZ - 15.0) - deltaXV0 * deltaV0;

	*_dCor = deltaXM + deltaXB;

	double deltaD = -1.0 * Z - 0.1* deltaZW * WZ;

	*_alphaOH = deltaD;
	
	return 1;
}

/*
	calcGRP - будуЇ таблицю GRP
*/
int BALLCALC::calcGRP()
{
	if (maxDist <= minDist || step <= 0.0 || minDist <= 0.0) return 0;


	double delta = maxDist - minDist;

	int N = 0;
	for (double d = 0.0; d < delta; N++, d += step) ;
	N++;


	double** tmp = new double*[N];
	for (int i = 0; i < N; i++) tmp[i] = new double[3];

	GRP = new TABLE(tmp, N, 3);


	delta = minDist;

	for (int i = 0; i < N; i++, delta += step)
	{
		if (i == N - 1) delta = maxDist;
		
		if (!calcCorrection(delta, &tmp[i][1], &tmp[i][2]))
		{
			delete GRP;
			GRP = NULL;
			return 0;
		}

		tmp[i][0] = delta;
	}

	return 1;
}

/*
	calcGRPCorrection - обчислюЇ поправки дл€ заданих параметр≥в 
	alphaOH, dTop на основ≥ таблиц≥ GRP
*/
int BALLCALC::calcGRPCorrection()
{
	if (!GRP) return 0;

	int n = GRP->getN();
	int m = GRP->getM();
	
	double** tmp = new double*[n];
	for (int i = 0; i < n; i++)
	{
		tmp[i] = new double[m];
	}

	TABLE* tmpTable = new TABLE(tmp, n, m);

	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < m; j++)
		{
			if (j == 0)
				tmp[i][j] = GRP->getCell(i, 0) - GRP->getCell(i, 1);
			else
				tmp[i][j] = GRP->getCell(i, j);
		}
	}

	if (!linearInterpolation(&dCor, (const double**)tmp, 0, 1, n, dTop)) return 0;
	if (!linearInterpolation(&alphaOHCor, (const double**)tmp, 0, 2, n, dTop)) return 0;
	if (!linearInterpolation(&gunAngleCor, TS->getTable(), 0, 1, max, dCor + dTop)) return 0;

	delete tmpTable;

	return 1;
}




BALLCALC::BALLCALC()
{
	InterpolatedColsTS = _InterpolatedCollsTS;
	InterpolatedColsMeteo = _InterpolatedCollsMeteo;

	sizeInterpolatedCollsTS = _sizeInterpolatedCollsTS;
	sizeInterpolatedCollsMeteo = _sizeInterpolatedCollsMeteo;

	TS = Meteo = GRP = NULL;
	
	max = 0;

	minDist = maxDist = step = 0.0;

	hM = deltaHM = deltaTM = 0.0;
	hVP = TZ = deltaV0 = 0.0;

	alphaOH = dTop = 0.0;
	alphaOHCor = gunAngleCor = dCor = 0.0;

	changed = changedGRP = true;
}

BALLCALC::BALLCALC(BALLPARMS _bp)
{
	InterpolatedColsTS = _InterpolatedCollsTS;
	InterpolatedColsMeteo = _InterpolatedCollsMeteo;

	sizeInterpolatedCollsTS = _sizeInterpolatedCollsTS;
	sizeInterpolatedCollsMeteo = _sizeInterpolatedCollsMeteo;

	TS = Meteo = GRP = NULL;
	
	max = 0;

	minDist = maxDist = step = 0.0;

	hM = _bp.hM;
	deltaHM = _bp.deltaHM;
	deltaTM = _bp.deltaTM;
	hVP = _bp.hVP;
	TZ = _bp.TZ;
	deltaV0 = _bp.deltaV0;

	alphaOH = dTop = 0.0;
	alphaOHCor = gunAngleCor = dCor = 0.0;

	changed = changedGRP = true;
}

BALLCALC::~BALLCALC()
{
	if (TS) delete TS;
	if (Meteo) delete Meteo;
	if (GRP) delete GRP;
}


void BALLCALC::setTS(TABLE* _TS)
{
	TS = _TS;
	max = TS->getN();

	changedGRP = true;
}

void BALLCALC::setTS(TABLE* _TS, int rows)
{
	TS = _TS;
	max = (rows < _TS->getN()) ? rows : _TS->getN();

	changedGRP = true;
}

TABLE* BALLCALC::getTS()
{
	return TS;
}


void BALLCALC::setMeteo(TABLE* _Meteo)
{
	Meteo = _Meteo;

	changedGRP = true;
}

TABLE* BALLCALC::getMeteo()
{
	return Meteo;
}


TABLE* BALLCALC::getGRP()
{
	if (changedGRP)
	{
		changedGRP = false;

		if (!calcGRP())
		{
			return NULL;
		}
	}

	return GRP;
}

void BALLCALC::setGrpDist(double _minDist, double _maxDist, double _step)
{
	minDist =_minDist;
	maxDist = _maxDist;
	step = _step;

	changedGRP = true;
}

void BALLCALC::setMinDist(double _minDist)
{
	minDist =_minDist;

	changedGRP = true;
}

void BALLCALC::setMaxDist(double _maxDist)
{
	maxDist = _maxDist;

	changedGRP = true;
}

void BALLCALC::setStep(double _step)
{
	step = _step;

	changedGRP = true;
}

double BALLCALC::getMinDist()
{
	return minDist;
}

double BALLCALC::getMaxDist()
{
	return maxDist;
}

double BALLCALC::getStep()
{
	return step;
}


const int* BALLCALC::getInterpolatedColsTS()
{
	return InterpolatedColsTS;
}

const int* BALLCALC::getInterpolatedColsMeteo()
{
	return InterpolatedColsMeteo;
}

int BALLCALC::getInterpolatedCollsTsSize()
{
	return sizeInterpolatedCollsTS;
}

int BALLCALC::getInterpolatedCollsMeteoSize()
{
	return sizeInterpolatedCollsMeteo;
}


void BALLCALC::setBallParms(double _hM,
							double _deltaHM,
							double _deltaTM,
							double _hVP,
							double _TZ,
							double _deltaV0)
{
	hM = _hM;
	deltaHM = _deltaHM;
	deltaTM = _deltaTM;
	hVP = _hVP;
	TZ = _TZ;
	deltaV0 = _deltaV0;

	changedGRP = true;
}

void BALLCALC::setBallParms(BALLPARMS _bp)
{
	hM = _bp.hM;
	deltaHM = _bp.deltaHM;
	deltaTM = _bp.deltaTM;
	hVP = _bp.hVP;
	TZ = _bp.TZ;
	deltaV0 = _bp.deltaV0;

	changedGRP = true;
}

BALLPARMS BALLCALC::getBallParms()
{
	BALLPARMS res;
	res.deltaHM = deltaHM;
	res.deltaTM = deltaTM;
	res.deltaV0 = deltaV0;
	res.hM = hM;
	res.hVP = hVP;
	res.TZ = TZ;

	return res;
}


void BALLCALC::setPCoords(double _alphaOH, double _dTop)
{
	alphaOH = _alphaOH;
	dTop = _dTop;

	changedGRP = true;
}

void BALLCALC::setAlphaOH(double _alphaOH)
{
	alphaOH = _alphaOH;

	changedGRP = true;
}

void BALLCALC::setDTop(double _dTop)
{
	dTop = _dTop;

	changed = true;
}

void BALLCALC::getInitPCoords(double* _alphaOH, double* _dTop)
{
	*_alphaOH = alphaOH;
	*_dTop = dTop;
}


int BALLCALC::getGrpCorrection(double* _alphaOHCor, double* _dCor)
{
	if (!getGRP()) return 0;
	
	if (changed)
	{
		changed = false;

		if (!calcGRPCorrection())
		{
			return 0;
		}
	}
	
	*_alphaOHCor = alphaOHCor;
	*_dCor = dCor;

	return 1;
}

int BALLCALC::getGrpCorrection(double* _alphaOHCor, double* _gunAngleCor, double* _dCor)
{
	if (!getGRP()) return 0;
	
	if (changed)
	{
		changed = false;

		if (!calcGRPCorrection())
		{
			return 0;
		}
	}

	*_alphaOHCor = alphaOHCor;
	*_gunAngleCor = gunAngleCor;
	*_dCor = dCor;

	return 1;
}

int BALLCALC::getGrpRightPCoords(double* direction, double* distance)//, double dist)
{
	if (!getGrpCorrection(direction, distance))
	{
		return 0;
	}

	*direction += alphaOH;
	*distance += dTop;

	return 1;
}

/*
 *	!class BALLCALC	********************************************************************
*/