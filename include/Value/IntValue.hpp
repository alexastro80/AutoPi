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

class IntValue: public QObject, public ValueObject<int> {
Q_OBJECT
public:
	IntValue(int val, int *valRef = nullptr) :
			ValueObject(val, valRef) {
	}
	~IntValue() {
		if (numSelector != nullptr)
			delete numSelector;
	}

	void Connect(const QObject *object, const char *method) const override {
		QObject::connect(this, SIGNAL(valueChanged(int)), object, method);
	}
	void Set(std::string newValue) override {
		try {
			Value(std::stoi(newValue));
			emit valueChanged(Value());
		} catch (std::invalid_argument &e) {
		}
	}

	std::string toString() const override {
		return std::to_string(Value());
	}

	QWidget* Widget(QWidget *parent) override {
		if (numSelector == nullptr) {
			numSelector = new NumberSelector(parent, Value());
			QObject::connect(numSelector, &NumberSelector::valueChanged, this,
					&IntValue::numberChanged);
		}
		numSelector->setValue(Value());
		return numSelector;
	}
public slots:
	void numberChanged(int x) {
		Value(x);
		emit valueChanged(x);
	}

private:
	NumberSelector *numSelector = nullptr;

signals:
	void valueChanged(int);
};

#endif /* INCLUDE_VALUE_INTVALUE_HPP_ */
