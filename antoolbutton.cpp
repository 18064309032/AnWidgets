#include <QPainter>
#include <QPainterPath>
#include <QStyleOptionToolButton>

#include "anthememanager.h"
#include "antoolbutton.h"

AnToolButton::AnToolButton(QWidget *parent)
    : QToolButton(parent)
{
    setAttribute(Qt::WA_Hover);
    connect(&AnThemeManager::instance(), &AnThemeManager::themeChanged, this, QOverload<>::of(&AnToolButton::update));
}

int AnToolButton::cornerRadius() const
{
    return m_cornerRadius;
}

void AnToolButton::setCornerRadius(int radius)
{
    radius = qMax(0, radius);
    if (m_cornerRadius == radius)
    {
        return;
    }

    m_cornerRadius = radius;
    update();
    emit cornerRadiusChanged(m_cornerRadius);
}

QSize AnToolButton::sizeHint() const
{
    const QSize nativeSize = QToolButton::sizeHint();
    return QSize(qMax(32, nativeSize.width() + 8), qMax(32, nativeSize.height()));
}

void AnToolButton::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QStyleOptionToolButton option;
    initStyleOption(&option);
    const bool enabled = option.state.testFlag(QStyle::State_Enabled);
    const bool focused = enabled && option.state.testFlag(QStyle::State_HasFocus);
    const bool active = enabled && (option.state.testFlag(QStyle::State_Sunken) || option.state.testFlag(QStyle::State_On));
    const bool hovered = enabled && option.state.testFlag(QStyle::State_MouseOver);
    const bool split = option.features.testFlag(QStyleOptionToolButton::MenuButtonPopup);
    const AnThemeColor textRole = enabled ? AnThemeColor::TextPrimary : AnThemeColor::TextDisabled;

    AnThemeColor backgroundRole = enabled ? AnThemeColor::Input : AnThemeColor::Surface;
    if (active)
    {
        backgroundRole = AnThemeColor::Selection;
    }
    else if (hovered)
    {
        backgroundRole = AnThemeColor::Hover;
    }

    const AnThemeColor borderRole = focused ? AnThemeColor::Primary : AnThemeColor::Border;
    const QRectF bounds = QRectF(rect()).adjusted(0.5, 0.5, -0.5, -0.5);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setBrush(AnThemeManager::instance().color(backgroundRole));
    painter.setPen(QPen(AnThemeManager::instance().color(borderRole)));
    painter.drawRoundedRect(bounds, m_cornerRadius, m_cornerRadius);

    QStyleOptionToolButton label(option);
    if (split)
    {
        label.rect = style()->subControlRect(QStyle::CC_ToolButton, &option, QStyle::SC_ToolButton, this);
    }

    if (option.features.testFlag(QStyleOptionToolButton::HasMenu))
    {
        QRect arrowRect;
        if (split)
        {
            arrowRect = style()->subControlRect(QStyle::CC_ToolButton, &option, QStyle::SC_ToolButtonMenu, this);
            const int separator = option.direction == Qt::RightToLeft ? arrowRect.right() : arrowRect.left();
            painter.setPen(AnThemeManager::instance().color(AnThemeColor::Divider));
            painter.drawLine(separator, bounds.top() + 6, separator, bounds.bottom() - 6);
        }
        else
        {
            const int arrowWidth = qMin(option.rect.width(), qMax(16, style()->pixelMetric(QStyle::PM_MenuButtonIndicator, &option, this)));
            const QRect arrowArea(option.rect.right() - arrowWidth + 1, option.rect.top(), arrowWidth, option.rect.height());
            arrowRect = QStyle::visualRect(option.direction, option.rect, arrowArea);
            label.rect = QStyle::visualRect(option.direction, option.rect, option.rect.adjusted(0, 0, -arrowWidth, 0));
        }

        const QPointF center = QRectF(arrowRect).center();
        QPainterPath arrow;
        arrow.moveTo(center.x() - 3.5, center.y() - 1.5);
        arrow.lineTo(center.x(), center.y() + 2);
        arrow.lineTo(center.x() + 3.5, center.y() - 1.5);
        painter.setBrush(Qt::NoBrush);
        painter.setPen(QPen(AnThemeManager::instance().color(textRole), 1.5, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
        painter.drawPath(arrow);
    }

    if (label.toolButtonStyle == Qt::ToolButtonTextOnly)
    {
        painter.setPen(AnThemeManager::instance().color(textRole));
        painter.drawText(label.rect, Qt::AlignCenter | Qt::TextShowMnemonic, label.text);
        return;
    }

    style()->drawControl(QStyle::CE_ToolButtonLabel, &label, &painter, this);
}
