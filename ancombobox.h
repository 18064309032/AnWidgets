#ifndef AN_COMBOBOX_H
#define AN_COMBOBOX_H

#include <QComboBox>

/**
 * @brief 提供主题配色和圆角外观的下拉选择框。
 */
class AnComboBox : public QComboBox
{
    Q_OBJECT
    Q_PROPERTY(int cornerRadius READ cornerRadius WRITE setCornerRadius NOTIFY cornerRadiusChanged)

public:
    /**
     * @brief 构造下拉选择框。
     * @param parent 管理生命周期的父对象，可为 nullptr。
     */
    explicit AnComboBox(QWidget *parent = nullptr);

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

    /**
     * @brief 应用弹出列表的主题配色并展开选项。
     */
    void showPopup() override;

signals:
    /**
     * @brief 在圆角半径改变时发出通知。
     * @param radius 更新后的圆角半径。
     */
    void cornerRadiusChanged(int radius);

protected:
    /**
     * @brief 绘制圆角背景、边框、箭头和当前选项。
     * @param event 当前绘制事件。
     */
    void paintEvent(QPaintEvent *event) override;

    /**
     * @brief 绘制弹出层的圆角背景、细边框和阴影。
     * @param watched 当前接收事件的对象。
     * @param event 当前事件。
     * @return 已处理弹出层绘制事件时返回 true。
     */
    bool eventFilter(QObject *watched, QEvent *event) override;


private:
    /**
     * @brief 在弹层显示后按选项高度和屏幕空间调整尺寸。
     */
    void adjustPopupGeometry();


    /**
     * @brief 更新选择框和弹出列表的主题配色。
     */
    void refreshTheme();

    /**
     * @brief 下拉框的圆角半径，单位为像素。
     */
    int m_cornerRadius = 6;
};

#endif
