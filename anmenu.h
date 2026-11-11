#ifndef AN_MENU_H
#define AN_MENU_H

#include <QMenu>

/**
 * @brief 提供圆角面板和主题配色的弹出菜单。
 */
class AnMenu : public QMenu
{
    Q_OBJECT

public:
    /**
     * @brief 构造菜单。
     * @param parent 管理生命周期的父对象，可为 nullptr。
     */
    explicit AnMenu(QWidget *parent = nullptr);

    /**
     * @brief 构造带标题的菜单。
     * @param title 菜单的标题，也可用于显示子菜单入口。
     * @param parent 管理生命周期的父对象，可为 nullptr。
     */
    explicit AnMenu(const QString &title, QWidget *parent = nullptr);
};

#endif
