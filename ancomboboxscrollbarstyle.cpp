#include <QPainter>
#include <QPainterPath>
#include <QStyleOptionSlider>
#include <QWidget>

#include "ancomboboxscrollbarstyle.h"
#include "anthememanager.h"

AnComboBoxScrollBarStyle::AnComboBoxScrollBarStyle(QWidget *parent)
    : QProxyStyle(QStringLiteral("Fusion"))
{
    setParent(parent);
}

int AnComboBoxScrollBarStyle::pixelMetric(PixelMetric metric, const QStyleOption *option, const QWidget *widget) const
{
    if (metric == PM_ScrollBarExtent)
    {
        return 12;
    }
    if (metric == PM_ScrollBarSliderMin)
    {
        return 16;
    }

    return QProxyStyle::pixelMetric(metric, option, widget);
}

void AnComboBoxScrollBarStyle::drawComplexControl(ComplexControl control, const QStyleOptionComplex *option, QPainter *painter, const QWidget *widget) const
{
    const QStyleOptionSlider *scrollBar = qstyleoption_cast<const QStyleOptionSlider *>(option);
    if (control != CC_ScrollBar || !scrollBar)
    {
        QProxyStyle::drawComplexControl(control, option, painter, widget);
        return;
    }

    painter->save();
    painter->setRenderHint(QPainter::Antialiasing);
    painter->setPen(Qt::NoPen);
    if (scrollBar->subControls.testFlag(SC_ScrollBarSlider))
    {
        const QRectF sliderRect(subControlRect(CC_ScrollBar, scrollBar, SC_ScrollBarSlider, widget));
        const QRectF thumb = scrollBar->orientation == Qt::Vertical ? sliderRect.adjusted(3, 1, -3, -1) : sliderRect.adjusted(1, 3, -1, -3);
        const bool active = scrollBar->activeSubControls.testFlag(SC_ScrollBarSlider);
        const bool pressed = active && scrollBar->state.testFlag(State_Sunken);
        const AnThemeColor thumbRole = pressed ? AnThemeColor::Primary : (active ? AnThemeColor::TextSecondary : AnThemeColor::TextMuted);
        painter->setBrush(AnThemeManager::instance().withAlpha(thumbRole, active ? 210 : 140));
        painter->drawRoundedRect(thumb, 3, 3);
    }

    drawArrow(painter, scrollBar, SC_ScrollBarSubLine, widget);
    drawArrow(painter, scrollBar, SC_ScrollBarAddLine, widget);
    painter->restore();
}

void AnComboBoxScrollBarStyle::drawArrow(QPainter *painter, const QStyleOptionSlider *option, SubControl control, const QWidget *widget) const
{
    if (!option->subControls.testFlag(control))
    {
        return;
    }

    const QRect bounds = subControlRect(CC_ScrollBar, option, control, widget);
    if (bounds.isEmpty())
    {
        return;
    }

    const bool active = option->activeSubControls.testFlag(control);
    const QPointF center = QRectF(bounds).center();
    const bool vertical = option->orientation == Qt::Vertical;
    const bool forward = control == SC_ScrollBarAddLine;
    const qreal direction = forward != (!vertical && option->direction == Qt::RightToLeft) ? 1.0 : -1.0;

    painter->save();
    painter->translate(center);
    if (!vertical)
    {
        painter->rotate(-90);
    }
    QPainterPath arrow;
    arrow.moveTo(-2, -direction);
    arrow.lineTo(0, direction);
    arrow.lineTo(2, -direction);
    painter->setBrush(Qt::NoBrush);
    painter->setPen(QPen(AnThemeManager::instance().withAlpha(AnThemeColor::TextMuted, active ? 200 : 100), 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter->drawPath(arrow);
    painter->restore();
}
