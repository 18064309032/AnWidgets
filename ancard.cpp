#include <QPainter>

#include "ancard.h"
#include "anthememanager.h"

AnCard::AnCard(QWidget *parent)
    : QFrame(parent)
{
    setFrameShape(QFrame::NoFrame);
    setContentsMargins(16, 16, 16, 16);
    connect(&AnThemeManager::instance(), &AnThemeManager::themeChanged, this, QOverload<>::of(&AnCard::update));
}

int AnCard::cornerRadius() const
{
    return m_cornerRadius;
}

void AnCard::setCornerRadius(int radius)
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

void AnCard::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(QPen(AnThemeManager::instance().color(AnThemeColor::Divider)));
    painter.setBrush(AnThemeManager::instance().color(AnThemeColor::Surface));
    painter.drawRoundedRect(QRectF(rect()).adjusted(0.5, 0.5, -0.5, -0.5), m_cornerRadius, m_cornerRadius);
}
