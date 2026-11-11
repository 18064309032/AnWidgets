#include <QPainter>
#include <QStyle>
#include <QStyleOptionToolButton>

#include "annavigationitem.h"
#include "anthememanager.h"

AnNavigationItem::AnNavigationItem(QWidget *parent)
    : QToolButton(parent)
{
    setFocusPolicy(Qt::StrongFocus);
    setFixedHeight(36);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    connect(&AnThemeManager::instance(), &AnThemeManager::themeChanged, this, QOverload<>::of(&AnNavigationItem::update));
}

void AnNavigationItem::setChildItem(bool child)
{
    m_childItem = child;
    update();
}

void AnNavigationItem::setBranchIndicator(bool enabled)
{
    m_branchIndicator = enabled;
    update();
}

void AnNavigationItem::setToggleIndicator(bool enabled)
{
    m_toggleIndicator = enabled;
    update();
}

void AnNavigationItem::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    const QRectF bounds = QRectF(rect()).adjusted(0.5, 0.5, -0.5, -0.5);
    QStyleOptionToolButton option;
    initStyleOption(&option);
    const bool selected = isChecked() && !m_branchIndicator;
    const bool keyboardFocus = hasFocus() && option.state.testFlag(QStyle::State_KeyboardFocusChange);
    if (isEnabled() && (selected || underMouse() || keyboardFocus))
    {
        painter.setBrush(AnThemeManager::instance().withAlpha(AnThemeColor::TextPrimary, selected ? 18 : 10));
        painter.setPen(keyboardFocus ? QPen(AnThemeManager::instance().color(AnThemeColor::Primary)) : QPen(Qt::NoPen));
        painter.drawRoundedRect(bounds, 7, 7);
    }
    if (isEnabled() && selected)
    {
        const int leadingEdge = layoutDirection() == Qt::RightToLeft ? width() - 5 : 2;
        painter.setPen(Qt::NoPen);
        painter.setBrush(AnThemeManager::instance().color(AnThemeColor::Primary));
        painter.drawRoundedRect(QRectF(leadingEdge, (height() - 18) / 2.0, 3, 18), 1.5, 1.5);
    }

    const bool iconOnly = toolButtonStyle() == Qt::ToolButtonIconOnly;
    const int indent = m_childItem ? 14 : 0;
    const QRect logicalIconRect(iconOnly ? (width() - 16) / 2 : 10 + indent, (height() - 16) / 2, 16, 16);
    const QRect iconRect = QStyle::visualRect(layoutDirection(), rect(), logicalIconRect);
    const AnThemeColor textRole = isEnabled() ? AnThemeColor::TextPrimary : AnThemeColor::TextDisabled;
    painter.setPen(AnThemeManager::instance().color(textRole));
    if (!icon().isNull())
    {
        icon().paint(&painter, iconRect, Qt::AlignCenter, isEnabled() ? QIcon::Normal : QIcon::Disabled);
    }
    else if (m_toggleIndicator)
    {
        const QPointF center = QRectF(iconRect).center();
        painter.setPen(QPen(AnThemeManager::instance().color(textRole), 1.5, Qt::SolidLine, Qt::RoundCap));
        for (int offset = -4; offset <= 4; offset += 4)
        {
            painter.drawLine(center + QPointF(-5, offset), center + QPointF(5, offset));
        }
    }
    else
    {
        painter.drawText(iconRect, Qt::AlignCenter, text().left(1));
    }

    if (!iconOnly)
    {
        const QRect textRect = QStyle::visualRect(layoutDirection(), rect(), rect().adjusted(36 + indent, 0, m_branchIndicator ? -25 : -8, 0));
        const QString label = painter.fontMetrics().elidedText(text(), Qt::ElideRight, textRect.width());
        painter.drawText(textRect, QStyle::visualAlignment(layoutDirection(), Qt::AlignLeading | Qt::AlignVCenter), label);
        if (m_branchIndicator)
        {
            const QPoint center = QStyle::visualRect(layoutDirection(), rect(), QRect(width() - 16, height() / 2, 1, 1)).center();
            const int direction = layoutDirection() == Qt::RightToLeft ? -1 : 1;
            painter.setPen(QPen(AnThemeManager::instance().color(isEnabled() ? AnThemeColor::TextSecondary : AnThemeColor::TextDisabled), 1.4, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
            if (isChecked())
            {
                const QPoint points[] = {center + QPoint(-3, -1), center + QPoint(0, 2), center + QPoint(3, -1)};
                painter.drawPolyline(points, 3);
            }
            else
            {
                const QPoint points[] = {center + QPoint(-direction, -3), center + QPoint(2 * direction, 0), center + QPoint(-direction, 3)};
                painter.drawPolyline(points, 3);
            }
        }
    }
}
