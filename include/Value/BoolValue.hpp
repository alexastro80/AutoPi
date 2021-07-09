/*
 * BoolValue.hpp
 *
 *  Created on: Jul 9, 2021
 *      Author: pi
 */

#ifndef INCLUDE_VALUE_BOOLVALUE_HPP_
#define INCLUDE_VALUE_BOOLVALUE_HPP_

#include "ValueObject.h"

#include <QCheckBox>

class BoolValue: public QObject, public ValueObject<bool> {
	Q_OBJECT
public:
	BoolValue(bool val, bool *valRef = nullptr) :
			ValueObject(val, valRef) {
	}
	~BoolValue() { if (cBox != nullptr) delete cBox; }
	void Connect(const QObject *object, const char *method) const override {
				QObject::connect(this, SIGNAL(valueChanged(bool)), object, method);
			}

	void Set(std::string newValue) override
	{
		Value(newValue.compare("TRUE") == 0);
		emit valueChanged(Value());
	}

	std::string toString() const override {
		return (value) ? "TRUE" : "FALSE";
	}

	QWidget* Widget(QWidget* parent) override {
		if (cBox == nullptr) {
			cBox = new QCheckBox(parent);
			QObject::connect(cBox, &QCheckBox::stateChanged, this, &BoolValue::stateChanged);
			cBox->setTristate(Value());
		}
		return cBox;
	}

	public slots:
	void stateChanged(int x) {
		emit valueChanged(x == Qt::Checked);
	}

private:
	QCheckBox *cBox = nullptr;
	signals:
		void valueChanged(bool);
};

#endif /* INCLUDE_VALUE_BOOLVALUE_HPP_ */
