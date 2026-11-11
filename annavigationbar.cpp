#include <QPainter>
#include <QScrollArea>

#include "annavigationbar.h"
#include "annavigationitem.h"
#include "anthemedlabel_p.h"
#include "anthememanager.h"

AnNavigationBar::AnNavigationBar(QWidget *parent)
    : QWidget(parent)
{
    m_layout = new QVBoxLayout(this);
    m_layout->setContentsMargins(8, 8, 8, 8);
    m_layout->setSpacing(4);

    m_items = new QButtonGroup(this);
    m_items->setExclusive(true);

    AnNavigationItem *toggle = new AnNavigationItem(this);
    toggle->setToggleIndicator(true);
    m_toggleButton = toggle;
    m_toggleButton->setToolButtonStyle(Qt::ToolButtonIconOnly);
    m_toggleButton->setFixedWidth(36);
    m_layout->addWidget(m_toggleButton, 0, Qt::AlignLeading);
    QScrollArea *scrollArea = new QScrollArea(this);
    scrollArea->setFrameShape(QFrame::NoFrame);
    scrollArea->setWidgetResizable(true);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollArea->viewport()->setAutoFillBackground(false);
    QWidget *body = new QWidget(scrollArea);
    m_bodyLayout = new QVBoxLayout(body);
    m_bodyLayout->setContentsMargins(0, 0, 0, 0);
    m_bodyLayout->setSpacing(4);
    m_bodyLayout->addStretch();
    scrollArea->setWidget(body);
    body->setAutoFillBackground(false);
    m_layout->addWidget(scrollArea, 1);

    QWidget *footer = new QWidget(this);
    m_footerLayout = new QVBoxLayout(footer);
    m_footerLayout->setContentsMargins(0, 6, 0, 0);
    m_footerLayout->setSpacing(4);
    m_layout->addWidget(footer);
    footer->hide();

    connect(m_toggleButton, &QToolButton::clicked, this, &AnNavigationBar::toggleCollapsed);
    connect(m_items, QOverload<QAbstractButton *>::of(&QButtonGroup::buttonClicked), this, &AnNavigationBar::activateItem);
    connect(&AnThemeManager::instance(), &AnThemeManager::themeChanged, this, QOverload<>::of(&AnNavigationBar::update));
    updatePresentation();
}

int AnNavigationBar::addItem(const QIcon &icon, const QString &text)
{
    return addSelectableItem(m_bodyLayout, icon, text, false);
}

int AnNavigationBar::addGroup(const QString &title)
{
    QWidget *group = new QWidget(m_bodyLayout->parentWidget());
    QVBoxLayout *layout = new QVBoxLayout(group);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(3);
    AnThemedLabel *label = new AnThemedLabel(title, group, AnThemeColor::TextSecondary);
    label->setContentsMargins(10, 5, 4, 0);
    label->setFixedHeight(26);
    QFont font = label->font();
    font.setBold(true);
    label->setFont(font);
    layout->addWidget(label);
    label->setVisible(!m_collapsed);
    m_groupLabels.append(label);
    m_bodyLayout->insertWidget(m_bodyLayout->count() - 1, group);
    m_groups.append(layout);
    return m_groups.size() - 1;
}

int AnNavigationBar::addGroupItem(int groupIndex, const QIcon &icon, const QString &text)
{
    QVBoxLayout *layout = m_groups.value(groupIndex, nullptr);
    return layout ? addSelectableItem(layout, icon, text, false) : -1;
}

int AnNavigationBar::addBranch(int groupIndex, const QIcon &icon, const QString &text, bool expanded)
{
    QVBoxLayout *layout = m_groups.value(groupIndex, nullptr);
    if (!layout)
    {
        return -1;
    }

    QWidget *group = layout->parentWidget();
    AnNavigationItem *button = createItem(group, icon, text);
    button->setBranchIndicator(true);
    button->setCheckable(true);
    button->setChecked(expanded);
    layout->addWidget(button);

    QWidget *children = new QWidget(group);
    QVBoxLayout *childLayout = new QVBoxLayout(children);
    childLayout->setContentsMargins(0, 0, 0, 0);
    childLayout->setSpacing(3);
    layout->addWidget(children);

    m_branches.append(qMakePair(button, children));
    connect(button, &QToolButton::toggled, this, &AnNavigationBar::updateBranches);
    updateBranches();
    return m_branches.size() - 1;
}

int AnNavigationBar::addChildItem(int branchIndex, const QIcon &icon, const QString &text)
{
    if (branchIndex < 0 || branchIndex >= m_branches.size())
    {
        return -1;
    }

    QWidget *children = m_branches.at(branchIndex).second;
    const int index = addSelectableItem(static_cast<QVBoxLayout *>(children->layout()), icon, text, true);
    updateBranches();
    return index;
}

int AnNavigationBar::addFooterItem(const QIcon &icon, const QString &text)
{
    const int index = addSelectableItem(m_footerLayout, icon, text, false);
    m_footerLayout->parentWidget()->show();
    update();
    return index;
}

void AnNavigationBar::setBranchIcon(int branchIndex, const QIcon &icon)
{
    if (branchIndex >= 0 && branchIndex < m_branches.size())
    {
        m_branches.at(branchIndex).first->setIcon(icon);
    }
}

void AnNavigationBar::setToggleIcon(const QIcon &icon)
{
    m_toggleButton->setIcon(icon);
    m_toggleButton->update();
}

AnNavigationItem *AnNavigationBar::createItem(QWidget *parent, const QIcon &icon, const QString &text)
{
    AnNavigationItem *button = new AnNavigationItem(parent);
    button->setText(text);
    button->setAccessibleName(text);
    button->setToolTip(text);
    button->setIcon(icon);
    button->setToolButtonStyle(m_collapsed ? Qt::ToolButtonIconOnly : Qt::ToolButtonTextBesideIcon);
    return button;
}

int AnNavigationBar::addSelectableItem(QVBoxLayout *layout, const QIcon &icon, const QString &text, bool child)
{
    const int index = m_items->buttons().size();
    AnNavigationItem *button = createItem(layout->parentWidget(), icon, text);
    button->setCheckable(true);
    button->setChildItem(child);
    m_items->addButton(button, index);
    if (layout == m_bodyLayout)
    {
        layout->insertWidget(layout->count() - 1, button);
    }
    else
    {
        layout->addWidget(button);
    }
    if (m_currentIndex < 0)
    {
        setCurrentIndex(index);
    }

    return index;
}

void AnNavigationBar::updateBranches()
{
    for (const QPair<AnNavigationItem *, QWidget *> &branch : m_branches)
    {
        branch.second->setVisible(branch.first->isChecked() && branch.second->layout()->count() > 0);
    }
}

void AnNavigationBar::setHeaderWidget(QWidget *widget)
{
    if (m_headerWidget == widget)
    {
        return;
    }

    if (m_headerWidget)
    {
        m_layout->removeWidget(m_headerWidget);
        m_headerWidget->hide();
    }

    m_headerWidget = widget;
    if (m_headerWidget)
    {
        m_layout->insertWidget(1, m_headerWidget);
        m_headerWidget->setVisible(!m_collapsed);
    }
}

void AnNavigationBar::setItemIcon(int index, const QIcon &icon)
{
    QAbstractButton *button = m_items->button(index);
    if (button)
    {
        button->setIcon(icon);
    }
}

bool AnNavigationBar::isCollapsed() const
{
    return m_collapsed;
}

void AnNavigationBar::setCollapsed(bool collapsed)
{
    if (m_collapsed == collapsed)
    {
        return;
    }

    m_collapsed = collapsed;
    updatePresentation();
    emit collapsedChanged(m_collapsed);
}

int AnNavigationBar::currentIndex() const
{
    return m_currentIndex;
}

void AnNavigationBar::setCurrentIndex(int index)
{
    QAbstractButton *button = m_items->button(index);
    if (!button || m_currentIndex == index)
    {
        return;
    }

    for (const QPair<AnNavigationItem *, QWidget *> &branch : m_branches)
    {
        if (button->parentWidget() == branch.second)
        {
            branch.first->setChecked(true);
            break;
        }
    }
    m_currentIndex = index;
    button->setChecked(true);
    emit currentChanged(m_currentIndex);
}

void AnNavigationBar::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(QPen(AnThemeManager::instance().color(AnThemeColor::Divider)));
    painter.setBrush(AnThemeManager::instance().color(AnThemeColor::Surface));
    painter.drawRoundedRect(QRectF(rect()).adjusted(0.5, 0.5, -0.5, -0.5), 8, 8);
    const QWidget *footer = m_footerLayout->parentWidget();
    if (!footer->isHidden())
    {
        const qreal y = footer->y() + 0.5;
        painter.drawLine(QPointF(8, y), QPointF(width() - 8, y));
    }
}

void AnNavigationBar::toggleCollapsed()
{
    setCollapsed(!m_collapsed);
}

void AnNavigationBar::activateItem(QAbstractButton *button)
{
    setCurrentIndex(m_items->id(button));
}

void AnNavigationBar::updatePresentation()
{
    setFixedWidth(m_collapsed ? 52 : 204);
    if (m_headerWidget)
    {
        m_headerWidget->setVisible(!m_collapsed);
    }
    const Qt::ToolButtonStyle itemStyle = m_collapsed ? Qt::ToolButtonIconOnly : Qt::ToolButtonTextBesideIcon;
    for (QWidget *label : m_groupLabels)
    {
        label->setVisible(!m_collapsed);
    }
    for (const QPair<AnNavigationItem *, QWidget *> &branch : m_branches)
    {
        branch.first->setToolButtonStyle(itemStyle);
    }
    const QList<QAbstractButton *> buttons = m_items->buttons();
    for (QAbstractButton *button : buttons)
    {
        QToolButton *item = static_cast<QToolButton *>(button);
        item->setToolButtonStyle(itemStyle);
        item->update();
    }

    const QString action = m_collapsed ? tr("展开导航栏") : tr("收起导航栏");
    m_toggleButton->setAccessibleName(action);
    m_toggleButton->setToolTip(action);
}
