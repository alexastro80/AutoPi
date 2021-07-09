/*
 * SettingsCategory.hpp
 *
 *  Created on: Jun 11, 2021
 *      Author: pi
 */

#ifndef SETTINGSCATEGORY_HPP_
#define SETTINGSCATEGORY_HPP_

#include <App/setting.hpp>

class SettingCategory {
public:
	SettingCategory(string _name = "") : name(_name) { initialize(); }
	~SettingCategory() {
		for (auto setting : Settings) {
			if (setting != nullptr)
				delete setting;
		}
		if (page != nullptr) delete page;
	}
	void Add(Setting *setting) {
		if (setting != nullptr)
			Settings.push_back(setting);
	}

	int IndexOf(string _name) {
		int i = 0;
		for (auto setting : Settings) {
			if (setting->name.compare(_name) == 0)
				return i;
			else
				i++;
		}
		return -1;
	}
	void initialize(QWidget* parent = nullptr);
	QWidget* Display() { return page; }
	Setting* Get(string _setting) {
		int index = IndexOf(_setting);
		return (index >= 0) ? (Settings.at(index)) : nullptr;
	}
	void SetParent(QWidget* parent) { initialize(parent); }
	void update() {
		for (auto setting : Settings) {
			if (setting != nullptr)
				setting->update();
		}
	}
	vector<Setting*> Settings;
	string name = "";
private:
	QWidget* page = nullptr;

};

#endif /* SETTINGSCATEGORY_HPP_ */

