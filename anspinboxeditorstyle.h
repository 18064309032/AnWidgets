#ifndef AN_SPINBOXEDITORSTYLE_H
#define AN_SPINBOXEDITORSTYLE_H

#include <QProxyStyle>

/**
 * @brief 为数值输入框的原生编辑区保留外框留白。
 */
class AnSpinBoxEditorStyle : public QProxyStyle
{
public:
    /**
     * @brief 构造数值输入框样式。
     * @param parent 管理生命周期的数值输入框。
     */
    explicit AnSpinBoxEditorStyle(QWidget *parent);

    /**
     * @brief 获取子控件的绘制区域，并将文本编辑区向内收缩。
     * @param control 当前复杂控件。
     * @param option 当前样式参数。
     * @param subControl 要查询的子控件。
     * @param widget 使用样式的控件。
     * @return 子控件所在的矩形。
     */
    QRect subControlRect(ComplexControl control, const QStyleOptionComplex *option, SubControl subControl, const QWidget *widget) const override;
};

#endif
