/*
 * StringValue.hpp
 *
 *  Created on: Jun 12, 2021
 *      Author: pi
 */

#ifndef INCLUDE_VALUE_STRINGVALUE_HPP_
#define INCLUDE_VALUE_STRINGVALUE_HPP_

#include <Value/ValueObject.h>
#include <QLineEdit>
#include <string.h>
using std::string;
class StringValue : public QObject, public ValueObject
{
	Q_OBJECT
public:
	StringValue(const std::string& _value = "") {
		value = _value;
	}
	StringValue(std::string* valRef, const std::string& defaultValue = "") {
		value = defaultValue;
		valueRef = valRef;
	}
	StringValue(const StringValue& _value) {
		value = _value.value;
		valueRef = _value.valueRef;
	}
	StringValue& operator=(const StringValue& _value) {
		value = _value.value;
		valueRef = _value.valueRef;
		return *this;
	}
	StringValue& operator=(const std::string _value) {
		value = _value;
		if (valueRef != nullptr)
			*valueRef = _value;
		return *this;
	}
	virtual void Connect(const QObject* object, const char* method) const {
		QObject::connect(this, SIGNAL(StringValue::valueChanged(string)), object, SLOT(method));
	}
	void Set(std::string newValue) override {
		value = newValue;
		if (valueRef != nullptr)
			*valueRef = value;
		valueChanged(value);
	}
	std::string toString() const override { return Value(); }
	std::string Type() const override { return "STRING"; }
	std::string Value() const {
			if (valueRef != nullptr)
				return *valueRef;
			return value;
		}
	QWidget* Widget(QWidget* parent = nullptr) const override
	{ return new QLineEdit(QString(Value().c_str()), parent);}
private:
	std::string value = "";
	std::string* valueRef = nullptr;

signals:
	void valueChanged(std::string);
};


#endif /* INCLUDE_VALUE_STRINGVALUE_HPP_ */
