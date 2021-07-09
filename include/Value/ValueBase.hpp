/*
 * ValueBase.hpp
 *
 *  Created on: Jul 9, 2021
 *      Author: pi
 */

#ifndef INCLUDE_VALUE_VALUEBASE_HPP_
#define INCLUDE_VALUE_VALUEBASE_HPP_

#include <QWidget>
#include <string>

class ValueBase {
public:
	virtual void Connect(const QObject *object, const char *method) const = 0;
	virtual void Set(std::string newValue) = 0;
	virtual std::string toString() const = 0;
	virtual std::string Type() const = 0;
	virtual QWidget* Widget(QWidget *parent = nullptr) = 0;
};

#endif /* INCLUDE_VALUE_VALUEBASE_HPP_ */
