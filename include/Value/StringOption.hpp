/*
 * StringOption.hpp
 *
 *  Created on: Jun 12, 2021
 *      Author: pi
 */

#ifndef INCLUDE_VALUE_STRINGOPTION_HPP_
#define INCLUDE_VALUE_STRINGOPTION_HPP_

#include <Value/StringValue.hpp>
#include <vector.h>

class StringOption : public StringValue
{
public:
	StringOption(const std::string& _value = "") {
		value = _value;
	}
	StringOption(const StringOption& _value) {
		value = _value.value;
		valueRef = _value.value;
	}
	StringOption& operator=(const StringOption& _value) {
		value = _value.value;
		valueRef = _value.valueRef;
		return this;
	}
	StringOption& operator=(const std::string _value) {
		value = _value;
		if (valueRef != nullptr)
			*valueRef = _value;
		return this;
	}
	void AddOption(std::string option) {
		options.push_back(option);
	}
	std::string Value() {
		if (valueRef != nullptr)
			return *valueRef;
		return value;
	}
	std::string Type() const override { return "STRING"; }
	QWidget* Widget() const override { return nullptr; }
private:
	std::string value = "";
	std::string* valueRef = nullptr;
	std::vector<std::string> options;
};



#endif /* INCLUDE_VALUE_STRINGOPTION_HPP_ */
