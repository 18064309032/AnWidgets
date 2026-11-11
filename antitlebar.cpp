#include <QHBoxLayout>

#include "anthemedlabel_p.h"
#include "antitlebar.h"

AnTitleBar::AnTitleBar(QWidget *window)
    : QWidget(window)
    , m_window(window)
    , m_icon(new QLabel(this))
    , m_title(new AnThemedLabel(QString(), this))
    , m_minimize(new AnCaptionButton(AnCaptionButton::Symbol::Minimize, this))
    , m_maximize(new AnCaptionButton(AnCaptionButton::Symbol::Maximize, this))
    , m_close(new AnCaptionButton(AnCaptionButton::Symbol::Close, this))
{
    setFixedHeight(40);
    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setContentsMargins(12, 0, 0, 0);
    layout->setSpacing(0);
    m_icon->setFixedSize(16, 16);
    m_title->setMinimumWidth(0);
    m_title->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Preferred);
    layout->addWidget(m_icon);
    layout->addSpacing(6);
    layout->addWidget(m_title, 1);

    for (AnCaptionButton *button : {m_minimize, m_maximize, m_close})
    {
        layout->addWidget(button);
    }

    m_minimize->setAccessibleName(tr("最小化"));
    m_minimize->setToolTip(m_minimize->accessibleName());
    m_close->setAccessibleName(tr("关闭"));
    m_close->setToolTip(m_close->accessibleName());
    connect(m_minimize, &QToolButton::clicked, m_window, &QWidget::showMinimized);
    connect(m_maximize, &QToolButton::clicked, this, &AnTitleBar::toggleMaximized);
    connect(m_close, &QToolButton::clicked, m_window, &QWidget::close);
    connect(m_window, &QWidget::windowTitleChanged, this, &AnTitleBar::refreshIdentity);
    connect(m_window, &QWidget::windowIconChanged, this, &AnTitleBar::refreshIdentity);
    refreshIdentity();
    refreshAppearance();
}

bool AnTitleBar::isDragArea(const QPoint &position) const
{
    if (!rect().contains(position))
    {
        return false;
    }

    const QWidget *child = childAt(position);
    return child != m_minimize && child != m_maximize && child != m_close;
}

QRect AnTitleBar::maximizeButtonRect() const
{
    return m_maximize->isVisible() ? m_maximize->geometry() : QRect();
}

void AnTitleBar::refreshAppearance()
{
    const bool maximized = m_window->isMaximized();
    const Qt::WindowFlags flags = m_window->windowFlags();
    m_minimize->setVisible(flags.testFlag(Qt::WindowMinimizeButtonHint));
    m_maximize->setVisible(flags.testFlag(Qt::WindowMaximizeButtonHint));
    m_close->setVisible(flags.testFlag(Qt::WindowCloseButtonHint));
    m_maximize->setAccessibleName(maximized ? tr("还原") : tr("最大化"));
    m_maximize->setToolTip(m_maximize->accessibleName());

    m_maximize->setSymbol(maximized ? AnCaptionButton::Symbol::Restore : AnCaptionButton::Symbol::Maximize);
}

void AnTitleBar::setMaximizeHovered(bool hovered)
{
    m_maximize->setNativeHovered(hovered);
}

void AnTitleBar::toggleMaximized()
{
    if (m_window->isMaximized())
    {
        m_window->showNormal();
    }
    else
    {
        m_window->showMaximized();
    }
}

void AnTitleBar::refreshIdentity()
{
    m_title->setText(m_window->windowTitle());
    const QIcon icon = m_window->windowIcon();
    m_icon->setVisible(!icon.isNull());
    m_icon->setPixmap(icon.pixmap(16, 16));
}
