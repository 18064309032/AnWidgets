#include <QEvent>
#include <QPainter>
#include <QPainterPath>
#include <QStyleOptionProgressBar>

#include "anprogressbar.h"
#include "anthememanager.h"

AnProgressBar::AnProgressBar(QWidget *parent)
    : QProgressBar(parent)
{
    setAlignment(Qt::AlignCenter);
    connect(this, &QProgressBar::valueChanged, this, QOverload<>::of(&AnProgressBar::update));
    connect(&AnThemeManager::instance(), &AnThemeManager::themeChanged, this, QOverload<>::of(&AnProgressBar::update));
}

int AnProgressBar::cornerRadius() const
{
    return m_cornerRadius;
}

void AnProgressBar::setCornerRadius(int radius)
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

void AnProgressBar::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QStyleOptionProgressBar option;
    initStyleOption(&option);
    const bool busy = option.minimum == 0 && option.maximum == 0;
    if (busy && isVisible() && !m_busyTimer.isActive())
    {
        m_busyFrame = 0;
        m_busyTimer.start(16, this);
    }
    else if (!busy && m_busyTimer.isActive())
    {
        m_busyTimer.stop();
    }

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    const bool vertical = option.orientation == Qt::Vertical;
    if (vertical)
    {
        painter.translate(0, height());
        painter.rotate(-90);
    }

    const QRectF track(0, 0, vertical ? height() : width(), vertical ? width() : height());
    const qreal radius = qMin(qreal(m_cornerRadius), qMin(track.width(), track.height()) / 2);
    QPainterPath trackPath;
    trackPath.addRoundedRect(track, radius, radius);
    painter.fillPath(trackPath, AnThemeManager::instance().color(AnThemeColor::Input));

    const bool reversed = option.invertedAppearance != (!vertical && option.direction == Qt::RightToLeft);
    QRectF fill = track;
    if (busy)
    {
        const qreal chunkWidth = track.width() * 0.3;
        const qreal offset = (track.width() + chunkWidth) * m_busyFrame / 100.0 - chunkWidth;
        fill.setWidth(chunkWidth);
        fill.moveLeft(reversed ? track.width() - offset - chunkWidth : offset);
    }
    else
    {
        qreal fraction = 0;
        if (option.progress >= option.minimum)
        {
            const qreal span = qreal(option.maximum) - option.minimum;
            fraction = span > 0 ? qBound(qreal(0), (qreal(option.progress) - option.minimum) / span, qreal(1)) : 1;
        }

        fill.setWidth(track.width() * fraction);
        if (reversed)
        {
            fill.moveRight(track.right());
        }
    }

    QPainterPath fillPath;
    if (fill.width() > 0)
    {
        const qreal fillRadius = qMin(radius, fill.width() / 2);
        fillPath.addRoundedRect(fill, fillRadius, fillRadius);
        fillPath = fillPath.intersected(trackPath);
        const AnThemeColor fillRole = isEnabled() ? AnThemeColor::Primary : AnThemeColor::Disabled;
        painter.fillPath(fillPath, AnThemeManager::instance().color(fillRole));
    }

    if (option.textVisible && !busy && !option.text.isEmpty())
    {
        const AnThemeColor textRole = isEnabled() ? AnThemeColor::TextPrimary : AnThemeColor::TextDisabled;
        painter.save();
        painter.setClipPath(trackPath.subtracted(fillPath), Qt::IntersectClip);
        if (vertical && !option.bottomToTop)
        {
            painter.translate(track.width(), track.height());
            painter.rotate(180);
        }
        painter.setPen(AnThemeManager::instance().color(textRole));
        painter.drawText(track, option.textAlignment, option.text);
        painter.restore();

        if (!fillPath.isEmpty())
        {
            painter.setClipPath(fillPath, Qt::IntersectClip);
            if (vertical && !option.bottomToTop)
            {
                painter.translate(track.width(), track.height());
                painter.rotate(180);
            }
            const AnThemeColor filledTextRole = isEnabled() ? AnThemeColor::TextOnAccent : textRole;
            painter.setPen(AnThemeManager::instance().color(filledTextRole));
            painter.drawText(track, option.textAlignment, option.text);
        }
    }
}

void AnProgressBar::timerEvent(QTimerEvent *event)
{
    if (event->timerId() != m_busyTimer.timerId())
    {
        QProgressBar::timerEvent(event);
        return;
    }

    if (minimum() != 0 || maximum() != 0 || !isVisible())
    {
        m_busyTimer.stop();
        return;
    }

    m_busyFrame = (m_busyFrame + 1) % 100;
    update();
}

void AnProgressBar::hideEvent(QHideEvent *event)
{
    m_busyTimer.stop();
    QProgressBar::hideEvent(event);
}
