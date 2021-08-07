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
	StringOption(string val, string *valRef = nullptr) :
			StringValue(val, valRef) {
		AddOption("");
	}
	StringOption(const std::vector<std::string>& _options,
			std::string *valRef = nullptr) :
			StringValue("", valRef) {
		AddOption("");
		AddOptions(_options);
	}
	~StringOption() {
		if (comboBox != nullptr)
			delete comboBox;
		comboBox = nullptr;
	}
	void AddOption(std::string option) {
		if (comboBox == nullptr)
			shouldRefresh = true;
		else {
			comboBox->addItem(option);
			std::cout << option << "\n";
		}
		options.push_back(option);
	}
	void AddOptions(std::vector<std::string> options) {
		if (!options.empty()) {
			for (int i = 0; i < options.size(); i++) {
				AddOption(options.at(i));
			}
		}
	}

	QWidget* Widget(QWidget *parent = nullptr) override
	{
		if (comboBox == nullptr) {
			comboBox = new StringSelector(parent);
			QObject::connect(comboBox,
					&StringSelector::currentTextChanged, this,
					&StringValue::textChanged);
			if (shouldRefresh) {
				for (std::string option : options) {
					std::cout << option << "\n";
					comboBox->addItem(option);
				}
				shouldRefresh = false;
			}
		}
		Value(Value());

		return comboBox;
	}
	std::string Value() const override {
		if (comboBox == nullptr) {
			return "";
		} else {
			return comboBox->currentText().toStdString();
		}
	}
	void Value(std::string val) override {
		if (comboBox != nullptr) {
			bool found = false;
			int size = options.size();
			for (int i = 0; i < size; i++) {
				if (val == options.at(i)) {
					comboBox->setCurrentIndex(i);
					found = true;
					break;
				}
			}
			if (!found) comboBox->setCurrentIndex(0);
		} else {
			ValueObject::Value(val);
		}
	}

private:
	StringSelector *comboBox = nullptr;
	std::vector<std::string> options = std::vector<std::string>();
	bool shouldRefresh = false;
};

#endif /* INCLUDE_VALUE_STRINGOPTION_HPP_ */
