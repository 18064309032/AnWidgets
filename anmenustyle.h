#ifndef ANMENUSTYLE_H
#define ANMENUSTYLE_H

#include <QProxyStyle>

#include "anmenu.h"

/**
 * @brief 绘制 AnMenu 的面板和菜单项外观。
 */
class AnMenuStyle : public QProxyStyle
{
public:
    /**
     * @brief 创建菜单样式。
     * @param parent 管理生命周期的菜单。
     */
    explicit AnMenuStyle(AnMenu *parent);

    /**
     * @brief 提供菜单留白和边框尺寸。
     * @param metric 要查询的像素尺寸。
     * @param option 当前样式参数。
     * @param widget 使用样式的控件。
     * @return 指定的像素尺寸。
     */
    int pixelMetric(PixelMetric metric, const QStyleOption *option, const QWidget *widget) const override;

    /**
     * @brief 增加普通菜单项的高度。
     * @param type 要计算尺寸的内容类型。
     * @param option 当前样式参数。
     * @param size 内容本身的尺寸。
     * @param widget 使用样式的控件。
     * @return 包含菜单留白的建议尺寸。
     */
    QSize sizeFromContents(ContentsType type, const QStyleOption *option, const QSize &size, const QWidget *widget) const override;

    /**
     * @brief 绘制圆角菜单面板和柔和阴影。
     * @param element 要绘制的基本图元。
     * @param option 当前样式参数。
     * @param painter 当前绘制所用的画家。
     * @param widget 使用样式的控件。
     */
    void drawPrimitive(PrimitiveElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget) const override;

    /**
     * @brief 绘制选中背景和分隔线，沿用 Qt 的菜单项内容布局。
     * @param element 要绘制的控件元素。
     * @param option 当前样式参数。
     * @param painter 当前绘制所用的画家。
     * @param widget 使用样式的控件。
     */
    void drawControl(ControlElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget) const override;
};

#endif
