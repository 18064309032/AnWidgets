#include <QEvent>
#include <QLayout>
#include <QMouseEvent>
#include <QPainter>
#include <QPushButton>
#include <QStyleOptionButton>

#include "anmessagebox.h"
#include "anthememanager.h"

AnMessageBox::AnMessageBox(QWidget *parent)
    : QMessageBox(parent)
{
    initialize();
}

AnMessageBox::AnMessageBox(Icon icon, const QString &title, const QString &text, StandardButtons buttons, QWidget *parent)
    : QMessageBox(icon, title, text, buttons, parent)
{
    initialize();
}

void AnMessageBox::showEvent(QShowEvent *event)
{
    m_dragging = false;
    if (layout())
    {
        layout()->setContentsMargins(25, windowTitle().isEmpty() ? 25 : 57, 25, 25);
        layout()->setSpacing(12);
    }

    QMessageBox::showEvent(event);

    const QList<QAbstractButton *> messageButtons = buttons();
    for (QAbstractButton *button : messageButtons)
    {
        if (qobject_cast<QPushButton *>(button))
        {
            button->setMinimumSize(76, 32);
            button->setAttribute(Qt::WA_Hover);
            button->installEventFilter(this);
        }
    }
}

void AnMessageBox::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setCompositionMode(QPainter::CompositionMode_Source);
    painter.fillRect(rect(), Qt::transparent);
    painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
    const QRectF panel = QRectF(rect()).adjusted(9, 9, -9, -9);
    QColor shadowColor = AnThemeManager::instance().color(AnThemeColor::Background).darker(400);
    painter.setPen(Qt::NoPen);
    for (int spread = 6; spread >= 1; --spread)
    {
        shadowColor.setAlpha((8 - spread) / 2);
        painter.setBrush(shadowColor);
        painter.drawRoundedRect(panel.translated(0, 1).adjusted(-spread, -spread, spread, spread), 10 + spread, 10 + spread);
    }

    painter.setPen(QPen(AnThemeManager::instance().color(AnThemeColor::Divider)));
    painter.setBrush(AnThemeManager::instance().color(AnThemeColor::PopupBackground));
    painter.drawRoundedRect(panel, 10, 10);

    if (!windowTitle().isEmpty())
    {
        QFont titleFont = font();
        titleFont.setBold(true);
        painter.setFont(titleFont);
        painter.setPen(AnThemeManager::instance().color(AnThemeColor::TextPrimary));
        const QRect titleRect(25, 25, qMax(0, width() - 50), 24);
        const QString title = painter.fontMetrics().elidedText(windowTitle(), Qt::ElideRight, titleRect.width());
        painter.drawText(titleRect, Qt::AlignLeading | Qt::AlignVCenter, title);
    }
}

bool AnMessageBox::eventFilter(QObject *watched, QEvent *event)
{
    QPushButton *button = qobject_cast<QPushButton *>(watched);
    if (button && (event->type() == QEvent::HoverEnter || event->type() == QEvent::HoverLeave))
    {
        button->update();
    }
    if (!button || event->type() != QEvent::Paint)
    {
        return QMessageBox::eventFilter(watched, event);
    }

    const bool enabled = button->isEnabled();
    const bool primary = enabled && button->isDefault();
    AnThemeColor backgroundRole = primary ? AnThemeColor::Primary : AnThemeColor::Input;
    if (!enabled)
    {
        backgroundRole = AnThemeColor::Surface;
    }
    else if (button->isDown())
    {
        backgroundRole = primary ? AnThemeColor::Pressed : AnThemeColor::Selection;
    }
    else if (button->underMouse())
    {
        backgroundRole = primary ? AnThemeColor::Pressed : AnThemeColor::Hover;
    }

    const AnThemeColor borderRole = enabled && (button->hasFocus() || primary) ? AnThemeColor::Primary : AnThemeColor::Border;
    const AnThemeColor textRole = primary ? AnThemeColor::TextOnAccent : (enabled ? AnThemeColor::TextPrimary : AnThemeColor::TextDisabled);
    QPainter painter(button);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setBrush(AnThemeManager::instance().color(backgroundRole));
    painter.setPen(QPen(AnThemeManager::instance().color(borderRole)));
    painter.drawRoundedRect(QRectF(button->rect()).adjusted(0.5, 0.5, -0.5, -0.5), 6, 6);

    QStyleOptionButton label;
    label.initFrom(button);
    label.rect.adjust(8, 0, -8, 0);
    label.text = button->text();
    label.icon = button->icon();
    label.iconSize = button->iconSize();
    if (label.icon.isNull())
    {
        painter.setPen(AnThemeManager::instance().color(textRole));
        painter.drawText(label.rect, Qt::AlignCenter | Qt::TextShowMnemonic, label.text);
        return true;
    }

    button->style()->drawControl(QStyle::CE_PushButtonLabel, &label, &painter, button);
    return true;
}

void AnMessageBox::mousePressEvent(QMouseEvent *event)
{
    const QRect titleArea(9, 9, width() - 18, 49);
    if (event->button() == Qt::LeftButton && !windowTitle().isEmpty() && titleArea.contains(event->pos()))
    {
        m_dragOffset = event->globalPos() - frameGeometry().topLeft();
        m_dragging = true;
        event->accept();
        return;
    }

    QMessageBox::mousePressEvent(event);
}

void AnMessageBox::mouseMoveEvent(QMouseEvent *event)
{
    if (m_dragging && event->buttons().testFlag(Qt::LeftButton))
    {
        move(event->globalPos() - m_dragOffset);
        event->accept();
        return;
    }

    m_dragging = false;
    QMessageBox::mouseMoveEvent(event);
}

void AnMessageBox::mouseReleaseEvent(QMouseEvent *event)
{
    if (m_dragging && event->button() == Qt::LeftButton)
    {
        m_dragging = false;
        event->accept();
        return;
    }

    QMessageBox::mouseReleaseEvent(event);
}

void AnMessageBox::initialize()
{
    setWindowFlags(windowFlags() | Qt::FramelessWindowHint | Qt::NoDropShadowWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);
    setAutoFillBackground(false);
    connect(&AnThemeManager::instance(), &AnThemeManager::themeChanged, this, &AnMessageBox::refreshTheme);
    refreshTheme();
}

void AnMessageBox::refreshTheme()
{
    update();
    const QList<QAbstractButton *> messageButtons = buttons();
    for (QAbstractButton *button : messageButtons)
    {
        button->update();
    }
}
