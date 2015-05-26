#include <QGuiApplication>
#include <QQuickView>
#include <QQmlEngine>
#include <myclass.h>
#include <QQmlContext>
#include <QQmlComponent>
#include <QObject>
#include <QQmlProperty>
#include "ballcalc.h"
#include <QDebug>
#include "auxiliary.h"

int main(int argc, char **argv)
{
    QGuiApplication app(argc, argv);

    QQuickView view;
    view.setSource(QUrl::fromLocalFile("c:/Qt/Tools/QtCreator/projects/maps/main.qml"));
    view.show();
    MyClass mCl;
    view.rootContext()->setContextProperty("mCl",&mCl);

    return app.exec();
}

