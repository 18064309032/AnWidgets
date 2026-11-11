#ifndef ANLISTVIEW_H
#define ANLISTVIEW_H

#include <QListView>

/**
 * @brief 提供主题配色和统一选中样式的列表视图。
 */
class AnListView : public QListView
{
    Q_OBJECT

public:
    /**
     * @brief 构造列表视图。
     * @param parent 管理生命周期的父对象，可为 nullptr。
     */
    explicit AnListView(QWidget *parent = nullptr);

protected:
    /**
     * @brief 绘制列表外框，并刷新焦点状态。
     * @param event 当前控件事件。
     * @return 事件是否已处理。
     */
    bool event(QEvent *event) override;

private:
    /**
     * @brief 将当前主题应用到控件。
     */
    void refreshTheme();
};

#endif
