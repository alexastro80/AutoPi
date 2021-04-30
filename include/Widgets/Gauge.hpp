#ifndef GAUGE_H
#define GAUGE_H

#include <Widgets/Indicator.hpp>
#include <Widgets/Paintable.hpp>
#include <QtWidgets>
class Gauge : public Indicator, public Paintable
{
public:
    Gauge(QObject* parent, int x, int y);
    void SetValue(double Value) override;
    void Paint(QPainter& painter) override;
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
    QColor LabelColor = Qt::black;
    QFont LableFont;
protected:
    void paintEvent(QPaintEvent *event) { QPainter painter(this); Paint(painter);}
private:
    double lastValue = 0.0;
};

#endif // GAUGE_H
