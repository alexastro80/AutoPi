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
class StringValue: public QObject, public ValueObject<std::string> {
	Q_OBJECT
public:
	StringValue(string val, string *valRef = nullptr) :
			ValueObject(val, valRef) {
	}
	~StringValue() { if (line != nullptr) delete line; }
	void Connect(const QObject *object, const char *method) const override {
				QObject::connect(this, SIGNAL(valueChanged(std::string)), object, SLOT(method));
			}
	void Set(std::string newValue) override {
		Value(newValue);
		valueChanged(Value());
	}
	std::string toString() const override {
		return Value();
	}
	QWidget* Widget(QWidget *parent = nullptr) override
	{
		if (line == nullptr) {
			line = new QLineEdit(QString(Value().c_str()), parent);
			QObject::connect(line, &QLineEdit::textChanged, this,
					&StringValue::textChanged);
		}
		return line;
	}

public slots:
	void textChanged(const QString &text) {
		ValueObject::Value(text.toStdString());
		emit valueChanged(Value());
	}

private:
	QLineEdit *line = nullptr;
signals:
	void valueChanged(std::string);
};

#endif /* INCLUDE_VALUE_STRINGVALUE_HPP_ */
