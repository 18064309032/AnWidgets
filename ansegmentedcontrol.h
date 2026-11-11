#ifndef AN_SEGMENTEDCONTROL_H
#define AN_SEGMENTEDCONTROL_H

#include <QTabBar>

/**
 * @brief 用于在一组互斥选项之间切换的分段选择器。
 */
class AnSegmentedControl : public QTabBar
{
    Q_OBJECT

public:
    /**
     * @brief 构造分段选择器。
     * @param parent 管理生命周期的父对象，可为 nullptr。
     */
    explicit AnSegmentedControl(QWidget *parent = nullptr);

protected:
    /**
     * @brief 计算包含文字留白的分段尺寸。
     * @param index 当前选项的索引。
     * @return 分段的建议尺寸。
     */
    QSize tabSizeHint(int index) const override;

    /**
     * @brief 绘制轨道和各分段的交互状态。
     * @param event 当前绘制事件。
     */
    void paintEvent(QPaintEvent *event) override;
};

#endif
