#ifndef AN_NAVIGATIONITEM_H
#define AN_NAVIGATIONITEM_H

#include <QToolButton>

/**
 * @brief 用主题颜色绘制侧栏内的导航按钮。
 */
class AnNavigationItem : public QToolButton
{
public:
    /**
     * @brief 构造导航按钮。
     * @param parent 管理生命周期的父控件。
     */
    explicit AnNavigationItem(QWidget *parent);

    /**
     * @brief 设置按钮是否以二级导航的缩进绘制。
     * @param child 是否为二级导航子项。
     */
    void setChildItem(bool child);

    /**
     * @brief 设置是否绘制二级导航展开指示符。
     * @param enabled 是否绘制展开指示符。
     */
    void setBranchIndicator(bool enabled);

    /**
     * @brief 设置空图标时是否绘制侧栏切换符号。
     * @param enabled 是否启用默认的三横线图标。
     */
    void setToggleIndicator(bool enabled);

protected:
    /**
     * @brief 绘制图标、文字和交互高亮。
     * @param event 当前绘制事件。
     */
    void paintEvent(QPaintEvent *event) override;

private:
    /**
     * @brief 是否在展开模式中缩进图标和标签。
     */
    bool m_childItem = false;

    /**
     * @brief 是否把选中状态绘制为二级导航展开指示。
     */
    bool m_branchIndicator = false;

    /**
     * @brief 空图标时是否绘制侧栏切换符号。
     */
    bool m_toggleIndicator = false;
};

#endif
