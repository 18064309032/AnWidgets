#include "anpushbutton.h"
#include "anthememanager.h"

AnPushButton::AnPushButton(QWidget *parent)
    : QPushButton(parent)
{
    connect(&AnThemeManager::instance(), &AnThemeManager::themeChanged, this, &AnPushButton::onThemeChanged);
}

AnPushButton::AnPushButton(const QString &text, QWidget *parent)
    : QPushButton(text, parent)
{
    connect(&AnThemeManager::instance(), &AnThemeManager::themeChanged, this, &AnPushButton::onThemeChanged);
}

void AnPushButton::setAppearance(Appearance appearance)
{
    if (m_appearance != appearance)
    {
        m_appearance = appearance;
        update();
    }
}

AnPushButton::Appearance AnPushButton::appearance() const
{
    return m_appearance;
}

void AnPushButton::setRadius(int radius)
{
    if (m_radius != radius)
    {
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
    bool enabled = isEnabled();
    bool pressed = isDown();
    bool hovered = underMouse();

    QColor backgroundColor;
    QColor borderColor;
    bool   drawBorder = false;

    /**
     * ── Primary ──
     */
    if (m_appearance == Appearance::Primary)
    {
        if (!enabled)
        {
            backgroundColor = AnThemeManager::instance().color(AnThemeColor::Disabled);
        }
        else if (pressed)
        {
            backgroundColor = AnThemeManager::instance().color(AnThemeColor::Pressed);
        }
        else if (hovered)
        {
            backgroundColor = AnThemeManager::instance().withAlpha(AnThemeColor::Primary, 220);
        }
        else
        {
            backgroundColor = AnThemeManager::instance().color(AnThemeColor::Primary);
        }
    }

    /**
     * ── Secondary ──
     */
    else if (m_appearance == Appearance::Secondary)
    {
        drawBorder = true;
        if (!enabled)
        {
            backgroundColor = AnThemeManager::instance().color(AnThemeColor::Disabled);
            borderColor = AnThemeManager::instance().color(AnThemeColor::Border);
        }
        else if (pressed)
        {
            backgroundColor = AnThemeManager::instance().withAlpha(AnThemeColor::Pressed, 30);
            borderColor = AnThemeManager::instance().color(AnThemeColor::Primary);
        }
        else if (hovered)
        {
            backgroundColor = AnThemeManager::instance().color(AnThemeColor::Hover);
            borderColor = AnThemeManager::instance().color(AnThemeColor::Primary);
        }
        else
        {
            backgroundColor = Qt::transparent;
            borderColor = AnThemeManager::instance().color(AnThemeColor::Border);
        }
    }

    /**
     * ── Ghost ──
     */
    else if (m_appearance == Appearance::Ghost)
    {
        if (!enabled)
        {
            backgroundColor = Qt::transparent;
        }
        else if (pressed)
        {
            backgroundColor = AnThemeManager::instance().withAlpha(AnThemeColor::Pressed, 20);
        }
        else if (hovered)
        {
            backgroundColor = AnThemeManager::instance().color(AnThemeColor::Hover);
        }
        else
        {
            backgroundColor = Qt::transparent;
        }
    }

    /**
     * 一次画完背景 + 边框（避免两次绘制错位）
     * 边框 pen 居中绘制，所以矩形要内缩 penW/2 让 pen 刚好落在按钮边缘
     */
    int penW = drawBorder ? 1 : 0;
    QRectF r = QRectF(rect()).adjusted(penW * 0.5, penW * 0.5, -penW * 0.5, -penW * 0.5);

    p.setBrush(backgroundColor);
    if (drawBorder)
    {
        p.setPen(QPen(borderColor, penW));
    }
    else
    {
        p.setPen(Qt::NoPen);
    }
    p.drawRoundedRect(r, m_radius, m_radius);
}

void AnPushButton::drawText(QPainter &p)
{
    bool enabled = isEnabled();
    bool hovered = underMouse();

    QColor textColor;

    /**
     * ── Primary ──
     */
    if (m_appearance == Appearance::Primary)
    {
        textColor = enabled ? AnThemeManager::instance().color(AnThemeColor::TextOnAccent) : AnThemeManager::instance().color(AnThemeColor::TextDisabled);
    }

    /**
     * ── Secondary ──
     */
    else if (m_appearance == Appearance::Secondary)
    {
        if (!enabled)
        {
            textColor = AnThemeManager::instance().color(AnThemeColor::TextDisabled);
        }
        else
        {
            textColor = AnThemeManager::instance().color(AnThemeColor::TextPrimary);
        }
    }

    /**
     * ── Ghost ──
     */
    else if (m_appearance == Appearance::Ghost)
    {
        if (!enabled)
        {
            textColor = AnThemeManager::instance().color(AnThemeColor::TextDisabled);
        }
        else if (hovered)
        {
            textColor = AnThemeManager::instance().color(AnThemeColor::Primary);
        }
        else
        {
            textColor = AnThemeManager::instance().color(AnThemeColor::TextPrimary);
        }
    }

    p.setPen(textColor);
    p.setFont(font());

    QRect textRect = rect().adjusted(m_paddingH, m_paddingV, -m_paddingH, -m_paddingV);
    p.drawText(textRect, Qt::AlignCenter, text());
}
