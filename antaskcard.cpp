#include <QFont>
#include <QHBoxLayout>
#include <QVBoxLayout>

#include "antaskcard.h"
#include "anthemedlabel_p.h"

AnTaskCard::AnTaskCard(QWidget *parent)
    : AnCard(parent)
{
    QVBoxLayout *cardLayout = new QVBoxLayout(this);
    cardLayout->setContentsMargins(16, 12, 16, 16);
    cardLayout->setSpacing(8);

    QHBoxLayout *headerLayout = new QHBoxLayout();
    headerLayout->setSpacing(8);
    m_titleLabel = new AnThemedLabel(QString(), this);
    m_titleLabel->setWordWrap(true);
    QFont titleFont = m_titleLabel->font();
    titleFont.setBold(true);
    m_titleLabel->setFont(titleFont);
    headerLayout->addWidget(m_titleLabel, 1);

    m_actionButton = new AnToolButton(this);
    m_actionButton->setText(tr("操作"));
    m_actionButton->setToolButtonStyle(Qt::ToolButtonTextOnly);
    m_actionButton->setPopupMode(QToolButton::InstantPopup);
    m_actionButton->hide();
    headerLayout->addWidget(m_actionButton, 0, Qt::AlignVCenter);
    cardLayout->addLayout(headerLayout);

    m_statusLabel = new AnThemedLabel(QString(), this, AnThemeColor::TextSecondary);
    m_statusLabel->setWordWrap(true);
    m_statusLabel->hide();
    cardLayout->addWidget(m_statusLabel);

    m_progressBar = new AnProgressBar(this);
    m_progressBar->setRange(0, 100);
    m_progressBar->setValue(0);
    cardLayout->addWidget(m_progressBar);

}

void AnTaskCard::setTitle(const QString &title)
{
    m_titleLabel->setText(title);
}

void AnTaskCard::setStatusText(const QString &statusText)
{
    m_statusLabel->setText(statusText);
    m_statusLabel->setVisible(!statusText.isEmpty());
}

void AnTaskCard::setProgressRange(int minimum, int maximum)
{
    m_progressBar->setRange(minimum, maximum);
}

void AnTaskCard::setProgressValue(int value)
{
    m_progressBar->setValue(value);
}

void AnTaskCard::setActionMenu(QMenu *menu)
{
    m_actionButton->setMenu(menu);
    m_actionButton->setVisible(menu != nullptr);
}
