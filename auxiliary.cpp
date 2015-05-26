#include "auxiliary.h"
#include <QDebug>

FILE* openFile(const char* fName);
int readDouble(double* cell, FILE* f);


TABLE* readTable(const char* fName, int n, int m)
{
    TABLE* table = NULL;

    FILE* f = openFile(fName);

    double** tmp;
    double d = 0;

    if (f)
    {
        tmp = new double*[n];
        for (int i = 0; i < n; i++)
        {
            tmp[i] = new double[m];
        }

        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < m; j++)
            {
                if (!readDouble(&d, f))
                {
                    tmp[i][j] = d;
                    break;
                }
                tmp[i][j] = d;
                d = 0;
            }
        }

        table = new TABLE(tmp, n, m);
        fclose(f);
    }
	
    return table;
}

void showTable(TABLE* table)
{
    for (int i = 0; i < table->getN(); i++)
    {
        for (int j = 0; j < table->getM(); j++)
        {
            printf("%f\t", table->getCell(i, j));
//            qDebug()<<table->getCell(i, j);
        }
        //printf("\n");
    }
}


FILE* openFile(const char* fName)
{
    FILE *f;
    errno_t err;
    err = fopen_s(&f,fName, "r");
    if (err != 0)
    {
        return NULL;
    }
    return f;
}

int readDouble(double* cell, FILE* f)
{
    int res = 0;
    char ch;
    char val[32];
    int pos = 0;

    if (!feof(f))
    {
        do {
            ch = fgetc(f);
        }
        while (ch < 33 && !feof(f));

        if (ch < 33) return res;

        do {
            val[pos] = ch;
            ch = fgetc(f);
            pos++;
        }
        while (ch >= 33 && !feof(f) && pos < 31);

        if (ch >= 33)
        {
            if (pos < 31)
            {
                val[pos] = ch;
                pos++;
                res = 0;
            }
            else
            {
                while (ch >= 33)
                {
                    ch = fgetc(f);
                    if (feof(f))
                    {
                        res = 0;
                        break;
                    }
                }
                res = 1;
            }
        }
        else
        {
            res = 1;
        }

        val[pos] = '\0';
        *cell = atof(val);
    }

    return res;
}
