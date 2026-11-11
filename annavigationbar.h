#ifndef AN_NAVIGATIONBAR_H
#define AN_NAVIGATIONBAR_H

#include <QButtonGroup>
#include <QIcon>
#include <QList>
#include <QPair>
#include <QPointer>
#include <QToolButton>
#include <QVBoxLayout>
#include <QWidget>

class AnNavigationItem;

/**
 * @brief 提供展开、收起和单项选择的侧边导航控件。
 */
class AnNavigationBar : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(bool collapsed READ isCollapsed WRITE setCollapsed NOTIFY collapsedChanged)
    Q_PROPERTY(int currentIndex READ currentIndex WRITE setCurrentIndex NOTIFY currentChanged)

public:
    /**
     * @brief 构造侧边导航栏。
     * @param parent 管理生命周期的父控件，可为 nullptr。
     */
    explicit AnNavigationBar(QWidget *parent = nullptr);

    /**
     * @brief 添加一个导航项。
     * @param icon 收起时显示的图标。
     * @param text 展开时显示的页面名称。
     * @return 新导航项的索引。
     */
    int addItem(const QIcon &icon, const QString &text);

    /**
     * @brief 在滚动区域中添加一个带标题的导航分组。
     * @param title 分组标题，侧栏收起时隐藏。
     * @return 分组索引，用于添加分组项或二级导航。
     */
    int addGroup(const QString &title);

    /**
     * @brief 添加属于指定分组的导航项。
     * @param groupIndex 分组索引。
     * @param icon 导航图标。
     * @param text 页面名称和悬停提示。
     * @return 导航项索引，分组无效时返回 -1。
     */
    int addGroupItem(int groupIndex, const QIcon &icon, const QString &text);

    /**
     * @brief 在指定分组中添加可展开的二级导航入口。
     * @param groupIndex 分组索引。
     * @param icon 入口图标。
     * @param text 入口名称和悬停提示。
     * @param expanded 是否默认展开。
     * @return 二级导航入口索引，分组无效时返回 -1。
     */
    int addBranch(int groupIndex, const QIcon &icon, const QString &text, bool expanded = false);

    /**
     * @brief 在指定二级导航入口下添加子项。
     * @param branchIndex 二级导航入口索引。
     * @param icon 导航图标。
     * @param text 页面名称和悬停提示。
     * @return 导航项索引，入口无效时返回 -1。
     */
    int addChildItem(int branchIndex, const QIcon &icon, const QString &text);

    /**
     * @brief 在侧栏底部添加固定导航项。
     * @param icon 导航图标。
     * @param text 页面名称和悬停提示。
     * @return 导航项索引。
     */
    int addFooterItem(const QIcon &icon, const QString &text);

    /**
     * @brief 更新二级导航入口的图标。
     * @param branchIndex 二级导航入口索引。
     * @param icon 新图标。
     */
    void setBranchIcon(int branchIndex, const QIcon &icon);

    /**
     * @brief 设置侧栏展开和收起按钮使用的图标。
     * @param icon 自定义图标，空图标恢复默认的三横线图标。
     */
    void setToggleIcon(const QIcon &icon);

    /**
     * @brief 设置展开时显示在导航项上方的控件。
     * @param widget 新的顶部控件，可为 nullptr。被替换的控件仍由侧栏管理。
     */
    void setHeaderWidget(QWidget *widget);

    /**
     * @brief 更新指定导航项的图标。
     * @param index 导航项的索引。
     * @param icon 新图标。
     */
    void setItemIcon(int index, const QIcon &icon);

    /**
     * @brief 获取导航栏是否收起。
     * @return 收起时返回 true。
     */
    bool isCollapsed() const;

    /**
     * @brief 设置导航栏的收起状态。
     * @param collapsed 为 true 时只显示图标。
     */
    void setCollapsed(bool collapsed);

    /**
     * @brief 获取当前选中项。
     * @return 当前项索引，尚无选项时为 -1。
     */
    int currentIndex() const;

    /**
     * @brief 选中指定导航项。
     * @param index 有效导航项的索引。
     */
    void setCurrentIndex(int index);

signals:
    /**
     * @brief 在选中项发生变化时发出通知。
     * @param index 新选中项的索引。
     */
    void currentChanged(int index);

    /**
     * @brief 在展开或收起时发出通知。
     * @param collapsed 更新后的收起状态。
     */
    void collapsedChanged(bool collapsed);

protected:
    /**
     * @brief 绘制侧栏背景和分隔线。
     * @param event 当前绘制事件。
     */
    void paintEvent(QPaintEvent *event) override;

private:
    /**
     * @brief 切换导航栏的收起状态。
     */
    void toggleCollapsed();

    /**
     * @brief 选中用户激活的导航按钮。
     * @param button 当前被激活的按钮。
     */
    void activateItem(QAbstractButton *button);

    /**
     * @brief 更新侧栏宽度和导航项的文字显示方式。
     */
    void updatePresentation();

    /**
     * @brief 在指定布局中创建可选择的导航项。
     * @param layout 容纳导航项的布局。
     * @param icon 导航图标。
     * @param text 页面名称和悬停提示。
     * @param child 是否使用二级导航缩进。
     * @return 新导航项的索引。
     */
    int addSelectableItem(QVBoxLayout *layout, const QIcon &icon, const QString &text, bool child);

    /**
     * @brief 创建导航按钮并设置显示方式和无障碍名称。
     * @param parent 管理按钮生命周期的父控件。
     * @param icon 导航图标。
     * @param text 显示名称和悬停提示。
     * @return 新导航按钮。
     */
    AnNavigationItem *createItem(QWidget *parent, const QIcon &icon, const QString &text);

    /**
     * @brief 根据二级导航入口的展开状态显示或隐藏对应的子项。
     */
    void updateBranches();

    /**
     * @brief 管理导航项与展开按钮的布局。
     */
    QVBoxLayout *m_layout = nullptr;

    /**
     * @brief 管理可滚动的导航项和分组。
     */
    QVBoxLayout *m_bodyLayout = nullptr;

    /**
     * @brief 管理固定在底部的导航项。
     */
    QVBoxLayout *m_footerLayout = nullptr;

    /**
     * @brief 保存各分组的布局，索引与 addGroup 的返回值对应。
     */
    QList<QVBoxLayout *> m_groups;

    /**
     * @brief 保存展开侧栏时可见的分组标题。
     */
    QList<QWidget *> m_groupLabels;

    /**
     * @brief 保存二级导航的入口按钮及其子项容器。
     */
    QList<QPair<AnNavigationItem *, QWidget *>> m_branches;

    /**
     * @brief 管理导航项的互斥选择。
     */
    QButtonGroup *m_items = nullptr;

    /**
     * @brief 展开或收起侧栏的按钮。
     */
    QToolButton *m_toggleButton = nullptr;

    /**
     * @brief 导航栏展开时显示的顶部控件。
     */
    QPointer<QWidget> m_headerWidget;

    /**
     * @brief 当前选中项索引。
     */
    int m_currentIndex = -1;

    /**
     * @brief 导航栏当前是否收起。
     */
    bool m_collapsed = false;
};

#endif
