#include <Widgets/Gauge.hpp>
#include <QObject>
Gauge::Gauge(QObject* parent, int x, int y) : Indicator(parent)
{
    X = x;
    Y = y;
}

inline void Gauge::SetValue(double Value)
{
    value = Value;
    bool tempDrawAll = DrawAll;
    DrawAll = false;
    update();
    DrawAll = tempDrawAll;
}

void Gauge::Paint(QPainter& painter)
{

    double pi = 3.1415926;
    int flip = (ThetaStart < ThetaEnd) ? 1 : -1;
    if (DrawAll) {
        painter.setPen(QPen(LabelColor, 3, Qt::SolidLine, Qt::RoundCap));
        double conversionValue = abs(Max - Min)/LabelInterval;
        double conversionTheta = abs(ThetaStart - ThetaEnd)/LabelInterval;
        int radius = RadiusIndicator + LabelSpace;

        for(int i = 0; i <= LabelInterval; i++)
        {
            double value = conversionValue * i + Min;
            double theta = conversionTheta * i * flip + ThetaStart;
            if (i == 0) theta -= (Increment * flip * 10);
            else if (i == LabelInterval) theta += (Increment * flip * 10);
            theta *= pi/180;
            //Draw Line
            int radiusIndicator = (i == 0 || i == LabelInterval) ? RadiusInner : RadiusOuter;
            int x1 = (radiusIndicator) * cos(theta) + X;
            int y1 = (radiusIndicator) * sin(theta) + Y;
            int x2 = (RadiusIndicator) * cos(theta) + X;
            int y2 = (RadiusIndicator) * sin(theta) + Y;
            painter.drawLine(x1, y1, x2, y2);
            //Draw Label
            double characterHeight = 5;
            double characterWidth = 3;
            double decimalWidth = (Precision > 0) ? 2 : 0;
            double x3 = radius * cos(theta) + X;
            x3 -= (characterWidth * (value/10.0 + Precision + 1) + decimalWidth)/2 ;
            double y3 = radius * sin(theta) + Y;
            y3 += (characterHeight/2);
            painter.drawText(x3, y3, QString::number(value, 'f', Precision));
        }
    }
    painter.setPen(QPen(ForegroundColor, 3, Qt::SolidLine, Qt::RoundCap));
    double conversion = abs((ThetaEnd - ThetaStart)/(Max - Min));
    double newTheta = conversion * value * flip + ThetaStart;

    for(double theta = ThetaStart; (theta * flip) < (newTheta * flip); theta += (Increment * flip))
    {
        double thetaRad = theta*pi/180;
        int X1 = RadiusOuter*cos(thetaRad) + X;
        int Y1 = RadiusOuter*sin(thetaRad) + Y;
        int X2 = RadiusInner*cos(thetaRad) + X;
        int Y2 = RadiusInner*sin(thetaRad) + Y;
        painter.drawLine(X1, Y1, X2, Y2);
    }
    painter.setPen(QPen(BackgroundColor, 3, Qt::SolidLine, Qt::RoundCap));
    for(double theta = newTheta; (theta * flip) < (ThetaEnd * flip); theta += (Increment * flip))
    {
        double thetaRad = theta*pi/180;
        int X1 = RadiusOuter*cos(thetaRad) + X;
        int Y1 = RadiusOuter*sin(thetaRad) + Y;
        int X2 = RadiusInner*cos(thetaRad) + X;
        int Y2 = RadiusInner*sin(thetaRad) + Y;
        painter.drawLine(X1, Y1, X2, Y2);
    }
}
