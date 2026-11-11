#ifndef AN_TITLEBAR_H
#define AN_TITLEBAR_H

#include <QLabel>
#include <QWidget>

#include "ancaptionbutton.h"

/**
 * @brief 显示窗口图标、标题和自绘窗口按钮。
 */
class AnTitleBar : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief 构造标题栏。
     * @param window 标题栏所属的窗口。
     */
    explicit AnTitleBar(QWidget *window);

    /**
     * @brief 判断标题栏的局部坐标能否用于拖动窗口。
     * @param position 标题栏内的位置。
     * @return 可拖动时返回 true。
     */
    bool isDragArea(const QPoint &position) const;

    /**
     * @brief 获取最大化按钮在标题栏内的位置。
     * @return 最大化按钮的矩形，按钮隐藏时为空。
     */
    QRect maximizeButtonRect() const;

    /**
     * @brief 根据窗口状态刷新标题栏内容。
     */
    void refreshAppearance();

    /**
     * @brief 切换窗口的最大化和还原状态。
     */
    void toggleMaximized();

    /**
     * @brief 更新最大化按钮的原生悬停状态。
     * @param hovered 鼠标是否悬停在最大化按钮上。
     */
    void setMaximizeHovered(bool hovered);


private:
    /**
     * @brief 按窗口标题和图标更新左侧内容。
     */
    void refreshIdentity();

    /**
     * @brief 标题栏所属的窗口。
     */
    QWidget *m_window;

    /**
     * @brief 显示窗口图标。
     */
    QLabel *m_icon;

    /**
     * @brief 显示窗口标题。
     */
    QLabel *m_title;

    /**
     * @brief 最小化按钮。
     */
    AnCaptionButton *m_minimize;

    /**
     * @brief 最大化和还原按钮。
     */
    AnCaptionButton *m_maximize;

    /**
     * @brief 关闭按钮。
     */
    AnCaptionButton *m_close;
};

#endif
