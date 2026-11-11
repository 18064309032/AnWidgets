#ifndef AN_PUSHBUTTON_H
#define AN_PUSHBUTTON_H

#include <QPushButton>
#include <QPainter>
#include <QPaintEvent>

/**
 * @brief 提供主题配色和自定义外观的按钮。
 */
class AnPushButton : public QPushButton
{
    Q_OBJECT
public:
    /**
     * @brief 定义按钮的外观样式。
     */
    enum class Appearance
    {
        /**
         * @brief 主按钮：Primary 色背景
         */
        Primary,
        /**
         * @brief 次按钮：透明背景 + 边框
         */
        Secondary,
        /**
         * @brief 幽灵按钮：无背景无边框，只文字变色
         */
        Ghost
    };

    /**
     * @brief 构造 AnPushButton 对象。
     * @param parent 管理生命周期的父对象，可为 nullptr。
     */
    explicit AnPushButton(QWidget *parent = nullptr);
    /**
     * @brief 构造带文字的 AnPushButton 对象。
     * @param text 按钮显示的文字。
     * @param parent 管理生命周期的父对象，可为 nullptr。
     */
    explicit AnPushButton(const QString &text, QWidget *parent = nullptr);

    /**
     * @brief 设置按钮的外观样式。
     * @param appearance 要应用的外观样式。
     */
    void setAppearance(Appearance appearance);
    /**
     * @brief 获取按钮当前的外观样式。
     * @return 当前外观样式。
     */
    Appearance appearance() const;

    /**
     * @brief 设置按钮的圆角半径。
     * @param radius 圆角半径，单位为像素。
     */
    void setRadius(int radius);
    /**
     * @brief 获取按钮的圆角半径。
     * @return 圆角半径，单位为像素。
     */
    int radius() const;

    /**
     * @brief 设置按钮文字四周的内边距。
     * @param vertical 上下内边距，单位为像素。
     * @param horizontal 左右内边距，单位为像素。
     */
    void setPadding(int vertical, int horizontal);

    /**
     * @brief 根据文字和内边距计算建议尺寸。
     * @return 按钮的建议尺寸。
     */
    QSize sizeHint() const override;
    /**
     * @brief 获取按钮的最小建议尺寸。
     * @return 按钮的最小建议尺寸。
     */
    QSize minimumSizeHint() const override;

protected:
    /**
     * @brief 绘制按钮背景和文字。
     * @param event 当前绘制事件。
     */
    void paintEvent(QPaintEvent *event) override;

private slots:
    /**
     * @brief 在主题变化后请求重绘按钮。
     */
    void onThemeChanged();

private:
    /**
     * @brief 根据按钮外观和交互状态绘制背景与边框。
     * @param p 用于绘制按钮的画家。
     */
    void drawBackground(QPainter &p);
    /**
     * @brief 根据按钮外观和交互状态绘制居中的文字。
     * @param p 用于绘制按钮的画家。
     */
    void drawText(QPainter &p);

    /**
     * @brief 当前的按钮外观样式。
     */
    Appearance m_appearance   = Appearance::Primary;
    /**
     * @brief 圆角半径，单位为像素。
     */
    int          m_radius = 6;
    /**
     * @brief 文字上下两侧的内边距，单位为像素。
     */
    int          m_paddingV = 10;
    /**
     * @brief 文字左右两侧的内边距，单位为像素。
     */
    int          m_paddingH = 18;
};

#endif
