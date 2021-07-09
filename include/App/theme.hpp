#pragma once

#include <QAbstractScrollArea>
#include <QApplication>
#include <QFrame>
#include <QMap>
#include <QObject>
#include <QPair>
#include <QPushButton>
#include <QScroller>
#include <QScrollerProperties>
#include <QString>
#include <QVariant>

class Theme : public QObject {
    Q_OBJECT

   public:
    enum Orientation { BOTTOM, RIGHT };

    // pls dont abuse this... this is temporary and will be removed soon
    static QFont font_10;
    static QFont font_12;
    static QFont font_14;
    static QFont font_16;
    static QFont font_18;
    static QFont font_24;
    static QFont font_36;
    static QFont font_72;

    static QSize icon_16;
    static QSize icon_20;
    static QSize icon_22;
    static QSize icon_24;
    static QSize icon_26;
    static QSize icon_28;
    static QSize icon_32;
    static QSize icon_36;
    static QSize icon_42;
    static QSize icon_48;
    static QSize icon_56;
    static QSize icon_84;
    static QSize icon_96;

    static const QColor danger_color;
    static const QColor success_color;

    Theme();
    bool Mode = true;

    void Scale(double scale);

    QIcon MakeButtonIcon(QString name, QPushButton *button, QString alt_name = QString());



    inline static QFrame *Br(QWidget *parent = nullptr, bool vertical = false)
    {
        QFrame *b = new QFrame(parent);
        b->setLineWidth(2);
        b->setFrameShape(vertical ? QFrame::VLine : QFrame::HLine);
        b->setFrameShadow(QFrame::Plain);

        return b;
    }
    inline static void ToTouchScroller(QAbstractScrollArea *area)
    {
        QVariant policy =
            QVariant::fromValue<QScrollerProperties::OvershootPolicy>(QScrollerProperties::OvershootAlwaysOff);
        QScrollerProperties properties = QScroller::scroller(area->viewport())->scrollerProperties();
        properties.setScrollMetric(QScrollerProperties::VerticalOvershootPolicy, policy);
        properties.setScrollMetric(QScrollerProperties::HorizontalOvershootPolicy, policy);

        area->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        QScroller::grabGesture(area->viewport(), QScroller::LeftMouseButtonGesture);
        QScroller::scroller(area->viewport())->setScrollerProperties(properties);
    }
    static Theme *getInstance();

   void Update() {Update(Mode); }
   public slots:

   void Update(bool mode);

   private:
    QPalette palette;
    QMap<QString, QString> stylesheets;
    double scale = 1.0;

    void setPalette();
    QString parseStylesheet(QString file);
    QString scaleStylesheet(QString stylesheet);
    QIcon themedButtonIcon(QIcon icon, QAbstractButton *button);

    inline QColor getBaseColor() { return Mode ? QColor(255, 255, 255) : QColor(0, 0, 0); }

   signals:
    void modeUpdated(bool mode);
};
