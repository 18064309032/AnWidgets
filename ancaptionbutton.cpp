#include <QPainter>
#include <QPainterPath>
#include <QStyleOptionToolButton>

#include "ancaptionbutton.h"
#include "anthememanager.h"
#include "anwindow.h"

AnCaptionButton::AnCaptionButton(Symbol symbol, QWidget *parent)
    : QToolButton(parent)
    , m_symbol(symbol)
{
    setFixedWidth(44);
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
    setFocusPolicy(Qt::TabFocus);
    setAttribute(Qt::WA_Hover);
    connect(&AnThemeManager::instance(), &AnThemeManager::themeChanged, this, QOverload<>::of(&AnCaptionButton::update));
}

void AnCaptionButton::setSymbol(Symbol symbol)
{
    if (m_symbol != symbol)
    {
        m_symbol = symbol;
        update();
    }
}

void AnCaptionButton::setNativeHovered(bool hovered)
{
    if (m_nativeHovered != hovered)
    {
        m_nativeHovered = hovered;
        update();
    }
}

void AnCaptionButton::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QStyleOptionToolButton option;
    initStyleOption(&option);
    const bool hovered = isEnabled() && (m_nativeHovered || underMouse() || isDown());
    const bool focused = hasFocus() && option.state.testFlag(QStyle::State_KeyboardFocusChange);
    QColor iconColor = AnThemeManager::instance().color(isEnabled() ? AnThemeColor::TextPrimary : AnThemeColor::TextDisabled);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    if (hovered || focused)
    {
        painter.setBrush(Qt::NoBrush);
        if (hovered && m_symbol == Symbol::Close)
        {
            painter.setBrush(isDown() ? QColor(196, 13, 32) : QColor(232, 17, 35));
            iconColor = Qt::white;
        }
        else if (hovered)
        {
            painter.setBrush(AnThemeManager::instance().withAlpha(AnThemeColor::TextPrimary, isDown() ? 30 : 17));
        }

        const QRectF bounds(rect());
        if (hovered)
        {
            painter.setPen(Qt::NoPen);
            const AnWindow *frame = qobject_cast<const AnWindow *>(window());
            if (frame && frame->frameShadowMargin() > 0)
            {
                const QPoint origin = mapTo(frame, QPoint());
                painter.setClipPath(frame->frameOutline().translated(-origin), Qt::IntersectClip);
            }
            painter.drawRect(bounds);
        }
        if (focused)
        {
            painter.setBrush(Qt::NoBrush);
            painter.setPen(QPen(AnThemeManager::instance().color(AnThemeColor::Primary)));
            painter.drawRoundedRect(bounds.adjusted(3.5, 3.5, -3.5, -3.5), 3, 3);
        }
    }

    painter.translate(QRectF(rect()).center());
    painter.setBrush(Qt::NoBrush);
    painter.setPen(QPen(iconColor, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    switch (m_symbol)
    {
    case Symbol::Minimize:
    {
        painter.drawLine(QPointF(-4.5, 0.5), QPointF(4.5, 0.5));
        break;
    }
    case Symbol::Maximize:
    {
        painter.drawRect(QRectF(-4.5, -4.5, 9, 9));
        break;
    }
    case Symbol::Restore:
    {
        painter.drawRect(QRectF(-4.5, -2.5, 7, 7));
        QPainterPath back;
        back.moveTo(-2.5, -2.5);
        back.lineTo(-2.5, -4.5);
        back.lineTo(4.5, -4.5);
        back.lineTo(4.5, 2.5);
        back.lineTo(2.5, 2.5);
        painter.drawPath(back);
        break;
    }
    case Symbol::Close:
    {
        const QTransform deviceTransform = painter.deviceTransform();
        const QPointF deviceCenter = deviceTransform.map(QPointF(0, 0));
        const QPointF alignedCenter(qRound(deviceCenter.x()), qRound(deviceCenter.y()));
        painter.translate(deviceTransform.inverted().map(alignedCenter));
        const qreal tip = 5;
        const qreal inset = 4;
        const qreal intersection = tip - inset;
        QPainterPath closeIcon;
        closeIcon.moveTo(-tip, -inset);
        closeIcon.lineTo(-inset, -tip);
        closeIcon.lineTo(0, -intersection);
        closeIcon.lineTo(inset, -tip);
        closeIcon.lineTo(tip, -inset);
        closeIcon.lineTo(intersection, 0);
        closeIcon.lineTo(tip, inset);
        closeIcon.lineTo(inset, tip);
        closeIcon.lineTo(0, intersection);
        closeIcon.lineTo(-inset, tip);
        closeIcon.lineTo(-tip, inset);
        closeIcon.lineTo(-intersection, 0);
        closeIcon.closeSubpath();
        painter.fillPath(closeIcon, iconColor);
        break;
    }
    }
}
