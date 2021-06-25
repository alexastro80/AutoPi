/*
 * SettingsManager.cpp
 *
 *  Created on: May 28, 2021
 *      Author: pi
 */

#include <App/SettingsManager.hpp>
#include <QString>
#include <QVBoxLayout>
#include <iostream>
SettingsManager::SettingsManager(QWidget* _frame) : Worker()
{

}

void SettingsManager::initialize(QWidget* parent)
{
	tabWidget = new QTabWidget(parent);
	std::cout << "Setting up settings manager\n";
	if (parent != nullptr)
	{
		QVBoxLayout* layout = new QVBoxLayout(parent);
		layout->addWidget(tabWidget);
		parent->setLayout(layout);
	}


	for (auto category : Categories)
	{
		if (category != nullptr) {
			category->SetParent(tabWidget);
			tabWidget->addTab(category->Display(), QString(category->name.c_str()));
		}
	}
}

bool SettingsManager::Add(Setting* setting)
{

	if(setting != nullptr && !setting->parent.empty())
	{
		int categoryIndex = IndexOf(setting->parent);
		if (categoryIndex >= 0)
		{
			auto category = Categories.at(categoryIndex);
			category->Add(setting);
			return true;
		}
	}
	return false;
}

bool SettingsManager::LoadSettings(string filename)
{
	std::ifstream file;
	file.open(filename);
	if (file.failbit)
	{
		std::cout << "File: " << filename << " does not exist!\n";
		return false;
	}
	string line = "";
	while(!file.eof()) {
		file >> line;
		string tokens[COLUMNS];
		parseLine(line, tokens);
		Setting readSetting(tokens[0], tokens[1], nullptr);
		if (readSetting.isValid())
		{
			SettingCategory* category = GetCategory(readSetting.parent);
			if (category != nullptr)
			{
				Setting* setting = category->Get(readSetting.name);
				if (setting != nullptr) setting->Modify(tokens);
			}
		}
	}
	return true;

}

bool SettingsManager::WriteSettings(string filename)
{
	std::ofstream file;
	file.open(filename);
	for (auto category : Categories) {
		if (category == nullptr)
			continue;
		for (auto setting : category->Settings) {
			if (setting == nullptr) continue;
			file << setting->ToCSV() << "\n";
		}
	}
	file.close();
	return true;
}

void SettingsManager::parseLine(const string& line, string tokens[], int size)
{
	int i = 0, start = 0, end = line.find(",");
	while(i < size && end > 0) {
		tokens[i] = line.substr(start, end);
		start = end;
		end = line.find(",",start);
		i++;
	}
	if (end == -1 && i < size) tokens[i] = line.substr(start,end);
}

void SettingsManager::UpdateSize() {
	if(!Categories.empty()) {
		for (auto category : Categories) {
			category->update();
		}
	}
}

