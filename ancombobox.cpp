#include <QAbstractItemView>
#include <QEvent>
#include <QGuiApplication>
#include <QLayout>
#include <QListView>
#include <QScreen>
#include <QScrollBar>
#include <QPainter>
#include <QPainterPath>
#include <QStyleOptionComboBox>
#include <QTimer>

#include "ancombobox.h"
#include "ancomboboxitemdelegate.h"
#include "ancomboboxscrollbarstyle.h"
#include "anthememanager.h"

AnComboBox::AnComboBox(QWidget *parent)
    : QComboBox(parent)
{
    setMinimumHeight(32);
    connect(&AnThemeManager::instance(), &AnThemeManager::themeChanged, this, &AnComboBox::refreshTheme);
    setItemDelegate(new AnComboBoxItemDelegate(this));
    refreshTheme();
}

int AnComboBox::cornerRadius() const
{
    return m_cornerRadius;
}

void AnComboBox::setCornerRadius(int radius)
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

void AnComboBox::showPopup()
{
    QAbstractItemView *popupView = view();
    QWidget *popup = popupView->window();
    if (popup == this || !popup->windowFlags().testFlag(Qt::Popup) || !popup->layout())
    {
        QComboBox::showPopup();
        return;
    }

    if (!popup->testAttribute(Qt::WA_TranslucentBackground))
    {
        popup->setWindowFlags(popup->windowFlags() | Qt::FramelessWindowHint | Qt::NoDropShadowWindowHint);
        popup->setAttribute(Qt::WA_TranslucentBackground);
        popup->setAutoFillBackground(false);
        popup->layout()->setContentsMargins(8, 8, 8, 9);
        popupView->setFrameShape(QFrame::NoFrame);
        popupView->viewport()->setAutoFillBackground(false);
        QScrollBar *scrollBar = popupView->verticalScrollBar();
        scrollBar->setStyle(new AnComboBoxScrollBarStyle(scrollBar));
        scrollBar->setAttribute(Qt::WA_Hover);
        popup->installEventFilter(this);
    }

    QComboBox::showPopup();
}

void AnComboBox::adjustPopupGeometry()
{
    QAbstractItemView *popupView = view();
    QWidget *popup = popupView->window();
    if (!popup->isVisible() || !popup->layout())
    {
        return;
    }

    popup->layout()->activate();
    const QListView *listView = qobject_cast<const QListView *>(popupView);
    const int rowSpacing = listView ? listView->spacing() * 2 : 0;
    const int visibleLimit = qMax(1, maxVisibleItems());
    int rowsHeight = 0;
    int visibleRows = 0;
    bool hasMoreRows = false;
    for (int row = 0; row < count(); ++row)
    {
        if (listView && listView->isRowHidden(row))
        {
            continue;
        }
        if (visibleRows == visibleLimit)
        {
            hasMoreRows = true;
            break;
        }

        const int rowHeight = popupView->sizeHintForRow(row);
        rowsHeight += (rowHeight > 0 ? rowHeight : popupView->fontMetrics().height()) + rowSpacing;
        ++visibleRows;
    }
    if (visibleRows == 0)
    {
        return;
    }

    const int frameHeight = popup->height() - popupView->viewport()->height();
    const int desiredHeight = rowsHeight + frameHeight;
    const QPoint topLeft = mapToGlobal(QPoint(0, 0));
    const QPoint bottomLeft = mapToGlobal(QPoint(0, height()));
    const QScreen *screen = QGuiApplication::screenAt(mapToGlobal(rect().center()));
    if (!screen)
    {
        return;
    }

    const QRect available = screen->availableGeometry();
    const int spaceBelow = qMax(0, available.bottom() + 1 - bottomLeft.y());
    const int spaceAbove = qMax(0, topLeft.y() - available.top());
    const bool below = spaceBelow >= desiredHeight || (spaceAbove < desiredHeight && spaceBelow >= spaceAbove);
    const int popupHeight = qMin(desiredHeight, below ? spaceBelow : spaceAbove);
    if (popupHeight <= frameHeight)
    {
        return;
    }

    popup->setGeometry(popup->x(), below ? bottomLeft.y() : topLeft.y() - popupHeight, popup->width(), popupHeight);
    popup->layout()->activate();
    if (!hasMoreRows && popupHeight == desiredHeight)
    {
        popupView->scrollToTop();
    }
    else if (popupView->currentIndex().isValid())
    {
        popupView->scrollTo(popupView->currentIndex(), QAbstractItemView::EnsureVisible);
    }
}

bool AnComboBox::eventFilter(QObject *watched, QEvent *event)
{
    if (event->type() == QEvent::Show && watched == view()->window())
    {
        QTimer::singleShot(0, this, &AnComboBox::adjustPopupGeometry);
    }
    if (event->type() != QEvent::Paint || watched != view()->window())
    {
        return QComboBox::eventFilter(watched, event);
    }

    QWidget *popup = static_cast<QWidget *>(watched);
    const QRectF panel = QRectF(popup->rect()).adjusted(3.5, 4.5, -3.5, -4.5);
    QPainter painter(popup);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setCompositionMode(QPainter::CompositionMode_Source);
    painter.fillRect(popup->rect(), Qt::transparent);
    painter.setCompositionMode(QPainter::CompositionMode_SourceOver);

    const QColor shadowColor = AnThemeManager::instance().color(AnThemeColor::TextPrimary);
    painter.setBrush(Qt::NoBrush);
    painter.setPen(QPen(QColor(shadowColor.red(), shadowColor.green(), shadowColor.blue(), 12), 5));
    painter.drawRoundedRect(panel.translated(0, 1), 8, 8);
    painter.setPen(QPen(QColor(shadowColor.red(), shadowColor.green(), shadowColor.blue(), 20), 3));
    painter.drawRoundedRect(panel.translated(0, 1), 8, 8);

    painter.setPen(QPen(AnThemeManager::instance().color(AnThemeColor::Border)));
    painter.setBrush(AnThemeManager::instance().color(AnThemeColor::PopupBackground));
    painter.drawRoundedRect(panel, 8, 8);
    return true;
}

void AnComboBox::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QStyleOptionComboBox option;
    initStyleOption(&option);
    const bool focused = isEnabled() && (hasFocus() || option.state.testFlag(QStyle::State_On));
    const AnThemeColor backgroundRole = isEnabled() ? AnThemeColor::Input : AnThemeColor::Surface;
    const AnThemeColor borderRole = focused ? AnThemeColor::Primary : AnThemeColor::Border;
    const AnThemeColor arrowRole = isEnabled() ? AnThemeColor::TextSecondary : AnThemeColor::TextDisabled;
    const QRectF bounds = QRectF(rect()).adjusted(0.5, 0.5, -0.5, -0.5);
    const QRect arrowRect = style()->subControlRect(QStyle::CC_ComboBox, &option, QStyle::SC_ComboBoxArrow, this);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setBrush(AnThemeManager::instance().color(backgroundRole));
    painter.setPen(QPen(AnThemeManager::instance().color(borderRole)));
    painter.drawRoundedRect(bounds, m_cornerRadius, m_cornerRadius);

    if (arrowRect.isValid())
    {
        const QPointF center = QRectF(arrowRect).center();
        QPainterPath arrow;
        arrow.moveTo(center.x() - 3.5, center.y() - 1.5);
        arrow.lineTo(center.x(), center.y() + 2);
        arrow.lineTo(center.x() + 3.5, center.y() - 1.5);
        painter.setBrush(Qt::NoBrush);
        painter.setPen(QPen(AnThemeManager::instance().color(arrowRole), 1.5, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
        painter.drawPath(arrow);
    }

    if (!option.editable && option.rect.width() > 16)
    {
        option.rect.adjust(8, 0, -8, 0);
    }
    if (!option.editable && option.currentIcon.isNull())
    {
        const QRect textRect = style()->subControlRect(QStyle::CC_ComboBox, &option, QStyle::SC_ComboBoxEditField, this);
        const AnThemeColor textRole = isEnabled() ? AnThemeColor::TextPrimary : AnThemeColor::TextDisabled;
        painter.setPen(AnThemeManager::instance().color(textRole));
        const QString text = painter.fontMetrics().elidedText(option.currentText, Qt::ElideRight, textRect.width());
        painter.drawText(textRect, QStyle::visualAlignment(option.direction, Qt::AlignLeading | Qt::AlignVCenter), text);
        return;
    }

    style()->drawControl(QStyle::CE_ComboBoxLabel, &option, &painter, this);
}

void AnComboBox::refreshTheme()
{
    view()->viewport()->update();
    update();
}
