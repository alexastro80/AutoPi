/*
 * StringOption.hpp
 *
 *  Created on: Jun 12, 2021
 *      Author: pi
 */

#ifndef INCLUDE_VALUE_STRINGOPTION_HPP_
#define INCLUDE_VALUE_STRINGOPTION_HPP_

#include <Value/StringValue.hpp>
#include <Widgets/StringSelector.hpp>
#include <vector>
#include <iostream>

class StringOption: public StringValue {
public:
	StringOption( std::string _value = "") {
		value = _value;
		options.push_back(_value);
		shouldRefresh = true;
	}
	StringOption(const StringOption &_value) {
		value = _value.value;
		valueRef = _value.valueRef;
		options = _value.options;
		shouldRefresh = true;
	}
	StringOption& operator=(const StringOption &_value) {
		value = _value.value;
		valueRef = _value.valueRef;
		options = _value.options;
		shouldRefresh = true;
		return *this;
	}
	StringOption& operator=(const std::string _value) {
		value = _value;
		options.push_back(value);
		shouldRefresh = true;
		if (valueRef != nullptr)
			*valueRef = _value;
		return *this;
	}
	void AddOption(std::string option) {
		if (comboBox == nullptr)
			shouldRefresh = true;
		else
			comboBox->addItem(option);
		options.push_back(option);
	}
	std::string Value() {
		if (valueRef != nullptr)
			return *valueRef;
		return value;
	}
	std::string Type() const override {
		return "STRING";
	}
	QWidget* Widget(QWidget *parent = nullptr) override
	{
		if (comboBox == nullptr) {
			comboBox = new StringSelector(parent);
			QObject::connect(comboBox, &StringSelector::currentTextChanged,
					this, &StringValue::textChanged);
			if (shouldRefresh) {
				for (std::string option : options)
					comboBox->addItem(option);
				shouldRefresh = false;
			}
		}
		return comboBox;
	}

private:
	StringSelector *comboBox = nullptr;
	std::vector<std::string> options = std::vector<std::string>();
	bool shouldRefresh = false;
};

#endif /* INCLUDE_VALUE_STRINGOPTION_HPP_ */
