#ifndef AN_SLIDER_H
#define AN_SLIDER_H

#include <QSlider>

/**
 * @brief 提供主题配色和自绘轨道的滑块控件。
 */
class AnSlider : public QSlider
{
    Q_OBJECT

public:
    /**
     * @brief 构造滑块。
     * @param parent 管理生命周期的父对象，可为 nullptr。
     */
    explicit AnSlider(QWidget *parent = nullptr);

    /**
     * @brief 构造指定方向的滑块。
     * @param orientation 滑块的方向。
     * @param parent 管理生命周期的父对象，可为 nullptr。
     */
    explicit AnSlider(Qt::Orientation orientation, QWidget *parent = nullptr);

protected:
    /**
     * @brief 绘制滑块的轨道、填充、刻度和滑柄。
     * @param event 当前绘制事件。
     */
    void paintEvent(QPaintEvent *event) override;

private:
    /**
     * @brief 绘制滑块两侧的刻度。
     * @param painter 当前绘制所用的画家。
     * @param handleRect Qt 样式提供的滑柄区域。
     * @param start 最小值对应的轨道端点。
     * @param end 最大值对应的轨道端点。
     */
    void drawTicks(QPainter &painter, const QRect &handleRect, const QPointF &start, const QPointF &end) const;
};

#endif
