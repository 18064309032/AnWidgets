#include <QFont>
#include <QHBoxLayout>
#include <QPainter>
#include <QPainterPath>
#include <QTextBoundaryFinder>
#include <QVBoxLayout>

#include "anavatarcard.h"
#include "anthemedlabel_p.h"
#include "anthememanager.h"

AnAvatarCard::AnAvatarCard(QWidget *parent)
    : AnCard(parent)
{
    QHBoxLayout *cardLayout = new QHBoxLayout(this);
    cardLayout->setContentsMargins(16, 14, 16, 14);
    cardLayout->setSpacing(12);

    m_avatarSlot = new QWidget(this);
    m_avatarSlot->setFixedSize(48, 48);
    cardLayout->addWidget(m_avatarSlot, 0, Qt::AlignVCenter);

    QVBoxLayout *textLayout = new QVBoxLayout();
    textLayout->setSpacing(3);
    m_nameLabel = new AnThemedLabel(QString(), this);
    QFont nameFont = m_nameLabel->font();
    nameFont.setBold(true);
    m_nameLabel->setFont(nameFont);
    m_nameLabel->setWordWrap(true);
    textLayout->addWidget(m_nameLabel);

    m_descriptionLabel = new AnThemedLabel(QString(), this, AnThemeColor::TextSecondary);
    m_descriptionLabel->setWordWrap(true);
    m_descriptionLabel->hide();
    textLayout->addWidget(m_descriptionLabel);
    cardLayout->addLayout(textLayout, 1);
}

void AnAvatarCard::setAvatar(const QPixmap &avatar)
{
    m_avatar = avatar;
    update();
}

void AnAvatarCard::setName(const QString &name)
{
    m_nameLabel->setText(name);
    update();
}

void AnAvatarCard::setDescription(const QString &description)
{
    m_descriptionLabel->setText(description);
    m_descriptionLabel->setVisible(!description.isEmpty());
}

void AnAvatarCard::paintEvent(QPaintEvent *event)
{
    AnCard::paintEvent(event);

    const QRectF avatarRect(m_avatarSlot->geometry());
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    if (!m_avatar.isNull())
    {
        const qreal side = qMin(m_avatar.width(), m_avatar.height());
        const QRectF source((m_avatar.width() - side) / 2, (m_avatar.height() - side) / 2, side, side);
        QPainterPath circle;
        circle.addEllipse(avatarRect);
        painter.setClipPath(circle, Qt::IntersectClip);
        painter.setRenderHint(QPainter::SmoothPixmapTransform);
        painter.drawPixmap(avatarRect, m_avatar, source);
        return;
    }

    painter.setPen(Qt::NoPen);
    painter.setBrush(AnThemeManager::instance().color(AnThemeColor::Primary));
    painter.drawEllipse(avatarRect);
    const QString name = m_nameLabel->text().trimmed();
    if (!name.isEmpty())
    {
        QTextBoundaryFinder finder(QTextBoundaryFinder::Grapheme, name);
        const QString initial = name.left(qMax(1, finder.toNextBoundary())).toUpper();
        QFont avatarFont = font();
        avatarFont.setPixelSize(20);
        avatarFont.setBold(true);
        painter.setFont(avatarFont);
        painter.setPen(AnThemeManager::instance().color(AnThemeColor::TextOnAccent));
        painter.drawText(avatarRect, Qt::AlignCenter, initial);
    }
}
