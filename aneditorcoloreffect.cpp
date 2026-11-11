#include <QImage>
#include <QPainter>
#include <QPainterPath>
#include <QPixmap>

#include "aneditorcoloreffect_p.h"
#include "anlineedit.h"
#include "anthememanager.h"

AnEditorColorEffect::AnEditorColorEffect(QWidget *editor)
    : QGraphicsEffect(editor)
    , m_editor(editor)
{
    connect(&AnThemeManager::instance(), &AnThemeManager::themeChanged, this, &AnEditorColorEffect::refreshTheme);
    refreshTheme();
}

void AnEditorColorEffect::refreshTheme()
{
    setEnabled(AnThemeManager::instance().color(AnThemeColor::Input).lightness() < 128);
    update();
}

void AnEditorColorEffect::draw(QPainter *painter)
{
    QPoint offset;
    const QPixmap source = sourcePixmap(Qt::LogicalCoordinates, &offset, NoPad);
    if (source.isNull())
    {
        return;
    }

    QImage image = source.toImage().convertToFormat(QImage::Format_ARGB32);
    const int sampleY = qMin(image.height() - 1, qMax(1, qRound(3 * source.devicePixelRatio())));
    const QRgb sample = image.pixel(image.width() / 2, sampleY);
    if (qAlpha(sample) < 128 || qGray(sample) < 128)
    {
        painter->drawPixmap(offset, source);
        return;
    }

    const AnThemeColor backgroundRole = m_editor->isEnabled() ? AnThemeColor::Input : AnThemeColor::Surface;
    const AnThemeColor textRole = m_editor->isEnabled() ? AnThemeColor::TextPrimary : AnThemeColor::TextDisabled;
    const QColor background = AnThemeManager::instance().color(backgroundRole);
    const QColor text = AnThemeManager::instance().color(textRole);
    QRgb tones[256];
    for (int level = 0; level < 256; ++level)
    {
        const int red = (text.red() * (255 - level) + background.red() * level + 127) / 255;
        const int green = (text.green() * (255 - level) + background.green() * level + 127) / 255;
        const int blue = (text.blue() * (255 - level) + background.blue() * level + 127) / 255;
        tones[level] = qRgb(red, green, blue);
    }

    for (int y = 0; y < image.height(); ++y)
    {
        QRgb *pixels = reinterpret_cast<QRgb *>(image.scanLine(y));
        for (int x = 0; x < image.width(); ++x)
        {
            const QRgb pixel = pixels[x];
            const int minimum = qMin(qRed(pixel), qMin(qGreen(pixel), qBlue(pixel)));
            const int maximum = qMax(qRed(pixel), qMax(qGreen(pixel), qBlue(pixel)));
            if (qAlpha(pixel) != 0 && maximum - minimum < 48)
            {
                const QRgb tone = tones[qGray(pixel)];
                pixels[x] = qRgba(qRed(tone), qGreen(tone), qBlue(tone), qAlpha(pixel));
            }
        }
    }

    const AnLineEdit *lineEdit = qobject_cast<const AnLineEdit *>(m_editor);
    if (lineEdit)
    {
        const QRectF frameBounds = QRectF(lineEdit->rect()).adjusted(0.5, 0.5, -0.5, -0.5);
        const qreal radius = qMin(qreal(lineEdit->cornerRadius()), qMin(frameBounds.width(), frameBounds.height()) / 2);
        const qreal innerRadius = qMax(qreal(0), radius - 1);
        QPainterPath interior;
        interior.addRoundedRect(frameBounds.adjusted(1, 1, -1, -1), innerRadius, innerRadius);

        painter->save();
        painter->setRenderHint(QPainter::Antialiasing);
        painter->setPen(QPen(lineEdit->borderColor()));
        painter->setBrush(background);
        painter->drawRoundedRect(frameBounds, radius, radius);
        painter->setClipPath(interior, Qt::IntersectClip);
        painter->drawImage(offset, image);
        painter->restore();
        return;
    }

    painter->drawImage(offset, image);
}
