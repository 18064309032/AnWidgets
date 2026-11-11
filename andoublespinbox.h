#ifndef AN_DOUBLESPINBOX_H
#define AN_DOUBLESPINBOX_H

#include <QDoubleSpinBox>

/**
 * @brief 提供主题配色和圆角外观的小数输入框。
 */
class AnDoubleSpinBox : public QDoubleSpinBox
{
    Q_OBJECT
    Q_PROPERTY(int cornerRadius READ cornerRadius WRITE setCornerRadius NOTIFY cornerRadiusChanged)

public:
    /**
     * @brief 构造小数输入框。
     * @param parent 管理生命周期的父对象，可为 nullptr。
     */
    explicit AnDoubleSpinBox(QWidget *parent = nullptr);

    /**
     * @brief 获取圆角半径。
     * @return 圆角半径，单位为像素。
     */
    int cornerRadius() const;

    /**
     * @brief 设置圆角半径。
     * @param radius 圆角半径，单位为像素，负数按零处理。
     */
    void setCornerRadius(int radius);

signals:
    /**
     * @brief 在圆角半径改变时发出通知。
     * @param radius 更新后的圆角半径。
     */
    void cornerRadiusChanged(int radius);

protected:
    /**
     * @brief 绘制圆角背景、边框和步进按钮。
     * @param event 当前绘制事件。
     */
    void paintEvent(QPaintEvent *event) override;

private:
    /**
     * @brief 在 Qt 样式提供的按钮区域绘制步进符号。
     * @param painter 当前绘制所用的画家。
     * @param option 当前输入框的样式参数。
     * @param up 是否绘制增加数值的按钮。
     */
    void drawStepButton(QPainter &painter, const QStyleOptionSpinBox &option, bool up) const;

    /**
     * @brief 输入框的圆角半径，单位为像素。
     */
    int m_cornerRadius = 6;
};

#endif
