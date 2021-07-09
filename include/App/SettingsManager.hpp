/*
 * SettingsManager.hpp
 *
 *  Created on: May 28, 2021
 *      Author: pi
 */

#ifndef INCLUDE_SETTINGS_MANAGER_HPP_
#define INCLUDE_SETTINGS_MANAGER_HPP_


#include <vector>
#include <string>
#include <fstream>
#include <iostream>

#include <App/setting.hpp>
#include <App/SettingsCategory.hpp>
#include <App/Worker.hpp>

using std::vector;
using std::string;

#include <QLabel>

class SettingsManager : public Worker {
public:
	explicit SettingsManager(QWidget* _frame = nullptr);
	~SettingsManager() {
		for (auto category : Categories) {
			if (category != nullptr) delete category;
		}
		if (tabWidget != nullptr) delete tabWidget;
	}
	//Worker Functions
	QWidget* Display() override { return tabWidget; }
	void UpdateSize() override;
	void initialize(QWidget* parent = nullptr);
	void Add(SettingCategory* category) { if (category != nullptr) Categories.push_back(category); }
	bool Add(Setting* setting);
	int IndexOf(string _name) {
		int i = 0;
		for (auto category : Categories) {
			if (category != nullptr && category->name.compare(_name) == 0) return i;
			else i++;
		}
		return -1;
	}
	SettingCategory* GetCategory(string _name) {
		int index = IndexOf(_name);
		return (index >= 0) ? (Categories.at(index)) : nullptr;
	}
	bool LoadSettings(string filename);
	bool WriteSettings(string filename);

	vector<SettingCategory*> Categories;
private:
	void parseLine(const string& line, string tokens[], int size = COLUMNS);

	QTabWidget* tabWidget = nullptr;

};

#endif /* INCLUDE_SETTINGS_MANAGER_HPP_ */
