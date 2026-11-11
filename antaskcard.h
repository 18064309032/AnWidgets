#ifndef AN_TASKCARD_H
#define AN_TASKCARD_H

#include <QLabel>

#include "ancard.h"
#include "anprogressbar.h"
#include "antoolbutton.h"

/**
 * @brief 组合任务标题、状态、进度和操作菜单的卡片。
 */
class AnTaskCard : public AnCard
{
    Q_OBJECT

public:
    /**
     * @brief 构造任务卡片。
     * @param parent 管理生命周期的父对象，可为 nullptr。
     */
    explicit AnTaskCard(QWidget *parent = nullptr);

    /**
     * @brief 设置任务标题。
     * @param title 显示在卡片顶部的任务名称。
     */
    void setTitle(const QString &title);

    /**
     * @brief 设置任务状态文字。
     * @param statusText 显示在进度条上方的状态说明。
     */
    void setStatusText(const QString &statusText);

    /**
     * @brief 设置任务进度范围。
     * @param minimum 最小进度值。
     * @param maximum 最大进度值，和最小值都为零时显示忙碌动画。
     */
    void setProgressRange(int minimum, int maximum);

    /**
     * @brief 更新任务的当前进度。
     * @param value 当前进度值。
     */
    void setProgressValue(int value);

    /**
     * @brief 设置操作按钮使用的菜单。
     * @param menu 由调用方管理生命周期的菜单，可为 nullptr 以隐藏操作按钮。
     */
    void setActionMenu(QMenu *menu);

private:
    /**
     * @brief 任务名称标签。
     */
    QLabel *m_titleLabel = nullptr;

    /**
     * @brief 任务状态标签。
     */
    QLabel *m_statusLabel = nullptr;

    /**
     * @brief 展示任务进度的进度条。
     */
    AnProgressBar *m_progressBar = nullptr;

    /**
     * @brief 展开任务操作菜单的工具按钮。
     */
    AnToolButton *m_actionButton = nullptr;
};

#endif
