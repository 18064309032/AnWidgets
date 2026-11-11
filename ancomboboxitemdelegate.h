#ifndef ANCOMBOBOXITEMDELEGATE_H
#define ANCOMBOBOXITEMDELEGATE_H

#include <QStyledItemDelegate>

/**
 * @brief 为下拉选项提供内边距和主题高亮。
 */
class AnComboBoxItemDelegate : public QStyledItemDelegate
{
public:
    /**
     * @brief 创建下拉选项绘制代理。
     * @param parent 管理生命周期的父对象。
     */
    explicit AnComboBoxItemDelegate(QObject *parent);

    /**
     * @brief 计算包含内边距的选项尺寸。
     * @param option 当前选项的绘制参数。
     * @param index 选项对应的模型索引。
     * @return 包含内边距的建议尺寸。
     */
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;

    /**
     * @brief 绘制圆角高亮及带留白的选项内容。
     * @param painter 当前绘制所用的画家。
     * @param option 当前选项的绘制参数。
     * @param index 选项对应的模型索引。
     */
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
};

#endif
