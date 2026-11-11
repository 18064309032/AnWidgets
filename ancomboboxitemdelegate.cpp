#include <QPainter>
#include <QStyleOptionViewItem>

#include "ancomboboxitemdelegate.h"
#include "anthememanager.h"

AnComboBoxItemDelegate::AnComboBoxItemDelegate(QObject *parent)
    : QStyledItemDelegate(parent)
{
}

QSize AnComboBoxItemDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    return QStyledItemDelegate::sizeHint(option, index) + QSize(16, 12);
}

void AnComboBoxItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QStyleOptionViewItem contentOption(option);
    contentOption.rect.adjust(8, 6, -8, -6);
    contentOption.state.setFlag(QStyle::State_HasFocus, false);
    contentOption.state.setFlag(QStyle::State_MouseOver, false);
    if (option.state.testFlag(QStyle::State_Selected))
    {
        painter->save();
        painter->setRenderHint(QPainter::Antialiasing);
        painter->setPen(Qt::NoPen);
        painter->setBrush(AnThemeManager::instance().color(AnThemeColor::Selection));
        painter->drawRoundedRect(QRectF(option.rect).adjusted(2, 1, -2, -1), 5, 5);
        painter->restore();
        contentOption.state.setFlag(QStyle::State_Selected, false);
    }
    else if (option.state.testFlag(QStyle::State_MouseOver) && option.state.testFlag(QStyle::State_Enabled))
    {
        painter->save();
        painter->setRenderHint(QPainter::Antialiasing);
        painter->setPen(Qt::NoPen);
        painter->setBrush(AnThemeManager::instance().color(AnThemeColor::Hover));
        painter->drawRoundedRect(QRectF(option.rect).adjusted(2, 1, -2, -1), 5, 5);
        painter->restore();
    }

    QStyleOptionViewItem labelOption(contentOption);
    initStyleOption(&labelOption, index);
    if (!labelOption.features.testFlag(QStyleOptionViewItem::HasDecoration) && !labelOption.features.testFlag(QStyleOptionViewItem::HasCheckIndicator) && !labelOption.features.testFlag(QStyleOptionViewItem::WrapText) && labelOption.backgroundBrush.style() == Qt::NoBrush)
    {
        painter->save();
        painter->setFont(labelOption.font);
        const AnThemeColor textRole = option.state.testFlag(QStyle::State_Enabled) ? AnThemeColor::TextPrimary : AnThemeColor::TextDisabled;
        painter->setPen(AnThemeManager::instance().color(textRole));
        const QString text = painter->fontMetrics().elidedText(labelOption.text, labelOption.textElideMode, labelOption.rect.width());
        const Qt::Alignment alignment = QStyle::visualAlignment(labelOption.direction, labelOption.displayAlignment);
        painter->drawText(labelOption.rect, int(alignment) | Qt::TextSingleLine, text);
        painter->restore();
        return;
    }

    QStyledItemDelegate::paint(painter, contentOption, index);
}
