#ifndef NUMBERSELECTOR_H
#define NUMBERSELECTOR_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QGridLayout>
class NumberSelector : public QWidget
{
    Q_OBJECT
public:
    NumberSelector(QWidget* parent = nullptr, int defaultValue = 0);
    int base = 10;
public slots:
    void increment() {
        value++;
        emit valueChanged(value);
        if (label != nullptr) label->setText(QString::number(value));
    }
    void decrement() { value--; emit valueChanged(value); if (label != nullptr) label->setText(QString::number(value));}
    void setValue(int x) {
    	value = x;
    	if (label != nullptr)
    		label->setText(QString::number(x, base));
    	emit valueChanged(value);
    }


private:
    int value;
    QLabel *label;
signals:
    void valueChanged(int);



};

#endif // NUMBERSELECTOR_H
