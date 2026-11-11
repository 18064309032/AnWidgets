#ifndef AN_TABBAR_H
#define AN_TABBAR_H

#include <QTabBar>

/**
 * @brief 绘制 AnTabWidget 使用的标签栏。
 */
class AnTabBar : public QTabBar
{
public:
    /**
     * @brief 构造自绘标签栏。
     * @param parent 管理生命周期的父对象，可为 nullptr。
     */
    explicit AnTabBar(QWidget *parent = nullptr);
protected:
    /**
     * @brief 在原生标签尺寸上增加留白。
     * @param index 标签索引。
     * @return 标签的建议尺寸。
     */
    QSize tabSizeHint(int index) const override;

    /**
     * @brief 绘制标签背景、选中状态和文字。
     * @param event 当前绘制事件。
     */
    void paintEvent(QPaintEvent *event) override;
};

#endif
