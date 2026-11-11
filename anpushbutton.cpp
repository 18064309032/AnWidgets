#include "anpushbutton.h"
#include "anthememanager.h"

AnPushButton::AnPushButton(QWidget *parent)
    : QPushButton(parent)
{
    connect(&AnThemeManager::instance(), &AnThemeManager::themeChanged,
            this, &AnPushButton::onThemeChanged);
}

AnPushButton::AnPushButton(const QString &text, QWidget *parent)
    : QPushButton(text, parent)
{
    connect(&AnThemeManager::instance(), &AnThemeManager::themeChanged,
            this, &AnPushButton::onThemeChanged);
}

void AnPushButton::setButtonType(AnButtonType type)
{
    if (m_type != type) {
        m_type = type;
        update();
    }
}

AnButtonType AnPushButton::buttonType() const
{
    return m_type;
}

void AnPushButton::setRadius(int radius)
{
    if (m_radius != radius) {
        m_radius = radius;
        update();
    }
}

int AnPushButton::radius() const
{
    return m_radius;
}

void AnPushButton::setPadding(int vertical, int horizontal)
{
    m_paddingV = vertical;
    m_paddingH = horizontal;
    updateGeometry();
    update();
}

QSize AnPushButton::sizeHint() const
{
    QFontMetrics fm(font());
    int textW = fm.horizontalAdvance(text());
    int textH = fm.height();
    return QSize(textW + m_paddingH * 2, textH + m_paddingV * 2);
}

QSize AnPushButton::minimumSizeHint() const
{
    return sizeHint();
}

void AnPushButton::onThemeChanged()
{
    update();
}

void AnPushButton::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);
    p.setRenderHint(QPainter::TextAntialiasing);

    drawBackground(p);
    drawText(p);
}

void AnPushButton::drawBackground(QPainter &p)
{
    auto &tm = AnThemeManager::instance();

    bool enabled = isEnabled();
    bool pressed = isDown();
    bool hovered = underMouse();

    QColor bg;
    QColor border;
    bool   drawBorder = false;

    // ── Primary ──
    if (m_type == AnButtonType::Primary) {
        if (!enabled) {
            bg = tm.color(AnThemeColor::Disabled);
        } else if (pressed) {
            bg = tm.color(AnThemeColor::Pressed);
        } else if (hovered) {
            bg = tm.withAlpha(AnThemeColor::Primary, 220);
        } else {
            bg = tm.color(AnThemeColor::Primary);
        }
    }

    // ── Secondary ──
    else if (m_type == AnButtonType::Secondary) {
        drawBorder = true;
        if (!enabled) {
            bg     = tm.color(AnThemeColor::Disabled);
            border = tm.color(AnThemeColor::Border);
        } else if (pressed) {
            bg     = tm.withAlpha(AnThemeColor::Pressed, 30);
            border = tm.color(AnThemeColor::Primary);
        } else if (hovered) {
            bg     = tm.color(AnThemeColor::Hover);
            border = tm.color(AnThemeColor::Primary);
        } else {
            bg     = Qt::transparent;
            border = tm.color(AnThemeColor::Border);
        }
    }

    // ── Ghost ──
    else if (m_type == AnButtonType::Ghost) {
        if (!enabled) {
            bg = Qt::transparent;
        } else if (pressed) {
            bg = tm.withAlpha(AnThemeColor::Pressed, 20);
        } else if (hovered) {
            bg = tm.color(AnThemeColor::Hover);
        } else {
            bg = Qt::transparent;
        }
    }

    // 一次画完背景 + 边框（避免两次绘制错位）
    // 边框 pen 居中绘制，所以矩形要内缩 penW/2 让 pen 刚好落在按钮边缘
    int penW = drawBorder ? 1 : 0;
    QRectF r = QRectF(rect()).adjusted(penW * 0.5, penW * 0.5, -penW * 0.5, -penW * 0.5);

    p.setBrush(bg);
    if (drawBorder) {
        p.setPen(QPen(border, penW));
    } else {
        p.setPen(Qt::NoPen);
    }
    p.drawRoundedRect(r, m_radius, m_radius);
}

void AnPushButton::drawText(QPainter &p)
{
    auto &tm = AnThemeManager::instance();

    bool enabled = isEnabled();
    bool hovered = underMouse();

    QColor textColor;

    // ── Primary ──
    if (m_type == AnButtonType::Primary) {
        textColor = enabled ? tm.color(AnThemeColor::TextOnAccent)
                            : tm.color(AnThemeColor::TextDisabled);
    }

    // ── Secondary ──
    else if (m_type == AnButtonType::Secondary) {
        if (!enabled)        textColor = tm.color(AnThemeColor::TextDisabled);
        else                 textColor = tm.color(AnThemeColor::TextPrimary);
    }

    // ── Ghost ──
    else if (m_type == AnButtonType::Ghost) {
        if (!enabled)                    textColor = tm.color(AnThemeColor::TextDisabled);
        else if (hovered)                textColor = tm.color(AnThemeColor::Primary);
        else                             textColor = tm.color(AnThemeColor::TextPrimary);
    }

    p.setPen(textColor);
    p.setFont(font());

    QRect textRect = rect().adjusted(m_paddingH, m_paddingV, -m_paddingH, -m_paddingV);
    p.drawText(textRect, Qt::AlignCenter, text());
}
