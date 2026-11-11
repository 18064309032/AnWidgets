#ifndef AN_CHECKBOX_H
#define AN_CHECKBOX_H

#include <QCheckBox>

/**
 * @brief 提供主题配色、圆角和三态外观的复选框。
 */
class AnCheckBox : public QCheckBox
{
    Q_OBJECT
    Q_PROPERTY(int cornerRadius READ cornerRadius WRITE setCornerRadius NOTIFY cornerRadiusChanged)

public:
    /**
     * @brief 构造复选框。
     * @param parent 管理生命周期的父对象，可为 nullptr。
     */
    explicit AnCheckBox(QWidget *parent = nullptr);

    /**
     * @brief 构造带文字的复选框。
     * @param text 复选框显示的文字。
     * @param parent 管理生命周期的父对象，可为 nullptr。
     */
    explicit AnCheckBox(const QString &text, QWidget *parent = nullptr);

    /**
     * @brief 获取方框的圆角半径。
     * @return 圆角半径，单位为像素。
     */
    int cornerRadius() const;

    /**
     * @brief 设置方框的圆角半径。
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
     * @brief 绘制方框、选中标记、标签和焦点提示。
     * @param event 当前绘制事件。
     */
    void paintEvent(QPaintEvent *event) override;

private:
    /**
     * @brief 连接主题变化通知。
     */
    void initialize();

    /**
     * @brief 方框的圆角半径，单位为像素。
     */
    int m_cornerRadius = 3;
};

#endif
