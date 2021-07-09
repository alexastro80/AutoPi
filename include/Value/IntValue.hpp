/*
 * IntValue.hpp
 *
 *  Created on: Jul 9, 2021
 *      Author: pi
 */

#ifndef INCLUDE_VALUE_INTVALUE_HPP_
#define INCLUDE_VALUE_INTVALUE_HPP_

#include "ValueObject.h"

#include <Widgets/NumberSelector.hpp>

#include <QObject>

class IntValue: public QObject, public ValueObject {
Q_OBJECT
public:
	IntValue(int integer) {
		value = integer;
	}
	IntValue(int *integerRef) {
		if (integerRef != nullptr) {
			valueRef = integerRef;
			value = *valueRef;
		}
	}

	void Connect(const QObject* object, const char* method) const override{
			QObject::connect(this, SIGNAL(IntValue::valueChanged(int)), object, SLOT(method));
		}
	void Set(std::string newValue) override {
		try {
			value = std::stoi(newValue);
			if (valueRef != nullptr)
				*valueRef = value;
			else
				emit valueChanged(value);
		} catch(std::invalid_argument &e) {}
	}

	std::string toString() const override {
		return std::to_string(value);
	}

	std::string Type() const override {
		return "INT";
	}

	QWidget* Widget(QWidget *parent) override{
		if (numSelector == nullptr) {
			numSelector = new NumberSelector(parent, value);
			QObject::connect(numSelector, &NumberSelector::valueChanged, this,
					&IntValue::valueChanged);
		}
		return numSelector;
	}
protected:
	int value = 0;
	int *valueRef = nullptr;
private:
	NumberSelector *numSelector = nullptr;

signals:
	void valueChanged(int);
};

#endif /* INCLUDE_VALUE_INTVALUE_HPP_ */
