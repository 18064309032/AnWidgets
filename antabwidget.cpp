#include "antabbar.h"
#include "antabwidget.h"

AnTabWidget::AnTabWidget(QWidget *parent)
    : QTabWidget(parent)
{
    setTabBar(new AnTabBar(this));
    setDocumentMode(true);
}
