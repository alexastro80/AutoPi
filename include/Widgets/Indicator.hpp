#ifndef INDICATOR_H
#define INDICATOR_H

#include <QWidget>

class Indicator : public QWidget
{
public:
    Indicator(QObject* parent) { QObject::setParent(parent);}

    virtual void SetValue(double value) = 0;
    double Max;
    double Min;
    QColor ForegroundColor;
    QColor BackgroundColor;


protected:
    double value;

};

#endif // INDICATOR_H
