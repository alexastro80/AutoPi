#include <math.h>

#include <QApplication>
#include <QFile>
#include <QBitmap>
#include <QFontDatabase>
#include <QIcon>
#include <QObject>
#include <QPainter>
#include <QPair>
#include <QPixmap>
#include <QRegularExpression>
#include <QTextStream>
#include <QTransform>
#include <QFileInfo>

#include "App/theme.hpp"
#include "App/config.hpp"

#include <iostream>
QFont Theme::font_10 = QFont("Montserrat", 10);
QFont Theme::font_12 = QFont("Montserrat", 12);
QFont Theme::font_14 = QFont("Montserrat", 14);
QFont Theme::font_16 = QFont("Montserrat", 16);
QFont Theme::font_18 = QFont("Montserrat", 18);
QFont Theme::font_24 = QFont("Montserrat", 24);
QFont Theme::font_36 = QFont("Montserrat", 36);
QFont Theme::font_72 = QFont("Montserrat", 72);

QSize Theme::icon_16 = QSize(16, 16);
QSize Theme::icon_20 = QSize(20, 20);
QSize Theme::icon_22 = QSize(22, 22);
QSize Theme::icon_24 = QSize(24, 24);
QSize Theme::icon_26 = QSize(26, 26);
QSize Theme::icon_28 = QSize(28, 28);
QSize Theme::icon_32 = QSize(32, 32);
QSize Theme::icon_36 = QSize(36, 36);
QSize Theme::icon_42 = QSize(42, 42);
QSize Theme::icon_48 = QSize(48, 48);
QSize Theme::icon_56 = QSize(56, 56);
QSize Theme::icon_84 = QSize(84, 84);
QSize Theme::icon_96 = QSize(96, 96);

const QColor Theme::danger_color = QColor(211, 47, 47);
const QColor Theme::success_color = QColor(56, 142, 60);

Theme::Theme() : QObject(qApp), palette()
{
    QFontDatabase::addApplicationFont(":/fonts/Nasalization/nasalization-rg.ttf");
    QFontDatabase::addApplicationFont(":/fonts/Roboto/Roboto-Light.ttf");
    QFontDatabase::addApplicationFont(":/fonts/Roboto/Roboto-Regular.ttf");

    stylesheets["light"] = parseStylesheet(":/stylesheets/light.qss");
    stylesheets["dark"] = parseStylesheet(":/stylesheets/dark.qss");
}

void Theme::Scale(double scale)
{
    scale = scale;

    Theme::font_10.setPointSize(std::ceil(10 * scale));
    Theme::font_12.setPointSize(std::ceil(12 * scale));
    Theme::font_14.setPointSize(std::ceil(14 * scale));
    Theme::font_16.setPointSize(std::ceil(16 * scale));
    Theme::font_18.setPointSize(std::ceil(18 * scale));
    Theme::font_24.setPointSize(std::ceil(24 * scale));
    Theme::font_36.setPointSize(std::ceil(36 * scale));
    Theme::font_72.setPointSize(std::ceil(72 * scale));

    Theme::icon_16 *= scale;
    Theme::icon_20 *= scale;
    Theme::icon_22 *= scale;
    Theme::icon_24 *= scale;
    Theme::icon_26 *= scale;
    Theme::icon_28 *= scale;
    Theme::icon_32 *= scale;
    Theme::icon_36 *= scale;
    Theme::icon_42 *= scale;
    Theme::icon_48 *= scale;
    Theme::icon_56 *= scale;
    Theme::icon_84 *= scale;
    Theme::icon_96 *= scale;

    qApp->setFont(Theme::font_14);
}

QString Theme::parseStylesheet(QString file)
{
    QFile f(file);
    f.open(QFile::ReadOnly | QFile::Text);
    QTextStream s(&f);
    QString stylesheet(s.readAll());
    f.close();

    return stylesheet;
}

QString Theme::scaleStylesheet(QString stylesheet)
{
    QRegularExpression px_regex(" (-?\\d+)px");
    QRegularExpressionMatchIterator i = px_regex.globalMatch(stylesheet);
    while (i.hasNext()) {
        QRegularExpressionMatch match = i.next();
        if (match.hasMatch()) {
            int scaled_px = std::ceil(match.captured(1).toInt() * scale);
            stylesheet.replace(match.captured(), QString("%1px").arg(scaled_px));
        }
    }

    return stylesheet;
}

void Theme::setPalette()
{
    QColor color(Config::getInstance()->Color());
    palette.setColor(QPalette::Base, color);
    color.setAlphaF(.5);
    palette.setColor(QPalette::AlternateBase, color);
    qApp->setPalette(palette);
}

QIcon Theme::themedButtonIcon(QIcon icon, QAbstractButton *button)
{
    QSize size(512, 512);
    QBitmap icon_mask(icon.pixmap(size).createMaskFromColor(Qt::transparent));
    QBitmap alt_icon_mask;
    {
        QIcon alt_icon(button->property("alt_icon").value<QIcon>());
        if (!alt_icon.isNull())
            alt_icon_mask = alt_icon.pixmap(size).createMaskFromColor(Qt::transparent);
    }

    QColor base_color(getBaseColor());
    base_color.setAlpha(mode ? 222 : 255);
    QColor accent_color(Config::getInstance()->Color());
    accent_color.setAlpha(mode ? 222 : 255);

    QPixmap normal_on(size);
    normal_on.fill(!button->property("page").isNull() ? accent_color : base_color);
    normal_on.setMask(!alt_icon_mask.isNull() ? alt_icon_mask : icon_mask);

    QPixmap normal_off(size);
    {
        QColor color(base_color);
        if (!button->property("page").isNull())
            color.setAlpha(mode ? 134 : 162);
        normal_off.fill(color);
        normal_off.setMask(icon_mask);
    }

    QPixmap disabled_on(size);
    QPixmap disabled_off(size);
    {
        QColor color(base_color);
        color.setAlpha(mode ? 128 : 97);
        disabled_on.fill(color);
        disabled_on.setMask(!alt_icon_mask.isNull() ? alt_icon_mask : icon_mask);
        disabled_off.fill(color);
        disabled_off.setMask(icon_mask);
    }

    QIcon themed_icon;
    themed_icon.addPixmap(disabled_on, QIcon::Disabled, QIcon::On);
    themed_icon.addPixmap(disabled_off, QIcon::Disabled, QIcon::Off);
    themed_icon.addPixmap(normal_on, QIcon::Normal, QIcon::On);
    themed_icon.addPixmap(normal_off, QIcon::Normal, QIcon::Off);

    return themed_icon;
}

QIcon Theme::MakeButtonIcon(QString name, QPushButton *button, QString alt_name)
{
    if (!alt_name.isNull())
        button->setProperty("alt_icon", QVariant::fromValue(QIcon(QString(":/icons/%1.svg").arg(alt_name))));
    QFileInfo checkIcon(QString(":/icons/%1.svg").arg(name));
    
    if (checkIcon.exists() && checkIcon.isFile())
    {
        button->setProperty("themed_icon", true);
        return QIcon(QString(":/icons/%1.svg").arg(name));
    }
    else 
    {
        checkIcon = QFileInfo(QString(":/icons/%1.png").arg(name));
        if (checkIcon.exists() && checkIcon.isFile())
        {
            button->setProperty("themed_icon", true);
            return QIcon(QString(":/icons/%1.png").arg(name));
        }
    }
    std::cout << "Icon Not Found!\n";
    return QIcon();
}

void Theme::Update()
{
    setPalette();
    qApp->setStyleSheet(scaleStylesheet(stylesheets[mode ? "dark" : "light"]));

    for (QWidget *widget : qApp->allWidgets()) {
        QAbstractButton *button = qobject_cast<QAbstractButton*>(widget);
        if ((button != nullptr) && !button->property("themed_icon").isNull())
            button->setIcon(themedButtonIcon(button->icon(), button));
    }

    emit modeUpdated(mode);
}

Theme *Theme::getInstance()
{
    static Theme theme;
    return &theme;
}
