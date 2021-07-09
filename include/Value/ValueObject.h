/*
 * ValueObject.h
 *
 *  Created on: Jun 12, 2021
 *      Author: pi
 */

#ifndef INCLUDE_VALUE_VALUEOBJECT_H_
#define INCLUDE_VALUE_VALUEOBJECT_H_

#include <string.h>
#include <QWidget>

#include "ValueBase.hpp"
template<class T>
class ValueObject: public ValueBase {

public:
	ValueObject(T val, T *valRef = nullptr): value(val) {
		if (valRef != nullptr) {
			valueRef = valRef;
			*valueRef = value;
		}
	}
	~ValueObject() {}


	std::string Type() const override{
		return typeid(value).name();
	}
	T Value() const {
		return (valueRef != nullptr) ? *valueRef : value;
	}
	void Value(T val) {
		(valueRef != nullptr) ? *valueRef = val : value = val;
	}
	virtual QWidget* Widget(QWidget *parent = nullptr) = 0;
protected:
	T value;
	T *valueRef = nullptr;
};

#endif /* INCLUDE_VALUE_VALUEOBJECT_H_ */
