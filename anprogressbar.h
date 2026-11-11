#ifndef AN_PROGRESSBAR_H
#define AN_PROGRESSBAR_H

#include <QBasicTimer>
#include <QProgressBar>

/**
 * @brief 提供主题配色、圆角和忙碌动画的进度条。
 */
class AnProgressBar : public QProgressBar
{
    Q_OBJECT
    Q_PROPERTY(int cornerRadius READ cornerRadius WRITE setCornerRadius NOTIFY cornerRadiusChanged)

public:
    /**
     * @brief 构造进度条。
     * @param parent 管理生命周期的父对象，可为 nullptr。
     */
    explicit AnProgressBar(QWidget *parent = nullptr);

    /**
     * @brief 获取进度条的圆角半径。
     * @return 圆角半径，单位为像素。
     */
    int cornerRadius() const;

    /**
     * @brief 设置进度条的圆角半径。
     * @param radius 圆角半径，单位为像素，负数按零处理。
     */
    void setCornerRadius(int radius);

signals:
    /**
     * @brief 在圆角半径改变时发出通知。
     * @param radius 更新后的圆角半径。
     */
    void cornerRadiusChanged(int radius);

protected:
    /**
     * @brief 绘制圆角轨道、进度填充和进度文字。
     * @param event 当前绘制事件。
     */
    void paintEvent(QPaintEvent *event) override;

    /**
     * @brief 推进忙碌动画，并在退出忙碌状态后停止计时。
     * @param event 当前计时器事件。
     */
    void timerEvent(QTimerEvent *event) override;

    /**
     * @brief 在进度条隐藏时停止忙碌动画。
     * @param event 当前隐藏事件。
     */
    void hideEvent(QHideEvent *event) override;

private:
    /**
     * @brief 进度条的圆角半径，单位为像素。
     */
    int m_cornerRadius = 6;

    /**
     * @brief 驱动忙碌动画的计时器。
     */
    QBasicTimer m_busyTimer;

    /**
     * @brief 忙碌动画在当前周期内的帧序号。
     */
    int m_busyFrame = 0;
};

#endif
