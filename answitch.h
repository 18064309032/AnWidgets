#ifndef ANSWITCH_H
#define ANSWITCH_H

#include <QAbstractButton>

/**
 * @brief 提供主题配色的布尔开关，沿用按钮的点击和键盘操作。
 */
class AnSwitch : public QAbstractButton
{
    Q_OBJECT

public:
    /**
     * @brief 构造开关。
     * @param parent 管理生命周期的父对象，可为 nullptr。
     */
    explicit AnSwitch(QWidget *parent = nullptr);

    /**
     * @brief 获取开关及其文字的建议尺寸。
     * @return 开关的建议尺寸。
     */
    QSize sizeHint() const override;

protected:
    /**
     * @brief 绘制开关轨道、滑块、文字和焦点提示。
     * @param event 当前绘制事件。
     */
    void paintEvent(QPaintEvent *event) override;
};

#endif
