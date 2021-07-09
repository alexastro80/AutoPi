#include <Widgets/NumberSelector.hpp>
#include <QFont>
NumberSelector::NumberSelector(QWidget* parent, int defaultValue) : QWidget(parent)
{
    value = defaultValue;

    //Create widgets
    label = new QLabel(QString::number(value));
    QPushButton* upButton = new QPushButton("+");
    QObject::connect(upButton,&QPushButton::pressed,this,&NumberSelector::increment);
    QPushButton* downButton = new QPushButton("-");
    QObject::connect(downButton,&QPushButton::pressed,this,&NumberSelector::decrement);
    QFont labelFont = label->font();
    QFont btnFont = upButton->font();
    labelFont.setPointSize(labelFont.pointSize()*3);
    label->setFont(labelFont);
    btnFont.setPointSize(labelFont.pointSize()*.7);
    upButton->setFont(btnFont);
    downButton->setFont(btnFont);
    int buttonWidth = btnFont.pointSize()*2;
    upButton->setMaximumWidth(buttonWidth);
    downButton->setMaximumWidth(buttonWidth);

    //Create Layout
    QGridLayout* layout = new QGridLayout(this);
    layout->addWidget(label,0,0,4,1);
    layout->addWidget(upButton,1,1,Qt::AlignLeft);
    layout->addWidget(downButton,2,1,Qt::AlignLeft);

    setFixedHeight(label->sizeHint().height()*2);
    setLayout(layout);
}
