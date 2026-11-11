#ifndef AN_TOOLBUTTON_H
#define AN_TOOLBUTTON_H

#include <QToolButton>

/**
 * @brief 提供主题配色和自绘外观的工具按钮。
 */
class AnToolButton : public QToolButton
{
    Q_OBJECT
    Q_PROPERTY(int cornerRadius READ cornerRadius WRITE setCornerRadius NOTIFY cornerRadiusChanged)

public:
    /**
     * @brief 构造工具按钮。
     * @param parent 管理生命周期的父对象，可为 nullptr。
     */
    explicit AnToolButton(QWidget *parent = nullptr);

    /**
     * @brief 获取按钮的圆角半径。
     * @return 圆角半径，单位为像素。
     */
    int cornerRadius() const;

    /**
     * @brief 设置按钮的圆角半径。
     * @param radius 圆角半径，单位为像素，负数按零处理。
     */
    void setCornerRadius(int radius);

    /**
     * @brief 获取包含内容留白的建议尺寸。
     * @return 工具按钮的建议尺寸。
     */
    QSize sizeHint() const override;

signals:
    /**
     * @brief 在圆角半径改变时发出通知。
     * @param radius 更新后的圆角半径。
     */
    void cornerRadiusChanged(int radius);

protected:
    /**
     * @brief 绘制按钮背景、边框、内容和菜单箭头。
     * @param event 当前绘制事件。
     */
    void paintEvent(QPaintEvent *event) override;

private:
    /**
     * @brief 按钮的圆角半径，单位为像素。
     */
    int m_cornerRadius = 6;
};

#endif
