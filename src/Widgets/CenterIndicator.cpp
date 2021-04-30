#include <Widgets/CenterIndicator.hpp>

CenterIndicator::CenterIndicator(QWidget* parent) : QWidget(parent) 
{	
	if(parent != nullptr)
	{
		_layout = new QVBoxLayout(this);
		//_layout->setContentsMargins(0, 0,0,0);
    
		topLabel = new QLabel("Speed", this);
		topLabel->setAlignment(Qt::AlignCenter);
		centerLabel = new QLabel("00.0", this);
		centerLabel->setAlignment(Qt::AlignCenter);
		bottomLabel = new QLabel("MPH", this);
		bottomLabel->setAlignment(Qt::AlignCenter);

		_layout->addStretch();
		_layout->addWidget(topLabel);
		_layout->addWidget(centerLabel);
		_layout->addWidget(bottomLabel);
		_layout->addStretch();
	}
}

