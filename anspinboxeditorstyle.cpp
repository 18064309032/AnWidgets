#include <QWidget>

#include "anspinboxeditorstyle.h"

AnSpinBoxEditorStyle::AnSpinBoxEditorStyle(QWidget *parent)
{
    setParent(parent);
}

QRect AnSpinBoxEditorStyle::subControlRect(ComplexControl control, const QStyleOptionComplex *option, SubControl subControl, const QWidget *widget) const
{
    const QRect bounds = QProxyStyle::subControlRect(control, option, subControl, widget);
    if (control == CC_SpinBox && subControl == SC_SpinBoxEditField && bounds.width() > 8 && bounds.height() > 8)
    {
        return bounds.adjusted(4, 4, -4, -4);
    }

    return bounds;
}
