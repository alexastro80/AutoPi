/*
 * Setting.cpp
 *
 *  Created on: May 14, 2021
 *      Author: pi
 */

#include <App/setting.hpp>
#include <QHBoxLayout>
#include <QLabel>
#include <QString>
#include <iostream>
Setting::Setting(string _parent, string _name, ValueBase* valObj)
{
	parent = _parent;
	name = _name;
	value = valObj;
}

Setting::Setting(string _parent, string _name, const QObject* object, const char* slot, ValueBase* _value)
{
	parent = _parent;
	name = _name;
	value = _value;
	value->Connect(object, slot);
}


string Setting::ToCSV()
{

	string str = "";
	for (int i = 0; i < COLUMNS; i++)
	{
		if(i == CATEGORY) 		str += parent;
		else if (i == NAME) 	str += name;
		else if (i == TYPE) 	str += Type();
		else if (i == VALUE && value != nullptr)
		{
			str += value->toString();
		}
		str += ",";
	}
	return str;
}



void Setting::Modify(string csv[], int size)
{
	if ((parent.compare(csv[0]) == 0)
		&& (name.compare(csv[1]) == 0)
		&& (Type().compare(csv[2]) == 0))
	{
		ChangeVal(csv[3]);
	}
}

void Setting::initialize(QWidget* parent)
{
	item = new QWidget(parent);
	QHBoxLayout* layout = new QHBoxLayout(item);
	layout->setAlignment(Qt::AlignLeft);
	item->setLayout(layout);
	QLabel* label = new QLabel();
	label->setVisible(true);
	label->setText(QString::fromStdString(name + ":"));
//	label->setAlignment(Qt::AlignRight);
	layout->addWidget(label);
	if (value != nullptr) {
		QWidget* widget = value->Widget();
		if (widget != nullptr) {
			widget->setMaximumWidth(500);
			layout->addWidget(value->Widget(item));

		}
	}

}



void CSVLine(const string& line, string csv[]) {
	string tempLine = line;
	for(int i = 0; i < COLUMNS; i++) {
		int index = tempLine.find(",");
		if(index < 0) return;
		csv[i] = tempLine.substr(0, index);
	}
}

