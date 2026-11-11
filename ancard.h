#ifndef ANCARD_H
#define ANCARD_H

#include <QFrame>

/**
 * @brief 提供圆角背景和主题边框的内容容器。
 */
class AnCard : public QFrame
{
    Q_OBJECT

    Q_PROPERTY(int cornerRadius READ cornerRadius WRITE setCornerRadius NOTIFY cornerRadiusChanged)

public:
    /**
     * @brief 构造卡片容器。
     * @param parent 管理生命周期的父对象，可为 nullptr。
     */
    explicit AnCard(QWidget *parent = nullptr);

    /**
     * @brief 获取卡片的圆角半径。
     * @return 圆角半径，单位为像素。
     */
    int cornerRadius() const;

    /**
     * @brief 设置卡片的圆角半径。
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
     * @brief 绘制卡片背景和边框。
     * @param event 当前绘制事件。
     */
    void paintEvent(QPaintEvent *event) override;

private:
    /**
     * @brief 卡片的圆角半径，单位为像素。
     */
    int m_cornerRadius = 10;
};

#endif
