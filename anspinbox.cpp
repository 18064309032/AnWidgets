#include <QLineEdit>
#include <QPainter>
#include <QPainterPath>
#include <QStyleOptionSpinBox>

#include "anspinbox.h"
#include "anspinboxeditorstyle.h"
#include "aneditorcoloreffect_p.h"
#include "anthememanager.h"

AnSpinBox::AnSpinBox(QWidget *parent)
    : QSpinBox(parent)
{
    setMinimumHeight(32);
    setAttribute(Qt::WA_Hover);
    setStyle(new AnSpinBoxEditorStyle(this));
    lineEdit()->setFrame(false);
    lineEdit()->setGraphicsEffect(new AnEditorColorEffect(lineEdit()));
    lineEdit()->setTextMargins(2, 0, 2, 0);
    connect(&AnThemeManager::instance(), &AnThemeManager::themeChanged, this, QOverload<>::of(&AnSpinBox::update));
}

int AnSpinBox::cornerRadius() const
{
    return m_cornerRadius;
}

void AnSpinBox::setCornerRadius(int radius)
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

void AnSpinBox::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QStyleOptionSpinBox option;
    initStyleOption(&option);
    const bool enabled = isEnabled();
    const AnThemeColor backgroundRole = enabled ? AnThemeColor::Input : AnThemeColor::Surface;
    const AnThemeColor borderRole = enabled && option.state.testFlag(QStyle::State_HasFocus) ? AnThemeColor::Primary : AnThemeColor::Border;
    const QRectF bounds = QRectF(rect()).adjusted(0.5, 0.5, -0.5, -0.5);
    const qreal radius = qMin(qreal(m_cornerRadius), qMin(bounds.width(), bounds.height()) / 2);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setBrush(AnThemeManager::instance().color(backgroundRole));
    painter.setPen(QPen(AnThemeManager::instance().color(borderRole)));
    painter.drawRoundedRect(bounds, radius, radius);

    if (option.buttonSymbols != QAbstractSpinBox::NoButtons)
    {
        drawStepButton(painter, option, true);
        drawStepButton(painter, option, false);
    }
}

void AnSpinBox::drawStepButton(QPainter &painter, const QStyleOptionSpinBox &option, bool up) const
{
    const QStyle::SubControl control = up ? QStyle::SC_SpinBoxUp : QStyle::SC_SpinBoxDown;
    if (!option.subControls.testFlag(control))
    {
        return;
    }

    const QRect buttonRect = style()->subControlRect(QStyle::CC_SpinBox, &option, control, this);
    if (buttonRect.isEmpty())
    {
        return;
    }

    const QAbstractSpinBox::StepEnabledFlag step = up ? QAbstractSpinBox::StepUpEnabled : QAbstractSpinBox::StepDownEnabled;
    const bool enabled = isEnabled() && !isReadOnly() && option.stepEnabled.testFlag(step);
    const bool active = enabled && option.activeSubControls.testFlag(control);
    const bool pressed = active && option.state.testFlag(QStyle::State_Sunken);
    if (active)
    {
        const QColor feedbackColor = pressed ? AnThemeManager::instance().withAlpha(AnThemeColor::Primary, 65) : AnThemeManager::instance().withAlpha(AnThemeColor::Primary, 30);
        painter.setPen(Qt::NoPen);
        painter.setBrush(feedbackColor);
        painter.drawRoundedRect(QRectF(buttonRect).adjusted(1, 1, -1, -1), 4, 4);
    }

    const AnThemeColor iconRole = !enabled ? AnThemeColor::TextDisabled : (active ? AnThemeColor::Primary : AnThemeColor::TextSecondary);
    painter.setBrush(Qt::NoBrush);
    painter.setPen(QPen(AnThemeManager::instance().color(iconRole), 1.5, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));

    const QPointF center = QRectF(buttonRect).center() + QPointF(0, pressed ? 0.5 : 0);
    const qreal halfWidth = qMin(3.0, buttonRect.width() / 4.0);
    const qreal halfHeight = qMin(1.5, buttonRect.height() / 4.0);
    if (option.buttonSymbols == QAbstractSpinBox::PlusMinus)
    {
        painter.drawLine(center - QPointF(halfWidth, 0), center + QPointF(halfWidth, 0));
        if (up)
        {
            painter.drawLine(center - QPointF(0, halfWidth), center + QPointF(0, halfWidth));
        }
    }
    else
    {
        const qreal direction = up ? -1.0 : 1.0;
        QPainterPath chevron;
        chevron.moveTo(center.x() - halfWidth, center.y() - direction * halfHeight);
        chevron.lineTo(center.x(), center.y() + direction * halfHeight);
        chevron.lineTo(center.x() + halfWidth, center.y() - direction * halfHeight);
        painter.drawPath(chevron);
    }
}
