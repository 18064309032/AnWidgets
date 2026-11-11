#include <QFontMetrics>
#include <QPainter>
#include <QSizePolicy>
#include <QStyle>
#include <QStyleOption>

#include "answitch.h"
#include "anthememanager.h"

AnSwitch::AnSwitch(QWidget *parent)
    : QAbstractButton(parent)
{
    setCheckable(true);
    setFocusPolicy(Qt::StrongFocus);
    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
    setAttribute(Qt::WA_Hover);
    connect(&AnThemeManager::instance(), &AnThemeManager::themeChanged, this, QOverload<>::of(&AnSwitch::update));
}

QSize AnSwitch::sizeHint() const
{
    const QFontMetrics metrics(font());
    const int labelWidth = text().isEmpty() ? 0 : metrics.size(Qt::TextShowMnemonic, text()).width() + 8;
    return QSize(40 + labelWidth, qMax(24, metrics.height() + 4));
}

void AnSwitch::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    const bool enabled = isEnabled();
    const bool checked = isChecked();
    AnThemeColor trackRole = checked ? AnThemeColor::Primary : AnThemeColor::Border;
    if (!enabled)
    {
        trackRole = AnThemeColor::Disabled;
    }
    else if (isDown() || (checked && underMouse()))
    {
        trackRole = AnThemeColor::Pressed;
    }
    else if (underMouse())
    {
        trackRole = AnThemeColor::TextMuted;
    }

    const QRect trackArea = QStyle::visualRect(layoutDirection(), rect(), QRect(2, (height() - 20) / 2, 36, 20));
    const QRectF track(trackArea);
    const bool knobAtRight = checked != (layoutDirection() == Qt::RightToLeft);
    const QPointF knobCenter(knobAtRight ? track.right() - 10 : track.left() + 10, track.center().y());
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(Qt::NoPen);
    painter.setBrush(AnThemeManager::instance().color(trackRole));
    painter.drawRoundedRect(track, 10, 10);
    painter.setBrush(AnThemeManager::instance().color(AnThemeColor::TextOnAccent));
    painter.drawEllipse(knobCenter, 7, 7);

    QStyleOption option;
    option.initFrom(this);
    if (enabled && hasFocus() && option.state.testFlag(QStyle::State_KeyboardFocusChange))
    {
        painter.setBrush(Qt::NoBrush);
        painter.setPen(QPen(AnThemeManager::instance().color(AnThemeColor::Primary)));
        painter.drawRoundedRect(track.adjusted(-1.5, -1.5, 1.5, 1.5), 11, 11);
    }

    if (!text().isEmpty())
    {
        const AnThemeColor labelRole = enabled ? AnThemeColor::TextPrimary : AnThemeColor::TextDisabled;
        const QRect labelRect = QStyle::visualRect(layoutDirection(), rect(), rect().adjusted(46, 0, 0, 0));
        const Qt::Alignment alignment = QStyle::visualAlignment(layoutDirection(), Qt::AlignLeft | Qt::AlignVCenter);
        painter.setPen(AnThemeManager::instance().color(labelRole));
        painter.drawText(labelRect, alignment | Qt::TextShowMnemonic, text());
    }
}
