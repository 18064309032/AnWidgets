#ifndef AN_PUSHBUTTON_H
#define AN_PUSHBUTTON_H

#include <QPushButton>
#include <QPainter>
#include <QPaintEvent>

enum class AnButtonType
{
    Primary,      // 主按钮：Primary 色背景
    Secondary,    // 次按钮：透明背景 + 边框
    Ghost         // 幽灵按钮：无背景无边框，只文字变色
};

class AnPushButton : public QPushButton
{
    Q_OBJECT
public:
    explicit AnPushButton(QWidget *parent = nullptr);
    explicit AnPushButton(const QString &text, QWidget *parent = nullptr);

    // ════════ 配置接口 ════════
    void setButtonType(AnButtonType type);
    AnButtonType buttonType() const;

    void setRadius(int radius);
    int radius() const;

    void setPadding(int vertical, int horizontal);

    // ════════ size 提示 ════════
    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;

protected:
    void paintEvent(QPaintEvent *event) override;

private slots:
    void onThemeChanged();

private:
    void drawBackground(QPainter &p);
    void drawText(QPainter &p);

    AnButtonType m_type   = AnButtonType::Primary;
    int          m_radius = 6;
    int          m_paddingV = 10;
    int          m_paddingH = 18;
};

#endif // AN_PUSHBUTTON_H
