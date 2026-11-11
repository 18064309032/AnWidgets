#ifndef AN_RADIOBUTTON_H
#define AN_RADIOBUTTON_H

#include <QRadioButton>

/**
 * @brief 提供主题配色和自绘选中标记的单选按钮。
 */
class AnRadioButton : public QRadioButton
{
    Q_OBJECT

public:
    /**
     * @brief 构造单选按钮。
     * @param parent 管理生命周期的父对象，可为 nullptr。
     */
    explicit AnRadioButton(QWidget *parent = nullptr);

    /**
     * @brief 构造带文字的单选按钮。
     * @param text 单选按钮显示的文字。
     * @param parent 管理生命周期的父对象，可为 nullptr。
     */
    explicit AnRadioButton(const QString &text, QWidget *parent = nullptr);

protected:
    /**
     * @brief 绘制单选圆环、选中标记、标签和焦点提示。
     * @param event 当前绘制事件。
     */
    void paintEvent(QPaintEvent *event) override;

private:
    /**
     * @brief 连接主题变化通知。
     */
    void initialize();
};

#endif
