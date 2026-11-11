#include <QEvent>
#include <QPainter>

#include "anplaintextedit.h"
#include "aneditorcoloreffect_p.h"
#include "anthememanager.h"

AnPlainTextEdit::AnPlainTextEdit(QWidget *parent)
    : QPlainTextEdit(parent)
{
    setFrameShape(QFrame::NoFrame);
    setViewportMargins(8, 5, 8, 5);
    viewport()->setGraphicsEffect(new AnEditorColorEffect(viewport()));
    connect(&AnThemeManager::instance(), &AnThemeManager::themeChanged, this, QOverload<>::of(&AnPlainTextEdit::update));
}

bool AnPlainTextEdit::event(QEvent *event)
{
    if (event->type() == QEvent::Paint)
    {
        const AnThemeColor backgroundRole = isEnabled() ? AnThemeColor::Input : AnThemeColor::Surface;
        const AnThemeColor borderRole = isEnabled() && hasFocus() ? AnThemeColor::Primary : AnThemeColor::Border;
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);
        painter.setBrush(AnThemeManager::instance().color(backgroundRole));
        painter.setPen(QPen(AnThemeManager::instance().color(borderRole)));
        painter.drawRoundedRect(QRectF(rect()).adjusted(0.5, 0.5, -0.5, -0.5), 6, 6);
        return true;
    }

    if (event->type() == QEvent::EnabledChange || event->type() == QEvent::FocusIn || event->type() == QEvent::FocusOut)
    {
        update();
    }

    return QPlainTextEdit::event(event);
}
