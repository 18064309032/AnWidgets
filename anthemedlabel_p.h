#ifndef AN_THEMEDLABEL_P_H
#define AN_THEMEDLABEL_P_H

#include <QLabel>
#include <QPainter>

#include "anthememanager.h"

/**
 * @brief 在 AnWidgets 自绘面板上显示主题配色的纯文本。
 */
class AnThemedLabel : public QLabel
{
public:
    /**
     * @brief 构造主题文本标签。
     * @param text 标签显示的纯文本。
     * @param parent 管理生命周期的父控件。
     * @param role 标签启用时使用的主题文字颜色。
     */
    explicit AnThemedLabel(const QString &text, QWidget *parent, AnThemeColor role = AnThemeColor::TextPrimary)
        : QLabel(text, parent)
        , m_role(role)
    {
        setTextFormat(Qt::PlainText);
        connect(&AnThemeManager::instance(), &AnThemeManager::themeChanged, this, QOverload<>::of(&AnThemedLabel::update));
    }

protected:
    /**
     * @brief 用主题颜色绘制标签文字。
     * @param event 当前绘制事件。
     */
    void paintEvent(QPaintEvent *event) override
    {
        Q_UNUSED(event);

        QPainter painter(this);
        const AnThemeColor textRole = isEnabled() ? m_role : AnThemeColor::TextDisabled;
        painter.setPen(AnThemeManager::instance().color(textRole));
        const int flags = int(alignment()) | (wordWrap() ? Qt::TextWordWrap : 0);
        painter.drawText(contentsRect().adjusted(margin(), margin(), -margin(), -margin()), flags, text());
    }

private:
    /**
     * @brief 标签启用时使用的主题文字颜色。
     */
    AnThemeColor m_role;
};

#endif
