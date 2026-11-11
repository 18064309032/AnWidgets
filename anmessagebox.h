#ifndef ANMESSAGEBOX_H
#define ANMESSAGEBOX_H

#include <QMessageBox>

/**
 * @brief 提供主题配色的消息、确认和错误对话框。
 */
class AnMessageBox : public QMessageBox
{
    Q_OBJECT

public:
    /**
     * @brief 构造消息框。
     * @param parent 管理生命周期的父对象，可为 nullptr。
     */
    explicit AnMessageBox(QWidget *parent = nullptr);

    /**
     * @brief 构造带消息内容和标准按钮的消息框。
     * @param icon 消息类型对应的图标。
     * @param title 对话框标题。
     * @param text 对话框的正文。
     * @param buttons 要显示的标准按钮。
     * @param parent 管理生命周期的父对象，可为 nullptr。
     */
    AnMessageBox(Icon icon, const QString &title, const QString &text, StandardButtons buttons = NoButton, QWidget *parent = nullptr);

protected:
    /**
     * @brief 配置本次显示所用的标准按钮。
     * @param event 当前显示事件。
     */
    void showEvent(QShowEvent *event) override;

    /**
     * @brief 绘制消息框面板、阴影和标题。
     * @param event 当前绘制事件。
     */
    void paintEvent(QPaintEvent *event) override;

    /**
     * @brief 自绘消息框的按钮，保留 Qt 的按钮交互。
     * @param watched 事件接收对象。
     * @param event 当前事件。
     * @return 已处理按钮绘制事件时返回 true。
     */
    bool eventFilter(QObject *watched, QEvent *event) override;

    /**
     * @brief 从自绘标题区开始拖动消息框。
     * @param event 当前鼠标按下事件。
     */
    void mousePressEvent(QMouseEvent *event) override;

    /**
     * @brief 按鼠标位置移动消息框。
     * @param event 当前鼠标移动事件。
     */
    void mouseMoveEvent(QMouseEvent *event) override;

    /**
     * @brief 在鼠标左键释放时结束拖动。
     * @param event 当前鼠标释放事件。
     */
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    /**
     * @brief 连接主题通知并应用初始配色。
     */
    void initialize();

    /**
     * @brief 将当前主题应用到控件。
     */
    void refreshTheme();

    /**
     * @brief 鼠标按下位置相对于消息框左上角的偏移。
     */
    QPoint m_dragOffset;

    /**
     * @brief 是否正在拖动消息框。
     */
    bool m_dragging = false;
};

#endif
