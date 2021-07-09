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
	StringOption(string val, string* valRef = nullptr) : StringValue(val, valRef) {}

	~StringOption() { if (comboBox != nullptr) delete comboBox; }
	void AddOption(std::string option) {
		if (comboBox == nullptr)
			shouldRefresh = true;
		else
			comboBox->addItem(option);
		options.push_back(option);
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
