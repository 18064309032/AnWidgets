#include <QPainter>
#include <QStyleOptionSlider>
#include <QtMath>

#include "anslider.h"
#include "anthememanager.h"

AnSlider::AnSlider(QWidget *parent)
    : AnSlider(Qt::Vertical, parent)
{
}

AnSlider::AnSlider(Qt::Orientation orientation, QWidget *parent)
    : QSlider(orientation, parent)
{
    connect(&AnThemeManager::instance(), &AnThemeManager::themeChanged, this, QOverload<>::of(&AnSlider::update));
}

void AnSlider::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QStyleOptionSlider option;
    initStyleOption(&option);
    const QRect handleRect = style()->subControlRect(QStyle::CC_Slider, &option, QStyle::SC_SliderHandle, this);
    if (handleRect.isEmpty())
    {
        return;
    }

    const QPointF center = QRectF(handleRect).center();
    if (option.minimum == option.maximum)
    {
        option.minimum = 0;
        option.maximum = 1;
    }

    option.sliderPosition = option.minimum;
    option.sliderValue = option.minimum;
    const QPointF start = QRectF(style()->subControlRect(QStyle::CC_Slider, &option, QStyle::SC_SliderHandle, this)).center();
    option.sliderPosition = option.maximum;
    option.sliderValue = option.maximum;
    const QPointF end = QRectF(style()->subControlRect(QStyle::CC_Slider, &option, QStyle::SC_SliderHandle, this)).center();

    const bool enabled = isEnabled();
    AnThemeColor accentRole = enabled ? AnThemeColor::Primary : AnThemeColor::Disabled;
    if (enabled && isSliderDown())
    {
        accentRole = AnThemeColor::Pressed;
    }

    const QColor accentColor = AnThemeManager::instance().color(accentRole);
    const QColor trackColor = AnThemeManager::instance().color(AnThemeColor::Border);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    if (tickPosition() != QSlider::NoTicks)
    {
        drawTicks(painter, handleRect, start, end);
    }

    painter.setPen(QPen(trackColor, 4, Qt::SolidLine, Qt::RoundCap));
    painter.drawLine(start, end);
    if (minimum() != maximum() && center != start)
    {
        painter.setPen(QPen(accentColor, 4, Qt::SolidLine, Qt::RoundCap));
        painter.drawLine(start, center);
    }

    const qreal handleRadius = qMin(qreal(7), qMin(handleRect.width(), handleRect.height()) / 2.0 - 0.5);
    if (handleRadius <= 0)
    {
        return;
    }

    painter.setPen(Qt::NoPen);
    painter.setBrush(accentColor);
    painter.drawEllipse(center, handleRadius, handleRadius);
    if (enabled && hasFocus() && handleRadius > 2)
    {
        painter.setBrush(Qt::NoBrush);
        painter.setPen(QPen(AnThemeManager::instance().color(AnThemeColor::TextOnAccent)));
        painter.drawEllipse(center, handleRadius - 2, handleRadius - 2);
    }
}

void AnSlider::drawTicks(QPainter &painter, const QRect &handleRect, const QPointF &start, const QPointF &end) const
{
    const bool horizontal = orientation() == Qt::Horizontal;
    const qreal length = horizontal ? qAbs(end.x() - start.x()) : qAbs(end.y() - start.y());
    const qint64 range = qint64(maximum()) - minimum();
    const int configuredInterval = tickInterval() > 0 ? tickInterval() : qMax(1, singleStep());
    const qint64 visibleIntervals = qMax(1, qFloor(length / 4));
    const qint64 stride = qMax(qint64(1), (range + visibleIntervals * configuredInterval - 1) / (visibleIntervals * configuredInterval)) * configuredInterval;
    const QRectF bounds(handleRect);
    const bool ticksBefore = tickPosition() == QSlider::TicksAbove || tickPosition() == QSlider::TicksBothSides;
    const bool ticksAfter = tickPosition() == QSlider::TicksBelow || tickPosition() == QSlider::TicksBothSides;

    painter.setPen(QPen(AnThemeManager::instance().color(AnThemeColor::Border)));
    for (qint64 offset = 0;; offset = qMin(offset + stride, range))
    {
        const qreal fraction = range > 0 ? qreal(offset) / range : 0;
        const QPointF position = start + (end - start) * fraction;
        if (ticksBefore)
        {
            const QLineF tick = horizontal ? QLineF(position.x(), bounds.top() - 3, position.x(), bounds.top() - 1) : QLineF(bounds.left() - 3, position.y(), bounds.left() - 1, position.y());
            painter.drawLine(tick);
        }
        if (ticksAfter)
        {
            const QLineF tick = horizontal ? QLineF(position.x(), bounds.bottom(), position.x(), bounds.bottom() + 2) : QLineF(bounds.right(), position.y(), bounds.right() + 2, position.y());
            painter.drawLine(tick);
        }
        if (offset == range)
        {
            break;
        }
    }
}
