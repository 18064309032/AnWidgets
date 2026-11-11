#include <QPainter>
#include <QPainterPath>
#include <QStyleOptionButton>

#include "ancheckbox.h"
#include "anthememanager.h"

AnCheckBox::AnCheckBox(QWidget *parent)
    : QCheckBox(parent)
{
    initialize();
}

AnCheckBox::AnCheckBox(const QString &text, QWidget *parent)
    : QCheckBox(text, parent)
{
    initialize();
}

int AnCheckBox::cornerRadius() const
{
    return m_cornerRadius;
}

void AnCheckBox::setCornerRadius(int radius)
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

void AnCheckBox::initialize()
{
    connect(&AnThemeManager::instance(), &AnThemeManager::themeChanged, this, QOverload<>::of(&AnCheckBox::update));
}

void AnCheckBox::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QStyleOptionButton option;
    initStyleOption(&option);
    const bool enabled = option.state.testFlag(QStyle::State_Enabled);
    const bool focused = enabled && option.state.testFlag(QStyle::State_HasFocus);
    const Qt::CheckState state = checkState();
    const QRect indicator = style()->subElementRect(QStyle::SE_CheckBoxIndicator, &option, this);
    const QRect labelRect = style()->subElementRect(QStyle::SE_CheckBoxContents, &option, this);

    AnThemeColor backgroundRole = AnThemeColor::Input;
    AnThemeColor borderRole = AnThemeColor::Border;
    if (!enabled)
    {
        backgroundRole = AnThemeColor::Disabled;
    }
    else if (state != Qt::Unchecked)
    {
        backgroundRole = AnThemeColor::Primary;
        if (option.state.testFlag(QStyle::State_Sunken))
        {
            backgroundRole = AnThemeColor::Pressed;
        }
        borderRole = backgroundRole;
    }
    else if (focused || option.state.testFlag(QStyle::State_MouseOver))
    {
        borderRole = AnThemeColor::Primary;
    }

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    const QRectF box = QRectF(indicator).adjusted(0.5, 0.5, -0.5, -0.5);
    if (!box.isEmpty())
    {
        const qreal radius = qMin(qreal(m_cornerRadius), qMin(box.width(), box.height()) / 2);
        painter.setBrush(AnThemeManager::instance().color(backgroundRole));
        painter.setPen(QPen(AnThemeManager::instance().color(borderRole)));
        painter.drawRoundedRect(box, radius, radius);

        if (state != Qt::Unchecked)
        {
            const AnThemeColor markRole = enabled ? AnThemeColor::TextOnAccent : AnThemeColor::TextDisabled;
            painter.setBrush(Qt::NoBrush);
            painter.setPen(QPen(AnThemeManager::instance().color(markRole), 1.5, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
            if (state == Qt::PartiallyChecked)
            {
                painter.drawLine(QPointF(box.left() + box.width() * 0.25, box.center().y()), QPointF(box.right() - box.width() * 0.25, box.center().y()));
            }
            else
            {
                QPainterPath checkMark;
                checkMark.moveTo(box.left() + box.width() * 0.23, box.top() + box.height() * 0.52);
                checkMark.lineTo(box.left() + box.width() * 0.43, box.top() + box.height() * 0.72);
                checkMark.lineTo(box.left() + box.width() * 0.77, box.top() + box.height() * 0.30);
                painter.drawPath(checkMark);
            }
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
        style()->drawControl(QStyle::CE_CheckBoxLabel, &option, &painter, this);
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
