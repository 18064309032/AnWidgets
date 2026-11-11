#ifndef AN_CAPTIONBUTTON_H
#define AN_CAPTIONBUTTON_H

#include <QToolButton>

/**
 * @brief 自绘窗口标题栏上的平面按钮。
 */
class AnCaptionButton : public QToolButton
{
public:
    /**
     * @brief 定义标题栏按钮绘制的图标。
     */
    enum class Symbol
    {
        Minimize,
        Maximize,
        Restore,
        Close
    };

    /**
     * @brief 构造标题栏按钮。
     * @param symbol 按钮显示的图标。
     * @param parent 管理生命周期的父控件。
     */
    explicit AnCaptionButton(Symbol symbol, QWidget *parent = nullptr);

    /**
     * @brief 更换按钮图标。
     * @param symbol 要显示的图标。
     */
    void setSymbol(Symbol symbol);

    /**
     * @brief 更新由 Windows 非客户区消息提供的悬停状态。
     * @param hovered 鼠标是否悬停在按钮上。
     */
    void setNativeHovered(bool hovered);

protected:
    /**
     * @brief 绘制按钮图标和交互状态。
     * @param event 当前绘制事件。
     */
    void paintEvent(QPaintEvent *event) override;

private:
    /**
     * @brief 当前绘制的按钮图标。
     */
    Symbol m_symbol;

    /**
     * @brief 是否收到 Windows 的非客户区悬停通知。
     */
    bool m_nativeHovered = false;
};

#endif
