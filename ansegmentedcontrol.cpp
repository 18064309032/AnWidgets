#include <QPainter>
#include <QStyleOptionTab>

#include "ansegmentedcontrol.h"
#include "anthememanager.h"

AnSegmentedControl::AnSegmentedControl(QWidget *parent)
    : QTabBar(parent)
{
    setDrawBase(false);
    setExpanding(true);
    setUsesScrollButtons(true);
    setElideMode(Qt::ElideRight);
    setAttribute(Qt::WA_Hover);
    connect(&AnThemeManager::instance(), &AnThemeManager::themeChanged, this, QOverload<>::of(&AnSegmentedControl::update));
}

QSize AnSegmentedControl::tabSizeHint(int index) const
{
    const QSize nativeSize = QTabBar::tabSizeHint(index);
    return QSize(nativeSize.width() + 8, qMax(34, nativeSize.height()));
}

void AnSegmentedControl::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    if (count() == 0)
    {
        return;
    }

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(QPen(AnThemeManager::instance().color(AnThemeColor::Divider)));
    painter.setBrush(AnThemeManager::instance().color(AnThemeColor::Surface));
    painter.drawRoundedRect(QRectF(rect()).adjusted(0.5, 0.5, -0.5, -0.5), 8, 8);

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
        const bool hovered = enabled && option.state.testFlag(QStyle::State_MouseOver);
        if (selected || hovered)
        {
            const AnThemeColor backgroundRole = selected ? AnThemeColor::Selection : AnThemeColor::Hover;
            const bool focused = enabled && selected && hasFocus();
            painter.setPen(focused ? QPen(AnThemeManager::instance().color(AnThemeColor::Primary)) : QPen(Qt::NoPen));
            painter.setBrush(AnThemeManager::instance().withAlpha(backgroundRole, enabled ? 140 : 70));
            painter.drawRoundedRect(QRectF(option.rect).adjusted(3, 3, -3, -3), 6, 6);
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
