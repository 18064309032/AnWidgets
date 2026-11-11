#include <QEvent>
#include <QPainter>

#include "anlineedit.h"
#include "aneditorcoloreffect_p.h"
#include "anthememanager.h"

AnLineEdit::AnLineEdit(QWidget *parent)
    : QLineEdit(parent)
{
    initialize();
}

AnLineEdit::AnLineEdit(const QString &text, QWidget *parent)
    : QLineEdit(text, parent)
{
    initialize();
}

int AnLineEdit::cornerRadius() const
{
    return m_cornerRadius;
}

void AnLineEdit::setCornerRadius(int radius)
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

AnLineEdit::ValidationState AnLineEdit::validationState() const
{
    return m_validationState;
}

void AnLineEdit::setValidationState(ValidationState state)
{
    if (m_validationState == state)
    {
        return;
    }

    m_validationState = state;
    update();
    emit validationStateChanged(m_validationState);
}

QColor AnLineEdit::borderColor() const
{
    AnThemeColor borderRole = AnThemeColor::Border;
    if (isEnabled())
    {
        if (m_validationState == ValidationState::Warning)
        {
            borderRole = AnThemeColor::Warning;
        }
        else if (m_validationState == ValidationState::Error)
        {
            borderRole = AnThemeColor::Error;
        }
        else if (hasFocus())
        {
            borderRole = AnThemeColor::Primary;
        }
    }

    return AnThemeManager::instance().color(borderRole);
}

void AnLineEdit::paintEvent(QPaintEvent *event)
{
    QLineEdit::paintEvent(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setBrush(Qt::NoBrush);
    painter.setPen(QPen(borderColor()));
    painter.drawRoundedRect(QRectF(rect()).adjusted(0.5, 0.5, -0.5, -0.5), m_cornerRadius, m_cornerRadius);
}

bool AnLineEdit::event(QEvent *event)
{
    if (event->type() == QEvent::FocusIn || event->type() == QEvent::FocusOut || event->type() == QEvent::EnabledChange)
    {
        update();
    }

    return QLineEdit::event(event);
}

void AnLineEdit::initialize()
{
    setFrame(false);
    setGraphicsEffect(new AnEditorColorEffect(this));
    setTextMargins(10, 6, 10, 6);
    connect(&AnThemeManager::instance(), &AnThemeManager::themeChanged, this, QOverload<>::of(&AnLineEdit::update));
}
