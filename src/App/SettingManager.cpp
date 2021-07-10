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
#include <sstream>

SettingsManager::SettingsManager(QWidget *_frame) :
		Worker() {

}

void SettingsManager::initialize(QWidget *parent) {
	tabWidget = new QTabWidget(parent);
	std::cout << "Setting up settings manager\n";
	if (parent != nullptr) {
		QVBoxLayout *layout = new QVBoxLayout(parent);
		parent->setLayout(layout);
		layout->addWidget(tabWidget);

		QWidget *buttonGroup = new QWidget(parent);
		layout->addWidget(buttonGroup);
		QHBoxLayout *buttonGroupLayout = new QHBoxLayout(buttonGroup);
		QPushButton *save = new QPushButton("Save");
		QObject::connect(save, &QPushButton::pressed, this,
				&SettingsManager::Save);
		buttonGroupLayout->addWidget(save,
				Qt::Alignment::enum_type::AlignRight);
		QPushButton *reset = new QPushButton("Reset");
		QObject::connect(reset, &QPushButton::pressed, this,
				&SettingsManager::Reset);
		buttonGroupLayout->addWidget(reset,
				Qt::Alignment::enum_type::AlignRight);

	}

	for (auto category : Categories) {
		if (category != nullptr) {
			category->SetParent(tabWidget);
			tabWidget->addTab(category->Display(),
					QString(category->name.c_str()));
		}
	}
}

bool SettingsManager::Add(Setting *setting) {

	if (setting != nullptr && !setting->parent.empty()) {
		int categoryIndex = IndexOf(setting->parent);
		if (categoryIndex >= 0) {
			auto category = Categories.at(categoryIndex);
			category->Add(setting);
			return true;
		}
	}
	return false;
}

bool SettingsManager::LoadSettings(string filename) {
	std::ifstream file;
	file.open(filename);
	if (file.fail()) {
		std::cout << "File: " << filename << " does not exist!\n";
		return false;
	}
	string line = "";
	while (!file.eof()) {
		std::getline(file, line);
		string tokens[COLUMNS];
		parseLine(line, tokens);
		Setting readSetting(tokens[0], tokens[1], nullptr);
		if (readSetting.isValid()) {
			SettingCategory *category = GetCategory(readSetting.parent);
			if (category != nullptr) {
				Setting *setting = category->Get(readSetting.name);
				if (setting != nullptr)
					setting->Modify(tokens);
			}
		}
	}
	UpdateSize();
	return true;
}

bool SettingsManager::WriteSettings(string filename) {
	std::ofstream file;
	file.open(filename, std::ios_base::trunc);
	for (auto category : Categories) {
		if (category == nullptr)
			continue;
		for (auto setting : category->Settings) {
			if (setting == nullptr)
				continue;
			file << setting->ToCSV() << "\n";
		}
	}
	file.close();
	return true;
}

void SettingsManager::parseLine(const string& line, string tokens[], int size) {
	int i = 0, start = 0, end = line.find(",");
	while (i < size && end > 0) {
		tokens[i] = line.substr(start, end-start);
		start = end + 1;
		end = line.find(",", start);
		i++;
	}
	if (end == -1 && i < size)
		tokens[i] = line.substr(start, line.length()-start);
}

void SettingsManager::UpdateSize() {
	if (!Categories.empty()) {
		for (auto category : Categories) {
			category->update();
		}
	}
}

