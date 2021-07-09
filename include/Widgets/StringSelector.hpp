#ifndef STRINGSELECTOR_H
#define STRINGSELECTOR_H

#include <QComboBox>
#include <vector>
#include <string.h>
class StringSelector : public QComboBox
{
public:
    StringSelector();
    StringSelector(QWidget* parent = nullptr, std::vector<std::string> list = std::vector<std::string>());

    void addItem(const std::string item, const QVariant &userData = QVariant()) {
        QComboBox::addItem(QString::fromStdString(item), userData);
    }
};

#endif // STRINGSELECTOR_H
