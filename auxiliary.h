#ifndef _AUXILIARY_H
#define _AUXILIARY_H

#include <stdio.h>
#include <conio.h>
#include <Windows.h>

#include "ballcalc.h"


/*
    readTable - зчитуЇ дан≥ таблиц≥ ≥з текстового файлу

    ¬х≥дн≥ параметри:
        const char* fName	- ≥м'¤ текстового файлу
        int n				- к≥льк≥сть р¤дк≥в таблиц≥, що створюЇтьс¤
        int m				- к≥льк≥сть стовбц≥в таблиц≥, що створюЇтьс¤

    «наченн¤, що повертаЇтьс¤:
        ¬каз≥вник на динам≥чно створений об'Їкт TABLE
*/

TABLE* readTable(const char* fName, int n, int m);


/*
    showTable - виводить зм≥ст об'Їкта типу TABLE у в≥кно консол≥

    ¬х≥дн≥ параметри:
        TABLE* table		- таблиц¤, зм≥ст ¤коњ необх≥дно в≥добразити
*/

void showTable(TABLE* table);

FILE* openFile(const char* fName);
int readDouble(double* cell, FILE* f);

#endif _AUXILIARY_H
