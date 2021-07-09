#ifndef PAINTABLE_HPP
#define PAINTABLE_HPP

#include <QObject>
#include <QtWidgets>
class Paintable : public QObject
{
    Q_OBJECT
public:
    explicit Paintable(QObject *parent = nullptr){}
    virtual void Paint(QPainter& painter) = 0;

protected:
    virtual void paintEvent(QPaintEvent *event) = 0;

};

#endif // PAINTABLE_HPP
