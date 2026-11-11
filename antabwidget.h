#ifndef ANTABWIDGET_H
#define ANTABWIDGET_H

#include <QTabWidget>

/**
 * @brief 提供主题配色和自绘标签的分页容器。
 */
class AnTabWidget : public QTabWidget
{
    Q_OBJECT

public:
    /**
     * @brief 构造分页容器。
     * @param parent 管理生命周期的父对象，可为 nullptr。
     */
    explicit AnTabWidget(QWidget *parent = nullptr);

};

#endif
