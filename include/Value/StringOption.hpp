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
	}
	StringOption(std::vector<std::string> _options,
			std::string *valRef = nullptr) :
			StringValue("", valRef) {
		if (!_options.empty()) {
			Value(_options.at(0));
					options = _options;
					shouldRefresh = true;
				}
			}
			~StringOption() {
				if (comboBox != nullptr)
					delete comboBox;
				comboBox = nullptr;
			}
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
					QObject::connect(comboBox,
							&StringSelector::currentTextChanged, this,
							&StringValue::textChanged);
					if (shouldRefresh) {
						for (std::string option : options)
							comboBox->addItem(option);
						shouldRefresh = false;
					}
				}

				int size = options.size();
				for (int i = 0; i < size; i++) {
					string _value = Value();
					if (_value == options.at(i))
						comboBox->setCurrentIndex(i);
				}
				return comboBox;
			}

		private:
			StringSelector *comboBox = nullptr;
			std::vector<std::string> options = std::vector<std::string>();
			bool shouldRefresh = false;
		}
		;

#endif /* INCLUDE_VALUE_STRINGOPTION_HPP_ */
