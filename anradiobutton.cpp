#include <QPainter>
#include <QStyleOptionButton>

#include "anradiobutton.h"
#include "anthememanager.h"

AnRadioButton::AnRadioButton(QWidget *parent)
    : QRadioButton(parent)
{
    initialize();
}

AnRadioButton::AnRadioButton(const QString &text, QWidget *parent)
    : QRadioButton(text, parent)
{
    initialize();
}

void AnRadioButton::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QStyleOptionButton option;
    initStyleOption(&option);
    const bool enabled = option.state.testFlag(QStyle::State_Enabled);
    const bool focused = enabled && option.state.testFlag(QStyle::State_HasFocus);
    const bool checked = option.state.testFlag(QStyle::State_On);
    const bool hovered = option.state.testFlag(QStyle::State_MouseOver);
    const QRect indicator = style()->subElementRect(QStyle::SE_RadioButtonIndicator, &option, this);
    const QRect labelRect = style()->subElementRect(QStyle::SE_RadioButtonContents, &option, this);

    AnThemeColor borderRole = AnThemeColor::Border;
    if (enabled && (checked || focused || hovered))
    {
        borderRole = AnThemeColor::Primary;
    }

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    const QRectF indicatorRect(indicator);
    const qreal radius = qMin(indicatorRect.width(), indicatorRect.height()) / 2 - 0.5;
    if (radius > 0)
    {
        const AnThemeColor backgroundRole = enabled ? AnThemeColor::Input : AnThemeColor::Disabled;
        painter.setBrush(AnThemeManager::instance().color(backgroundRole));
        painter.setPen(QPen(AnThemeManager::instance().color(borderRole)));
        painter.drawEllipse(indicatorRect.center(), radius, radius);
        if (checked)
        {
            const AnThemeColor dotRole = enabled ? AnThemeColor::Primary : AnThemeColor::TextDisabled;
            painter.setPen(Qt::NoPen);
            painter.setBrush(AnThemeManager::instance().color(dotRole));
            painter.drawEllipse(indicatorRect.center(), radius * 0.55, radius * 0.55);
        }
    }

    option.rect = labelRect;
    if (option.icon.isNull())
    {
        const AnThemeColor textRole = enabled ? AnThemeColor::TextPrimary : AnThemeColor::TextDisabled;
        painter.setPen(AnThemeManager::instance().color(textRole));
        painter.drawText(option.rect, int(QStyle::visualAlignment(option.direction, Qt::AlignLeading | Qt::AlignVCenter)) | Qt::TextShowMnemonic, option.text);
    }
    else
    {
        style()->drawControl(QStyle::CE_RadioButtonLabel, &option, &painter, this);
    }

    if (focused)
    {
        const bool hasLabel = !option.text.isEmpty() || !option.icon.isNull();
        const QRect focusBounds = hasLabel ? labelRect : indicator.adjusted(-2, -2, 2, 2);
        const QRectF focusRect = QRectF(focusBounds.intersected(rect())).adjusted(0.5, 0.5, -0.5, -0.5);
        painter.setBrush(Qt::NoBrush);
        painter.setPen(QPen(AnThemeManager::instance().color(AnThemeColor::Primary), 1, Qt::DotLine));
        painter.drawRoundedRect(focusRect, 2, 2);
    }
}

void AnRadioButton::initialize()
{
    connect(&AnThemeManager::instance(), &AnThemeManager::themeChanged, this, QOverload<>::of(&AnRadioButton::update));
}
