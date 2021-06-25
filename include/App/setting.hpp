

#ifndef SETTING_HPP
#define SETTING_HPP

#include <string>
#include <vector>
using std::string;
using std::vector;

#include <QtWidgets>
#include <QString>
#include <QObject>

#include <Value/ValueObject.h>


const int COLUMNS =4;

#define DOUBLE_TYPE "DOUBLE"
#define INT_TYPE "INTEGER"
#define STRING_TYPE "STRING"

const int CATEGORY = 0;
const int NAME = 1;
const int TYPE = 2;
const int VALUE = 3;


union ValueRef {
	double* doubleRef;
	int*	intRef;
	string* strRef;
};

union Value {
	double 	d;
	int		i;
};

class Setting : public QObject {
	Q_OBJECT
public:
	Setting(string _parent, string _name, ValueObject* value);
	Setting(string _parent, string _name, const QObject* object, const char* method, ValueObject* value);

	Setting(const Setting& setting) {
		parent = setting.parent;
		name = setting.name;
		value = setting.value;
	}

	Setting& operator=(const Setting& setting) {
		parent = setting.parent;
		name = setting.name;
		value = setting.value;
		return *this;
	}

//	Setting(string csv[]);
	QWidget* Display(QWidget* parent = nullptr) { initialize(parent); return item; }
	string ToCSV();

	void ChangeVal(string newValue) {
		if (value != nullptr)
			value->Set(newValue);
	}
	void GetValue(ValueObject *val) const {
		val = value;
	}

	std::string Type() const {
		if (value != nullptr)
			return value->Type();
		return "";
	}
	bool isValid() 	{ return (parent.empty() && name.empty()); }

	void Modify(string csv[], int size = COLUMNS);

	void update()
	{ if (item != nullptr)
		initialize(item->parentWidget()); }
	string parent	= "";
	string name		= "";
private:
	void initialize(QWidget* parent = nullptr);
	QWidget* item = nullptr;
	ValueObject* value = nullptr;
};


void CSVLine(const string& line, string csv[]);

#endif //SETTING_HPP

