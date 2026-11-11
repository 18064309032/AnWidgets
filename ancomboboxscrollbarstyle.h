#ifndef AN_COMBOBOXSCROLLBARSTYLE_H
#define AN_COMBOBOXSCROLLBARSTYLE_H

#include <QProxyStyle>

/**
 * @brief 为下拉列表的滚动条提供细滑块和主题配色。
 */
class AnComboBoxScrollBarStyle : public QProxyStyle
{
public:
    /**
     * @brief 构造滚动条样式。
     * @param parent 管理生命周期的滚动条。
     */
    explicit AnComboBoxScrollBarStyle(QWidget *parent);

    /**
     * @brief 返回滚动条宽度及最小滑块长度。
     * @param metric 要查询的像素尺寸。
     * @param option 当前样式参数。
     * @param widget 使用样式的控件。
     * @return 对应的像素尺寸。
     */
    int pixelMetric(PixelMetric metric, const QStyleOption *option, const QWidget *widget) const override;

    /**
     * @brief 自绘滚动条滑块和步进箭头。
     * @param control 当前复杂控件类型。
     * @param option 当前样式参数。
     * @param painter 当前绘制所用的画家。
     * @param widget 使用样式的控件。
     */
    void drawComplexControl(ComplexControl control, const QStyleOptionComplex *option, QPainter *painter, const QWidget *widget) const override;

private:
    /**
     * @brief 绘制一个可点击的滚动步进箭头。
     * @param painter 当前绘制所用的画家。
     * @param option 当前滚动条样式参数。
     * @param control 要绘制的步进子控件。
     * @param widget 使用样式的滚动条。
     */
    void drawArrow(QPainter *painter, const QStyleOptionSlider *option, SubControl control, const QWidget *widget) const;
};

#endif
