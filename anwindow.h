#ifndef AN_WINDOW_H
#define AN_WINDOW_H

#include <QColor>
#include <QHBoxLayout>
#include <QPainterPath>
#include <QVBoxLayout>
#include <QWidget>

#include "antitlebar.h"

/**
 * @brief 提供主题配色和三个布局区域的通用窗口。
 */
class AnWindow : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief 构造窗口。
     * @param parent 管理生命周期的父控件，可为 nullptr。
     * @param flags Qt 窗口标志，默认创建独立窗口并使用系统标题栏。
     */
    explicit AnWindow(QWidget *parent = nullptr, Qt::WindowFlags flags = Qt::Window);

    /**
     * @brief 获取顶部操作区的布局。
     * @return 窗口管理的顶部布局。
     */
    QHBoxLayout *headerLayout() const;

    /**
     * @brief 获取中央内容区的布局。
     * @return 窗口管理的内容布局。
     */
    QVBoxLayout *contentLayout() const;

    /**
     * @brief 获取底部状态区的布局。
     * @return 窗口管理的底部布局。
     */
    QHBoxLayout *statusLayout() const;

    /**
     * @brief 启用或关闭自绘标题栏，建议在首次显示窗口之前调用。
     * @param enabled 是否启用无边框模式。
     */
    void setFramelessEnabled(bool enabled);

    /**
     * @brief 设置自绘窗口阴影的颜色和透明度。
     * @param color 阴影颜色。
     */
    void setShadowColor(const QColor &color);

    /**
     * @brief 设置普通状态下自绘窗口的边框颜色。
     * @param color 边框颜色，完全透明时隐藏边框。
     */
    void setBorderColor(const QColor &color);

    /**
     * @brief 设置阴影向窗口外扩散的距离。
     * @param radius 阴影半径，单位为像素，限制在 0 到 48。
     */
    void setShadowRadius(int radius);

    /**
     * @brief 设置普通窗口状态是否显示阴影。
     * @param enabled 是否显示阴影。
     */
    void setShadowEnabled(bool enabled);

protected:
    /**
     * @brief 初始化自绘窗口所需的原生窗口样式。
     * @param event 当前显示事件。
     */
    void showEvent(QShowEvent *event) override;

    /**
     * @brief 在窗口状态变化时刷新标题栏和阴影。
     * @param event 当前控件变化事件。
     */
    void changeEvent(QEvent *event) override;

    /**
     * @brief 处理 Windows 的标题栏、边缘缩放和贴靠命中测试。
     * @param eventType 原生事件类型。
     * @param message 平台消息。
     * @param result 处理后的原生返回值。
     * @return 消息已处理时返回 true。
     */
    bool nativeEvent(const QByteArray &eventType, void *message, long *result) override;

    /**
     * @brief 绘制窗口的主题背景。
     * @param event 当前绘制事件。
     */
    void paintEvent(QPaintEvent *event) override;


private:
    friend class AnCaptionButton;

    /**
     * @brief 普通窗口有阴影时的轮廓圆角半径。
     */
    static constexpr int FrameCornerRadius = 8;

    /**
     * @brief 获取窗口当前用于绘制外阴影的边距。
     * @return 外阴影边距，单位为逻辑像素。
     */
    int frameShadowMargin() const;

    /**
     * @brief 获取窗口主体在窗口坐标系中的可见轮廓。
     * @return 窗口背景与标题栏共用的绘制路径。
     */
    QPainterPath frameOutline() const;

    /**
     * @brief 将最大化窗口的原生边界对齐到显示器工作区。
     */
    void fitMaximizedWindow();

    /**
     * @brief 刷新窗口外缘用于绘制阴影的留白。
     */
    void updateFrameMargins();

    /**
     * @brief 应用于窗口的顶层布局。
     */
    QVBoxLayout *m_windowLayout = nullptr;

    /**
     * @brief 无边框模式使用的标题栏。
     */
    AnTitleBar *m_titleBar = nullptr;

    /**
     * @brief 自绘窗口阴影的颜色。
     */
    QColor m_shadowColor = QColor(0, 0, 0, 80);

    /**
     * @brief 自绘窗口的边框颜色。
     */
    QColor m_borderColor = Qt::transparent;

    /**
     * @brief 阴影留白的宽度，单位为像素。
     */
    int m_shadowRadius = 12;

    /**
     * @brief 是否启用自绘标题栏。
     */
    bool m_framelessEnabled = false;

    /**
     * @brief 是否绘制自定义阴影。
     */
    bool m_shadowEnabled = true;

    /**
     * @brief 管理顶部操作区的布局。
     */
    QHBoxLayout *m_headerLayout = nullptr;

    /**
     * @brief 管理中央内容区的布局。
     */
    QVBoxLayout *m_contentLayout = nullptr;

    /**
     * @brief 管理底部状态区的布局。
     */
    QHBoxLayout *m_statusLayout = nullptr;
};

#endif
