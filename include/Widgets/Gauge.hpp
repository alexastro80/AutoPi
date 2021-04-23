#ifndef GAUGE_H
#define GAUGE_H

#include "indicator.h"
#include <QtWidgets>
class Gauge : public Indicator
{
public:
    Gauge(QObject* parent, int x, int y);
    void SetValue(double Value) override;
    void Draw() {  update(); }

    bool DrawAll = true;
    double Increment = 0.25;
    double ThetaStart = 180.0, ThetaEnd = 360.0;

    int LabelInterval = 1;
    int LabelSpace = 15;
    int Precision = 1;
    int RadiusIndicator= 70;
    int RadiusLabel = 80;
    int RadiusInner = 30, RadiusOuter = 50;
    int X = 0, Y = 0;
    QColor ForegroundColor = Qt::red,
        BackgroundColor = Qt::gray,
        LabelColor = Qt::black;
    QFont LableFont;
protected:
    void paintEvent(QPaintEvent *event);
private:
    double lastValue = 0.0;
};

#endif // GAUGE_H
