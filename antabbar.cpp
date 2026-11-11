#include <QPainter>
#include <QStyleOptionTab>

#include "antabbar.h"
#include "anthememanager.h"

AnTabBar::AnTabBar(QWidget *parent)
    : QTabBar(parent)
{
    setAttribute(Qt::WA_Hover);
    setExpanding(false);
    setDrawBase(false);
    connect(&AnThemeManager::instance(), &AnThemeManager::themeChanged, this, QOverload<>::of(&AnTabBar::update));
}

QSize AnTabBar::tabSizeHint(int index) const
{
    return QTabBar::tabSizeHint(index) + QSize(8, 6);
}

void AnTabBar::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    for (int index = 0; index < count(); ++index)
    {
        QStyleOptionTab option;
        initStyleOption(&option, index);
        if (!option.rect.intersects(rect()))
        {
            continue;
        }

        const bool enabled = option.state.testFlag(QStyle::State_Enabled);
        const bool selected = option.state.testFlag(QStyle::State_Selected);
        const bool hovered = option.state.testFlag(QStyle::State_MouseOver);
        const QRectF bounds = QRectF(option.rect).adjusted(1.5, 1.5, -1.5, -1.5);
        if (selected || (enabled && hovered))
        {
            const AnThemeColor backgroundRole = selected ? AnThemeColor::Selection : AnThemeColor::Hover;
            painter.setPen(Qt::NoPen);
            painter.setBrush(AnThemeManager::instance().color(backgroundRole));
            painter.drawRoundedRect(bounds, 6, 6);
        }
        if (selected && enabled && hasFocus())
        {
            painter.setBrush(Qt::NoBrush);
            painter.setPen(QPen(AnThemeManager::instance().color(AnThemeColor::Primary)));
            painter.drawRoundedRect(bounds, 6, 6);
        }

        if (option.icon.isNull() && option.leftButtonSize.isEmpty() && option.rightButtonSize.isEmpty() && (shape() == RoundedNorth || shape() == RoundedSouth || shape() == TriangularNorth || shape() == TriangularSouth))
        {
            const AnThemeColor textRole = enabled ? AnThemeColor::TextPrimary : AnThemeColor::TextDisabled;
            painter.setPen(AnThemeManager::instance().color(textRole));
            painter.drawText(option.rect, Qt::AlignCenter | Qt::TextShowMnemonic, option.text);
        }
        else
        {
            style()->drawControl(QStyle::CE_TabBarTabLabel, &option, &painter, this);
        }
    }
}
