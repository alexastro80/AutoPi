#ifndef CENTER_INDICATOR_HPP
#define CENTER_INDICATOR_HPP
#include <QWidget>
#include <QtWidgets>
#include <QLabel>
#include <App/theme.hpp>

class CenterIndicator : public QWidget
{
	Q_OBJECT
public:
	explicit CenterIndicator(QWidget* parent = nullptr);
	~CenterIndicator() 
		{if(topLabel != nullptr) delete topLabel;
		 if(centerLabel != nullptr) delete centerLabel;
		 if(bottomLabel != nullptr) delete bottomLabel;}
	void SetPosition(int x, int y) { positionX = x; positionY = y; }
	void SetSize(int h, int w);
	void SetTopLabel(const QString& label)
		{ if(topLabel != nullptr) topLabel->setText(label); }
	void SetCenterLabel(const QString& label)
		{ if(centerLabel != nullptr) centerLabel->setText(label); }
	void SetBottomLabel(const QString& label)
		{ if(bottomLabel != nullptr) bottomLabel->setText(label); }
	void Update(double value)
		{ val = value+val; SetCenterLabel(QString::number(val)); }
	
private:
	QVBoxLayout* _layout = nullptr;
	QLabel* topLabel = nullptr;
	QLabel* centerLabel = nullptr;
	QLabel* bottomLabel = nullptr;
	double val;
	int positionX = 0;
	int positionY = 0;

};
#endif
