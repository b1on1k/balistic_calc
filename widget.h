#ifndef WIDGET_H
#define WIDGET_H

#endif // WIDGET_H


#include <QWidget>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
    void Change();

//private slots:



private:
    Ui::Widget *ui;
};

#endif // WIDGET_H

