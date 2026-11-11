#include <QEvent>
#include <QPainter>

#include "anlistview.h"
#include "anlistviewitemdelegate.h"
#include "anthememanager.h"

AnListView::AnListView(QWidget *parent)
    : QListView(parent)
{
    setFrameShape(QFrame::NoFrame);
    viewport()->setAutoFillBackground(false);
    setViewportMargins(8, 8, 8, 8);
    setSpacing(2);
    setMouseTracking(true);
    setItemDelegate(new AnListViewItemDelegate(this));
    connect(&AnThemeManager::instance(), &AnThemeManager::themeChanged, this, &AnListView::refreshTheme);
    refreshTheme();
}

bool AnListView::event(QEvent *event)
{
    if (event->type() == QEvent::Paint)
    {
        const AnThemeColor borderRole = isEnabled() && hasFocus() ? AnThemeColor::Primary : AnThemeColor::Divider;
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);
        painter.setPen(QPen(AnThemeManager::instance().color(borderRole)));
        painter.setBrush(AnThemeManager::instance().color(AnThemeColor::Input));
        painter.drawRoundedRect(QRectF(rect()).adjusted(0.5, 0.5, -0.5, -0.5), 10, 10);
        return true;
    }

    if (event->type() == QEvent::FocusIn || event->type() == QEvent::FocusOut || event->type() == QEvent::EnabledChange)
    {
        update();
    }

    return QListView::event(event);
}

void AnListView::refreshTheme()
{
    viewport()->update();
    update();
}
