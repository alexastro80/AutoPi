/*
 * SettingsCategory.cpp
 *
 *  Created on: Jun 13, 2021
 *      Author: pi
 */

#include <App/SettingsCategory.hpp>
#include <QVBoxLayout>
void SettingCategory::initialize(QWidget* parent)
{
	page = new QWidget(parent);
	QVBoxLayout* layout = new QVBoxLayout(page);
	for (auto setting : Settings)
	{
		if (setting != nullptr)
		{
			layout->addWidget(setting->Display(page));
		}
	}
	page->setLayout(layout);
}


