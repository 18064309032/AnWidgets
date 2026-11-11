#include <QPainter>
#include <QStyleOptionMenuItem>

#include "anmenustyle.h"
#include "anthememanager.h"

AnMenuStyle::AnMenuStyle(AnMenu *parent)
    : QProxyStyle(QStringLiteral("Fusion"))
{
    setParent(parent);
}

int AnMenuStyle::pixelMetric(PixelMetric metric, const QStyleOption *option, const QWidget *widget) const
{
    if (metric == PM_MenuPanelWidth)
    {
        return 16;
    }
    if (metric == PM_MenuHMargin || metric == PM_MenuVMargin)
    {
        return 5;
    }
    if (metric == PM_SubMenuOverlap)
    {
        return -8;
    }

    return QProxyStyle::pixelMetric(metric, option, widget);
}

QSize AnMenuStyle::sizeFromContents(ContentsType type, const QStyleOption *option, const QSize &size, const QWidget *widget) const
{
    QSize result = QProxyStyle::sizeFromContents(type, option, size, widget);
    const QStyleOptionMenuItem *item = qstyleoption_cast<const QStyleOptionMenuItem *>(option);
    if (type == CT_MenuItem && item && item->menuItemType != QStyleOptionMenuItem::Separator)
    {
        result.setHeight(qMax(result.height() + 8, 34));
    }

    return result;
}

void AnMenuStyle::drawPrimitive(PrimitiveElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget) const
{
    if (element == PE_FrameMenu)
    {
        return;
    }
    if (element != PE_PanelMenu)
    {
        QProxyStyle::drawPrimitive(element, option, painter, widget);
        return;
    }

    const QRectF panel = QRectF(option->rect).adjusted(15.5, 15.5, -15.5, -15.5);
    painter->save();
    painter->setRenderHint(QPainter::Antialiasing);
    painter->setCompositionMode(QPainter::CompositionMode_Source);
    painter->fillRect(option->rect, Qt::transparent);
    painter->setCompositionMode(QPainter::CompositionMode_SourceOver);
    QColor shadowColor = AnThemeManager::instance().color(AnThemeColor::Background).darker(400);
    painter->setPen(Qt::NoPen);
    for (int spread = 7; spread >= 1; --spread)
    {
        shadowColor.setAlpha((9 - spread) / 2);
        painter->setBrush(shadowColor);
        const QRectF shadowBounds = panel.translated(0, 1).adjusted(-spread, -spread, spread, spread);
        painter->drawRoundedRect(shadowBounds, 10 + spread, 10 + spread);
    }

    painter->setBrush(AnThemeManager::instance().color(AnThemeColor::PopupBackground));
    painter->drawRoundedRect(panel, 10, 10);
    painter->restore();
}

void AnMenuStyle::drawControl(ControlElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget) const
{
    const QStyleOptionMenuItem *item = qstyleoption_cast<const QStyleOptionMenuItem *>(option);
    if (element != CE_MenuItem || !item)
    {
        QProxyStyle::drawControl(element, option, painter, widget);
        return;
    }

    if (item->menuItemType == QStyleOptionMenuItem::Separator && item->text.isEmpty() && item->icon.isNull())
    {
        painter->save();
        painter->setPen(AnThemeManager::instance().color(AnThemeColor::Divider));
        painter->drawLine(item->rect.left() + 4, item->rect.center().y(), item->rect.right() - 4, item->rect.center().y());
        painter->restore();
        return;
    }

    QStyleOptionMenuItem content(*item);

    if (item->state.testFlag(State_Selected) && item->state.testFlag(State_Enabled))
    {
        painter->save();
        painter->setRenderHint(QPainter::Antialiasing);
        painter->setPen(Qt::NoPen);
        painter->setBrush(AnThemeManager::instance().withAlpha(AnThemeColor::Selection, 115));
        painter->drawRoundedRect(QRectF(item->rect).adjusted(0, 1, 0, -1), 5, 5);
        painter->restore();
    }

    content.state.setFlag(State_Selected, false);
    content.state.setFlag(State_Sunken, false);
    content.state.setFlag(State_MouseOver, false);
    QProxyStyle::drawControl(element, &content, painter, widget);
}
