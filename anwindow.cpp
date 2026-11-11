#include <QEvent>
#include <QPainter>
#include <QTimer>
#include <QtMath>

#ifdef Q_OS_WIN
#include <qt_windows.h>
#include <windowsx.h>
#endif

#include "anthememanager.h"
#include "anwindow.h"

AnWindow::AnWindow(QWidget *parent, Qt::WindowFlags flags)
    : QWidget(parent, flags)
{
    m_windowLayout = new QVBoxLayout(this);
    m_windowLayout->setContentsMargins(0, 0, 0, 0);
    m_windowLayout->setSpacing(0);

    m_headerLayout = new QHBoxLayout();
    m_contentLayout = new QVBoxLayout();
    m_statusLayout = new QHBoxLayout();
    m_headerLayout->setContentsMargins(0, 0, 0, 0);
    m_contentLayout->setContentsMargins(0, 0, 0, 0);
    m_statusLayout->setContentsMargins(0, 0, 0, 0);
    m_windowLayout->addLayout(m_headerLayout);
    m_windowLayout->addLayout(m_contentLayout, 1);
    m_windowLayout->addLayout(m_statusLayout);

    connect(&AnThemeManager::instance(), &AnThemeManager::themeChanged, this, QOverload<>::of(&AnWindow::update));
}

QHBoxLayout *AnWindow::headerLayout() const
{
    return m_headerLayout;
}

QVBoxLayout *AnWindow::contentLayout() const
{
    return m_contentLayout;
}

QHBoxLayout *AnWindow::statusLayout() const
{
    return m_statusLayout;
}

void AnWindow::setFramelessEnabled(bool enabled)
{
#ifdef Q_OS_WIN
    if (enabled == m_framelessEnabled || !isWindow())
    {
        return;
    }

    const bool visible = isVisible();
    m_framelessEnabled = enabled;
    if (enabled && !m_titleBar)
    {
        m_titleBar = new AnTitleBar(this);
        m_windowLayout->insertWidget(0, m_titleBar);
    }

    setWindowFlag(Qt::FramelessWindowHint, enabled);
    setAttribute(Qt::WA_TranslucentBackground, enabled);
    m_titleBar->setVisible(enabled);
    m_titleBar->refreshAppearance();
    updateFrameMargins();
    if (visible)
    {
        show();
    }
#else
    Q_UNUSED(enabled);
#endif
}

void AnWindow::setShadowColor(const QColor &color)
{
    if (color.isValid() && color != m_shadowColor)
    {
        m_shadowColor = color;
        updateFrameMargins();
    }
}

void AnWindow::setBorderColor(const QColor &color)
{
    if (color.isValid() && color != m_borderColor)
    {
        m_borderColor = color;
        update();
    }
}

void AnWindow::setShadowRadius(int radius)
{
    radius = qBound(0, radius, 48);
    if (radius != m_shadowRadius)
    {
        m_shadowRadius = radius;
        updateFrameMargins();
    }
}

void AnWindow::setShadowEnabled(bool enabled)
{
    if (enabled != m_shadowEnabled)
    {
        m_shadowEnabled = enabled;
        updateFrameMargins();
    }
}

void AnWindow::showEvent(QShowEvent *event)
{
    QWidget::showEvent(event);
#ifdef Q_OS_WIN
    if (m_framelessEnabled)
    {
        const HWND handle = reinterpret_cast<HWND>(winId());
        const Qt::WindowFlags flags = windowFlags();
        LONG_PTR style = GetWindowLongPtrW(handle, GWL_STYLE) | WS_CAPTION | WS_SYSMENU;
        if (minimumSize() != maximumSize())
        {
            style |= WS_THICKFRAME;
        }
        if (flags.testFlag(Qt::WindowMinimizeButtonHint))
        {
            style |= WS_MINIMIZEBOX;
        }
        if (flags.testFlag(Qt::WindowMaximizeButtonHint))
        {
            style |= WS_MAXIMIZEBOX;
        }
        SetWindowLongPtrW(handle, GWL_STYLE, style);
        SetWindowPos(handle, nullptr, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE | SWP_FRAMECHANGED);
    }
#endif
}

void AnWindow::changeEvent(QEvent *event)
{
    QWidget::changeEvent(event);
    if (event->type() == QEvent::WindowStateChange && m_framelessEnabled)
    {
        m_titleBar->refreshAppearance();
        updateFrameMargins();
#ifdef Q_OS_WIN
        if (isMaximized() && !isFullScreen())
        {
            QTimer::singleShot(0, this, &AnWindow::fitMaximizedWindow);
        }
#endif
    }
}

void AnWindow::fitMaximizedWindow()
{
#ifdef Q_OS_WIN
    if (!m_framelessEnabled || !isMaximized() || isFullScreen())
    {
        return;
    }

    const HWND handle = reinterpret_cast<HWND>(winId());
    MONITORINFO info{};
    info.cbSize = sizeof(info);
    if (GetMonitorInfoW(MonitorFromWindow(handle, MONITOR_DEFAULTTONEAREST), &info))
    {
        const RECT &work = info.rcWork;
        RECT frame{};
        if (GetWindowRect(handle, &frame))
        {
            const qreal scale = devicePixelRatioF();
            const int left = qCeil(qMax(0L, work.left - frame.left) / scale);
            const int top = qCeil(qMax(0L, work.top - frame.top) / scale);
            const int right = qCeil(qMax(0L, frame.right - work.right) / scale);
            const int bottom = qCeil(qMax(0L, frame.bottom - work.bottom) / scale);
            m_windowLayout->setContentsMargins(left, top, right, bottom);
        }
        SetWindowPos(handle, nullptr, work.left, work.top, work.right - work.left, work.bottom - work.top, SWP_NOZORDER | SWP_NOACTIVATE | SWP_FRAMECHANGED);
    }
    m_windowLayout->activate();
    update();
#endif
}

void AnWindow::updateFrameMargins()
{
    const int margin = m_framelessEnabled && m_shadowEnabled && m_shadowColor.alpha() > 0 && !isMaximized() && !isFullScreen() ? m_shadowRadius : 0;
    m_windowLayout->setContentsMargins(margin, margin, margin, margin);
    update();
}

bool AnWindow::nativeEvent(const QByteArray &eventType, void *message, long *result)
{
#ifdef Q_OS_WIN
    if (m_framelessEnabled && eventType == "windows_generic_MSG")
    {
        const MSG *native = static_cast<const MSG *>(message);
        if (native->message == WM_NCCALCSIZE)
        {
            *result = 0;
            return true;
        }
        if (native->message == WM_NCMOUSEMOVE)
        {
            const bool hovered = native->wParam == HTMAXBUTTON;
            m_titleBar->setMaximizeHovered(hovered);
            if (hovered)
            {
                TRACKMOUSEEVENT tracking{};
                tracking.cbSize = sizeof(tracking);
                tracking.dwFlags = TME_NONCLIENT | TME_LEAVE;
                tracking.hwndTrack = native->hwnd;
                TrackMouseEvent(&tracking);
            }
        }
        else if (native->message == WM_NCMOUSELEAVE || native->message == WM_MOUSEMOVE)
        {
            m_titleBar->setMaximizeHovered(false);
        }


        if (native->message == WM_NCLBUTTONDOWN && native->wParam == HTMAXBUTTON)
        {
            m_titleBar->toggleMaximized();
            *result = 0;
            return true;
        }

        if (native->message == WM_NCHITTEST)
        {
            POINT point{GET_X_LPARAM(native->lParam), GET_Y_LPARAM(native->lParam)};
            MapWindowPoints(nullptr, native->hwnd, &point, 1);
            const qreal scale = devicePixelRatioF();
            const QPoint position(qFloor(point.x / scale), qFloor(point.y / scale));
            const QPoint titlePosition = m_titleBar->mapFrom(this, position);
            if (m_titleBar->maximizeButtonRect().contains(titlePosition))
            {
                *result = HTMAXBUTTON;
                return true;
            }
            if (!isMaximized() && !isFullScreen())
            {
                const int edge = qMax(m_windowLayout->contentsMargins().left(), 4);
                const bool horizontal = minimumWidth() < maximumWidth();
                const bool vertical = minimumHeight() < maximumHeight();
                const bool left = horizontal && position.x() < edge;
                const bool right = horizontal && position.x() >= width() - edge;
                const bool top = vertical && position.y() < edge;
                const bool bottom = vertical && position.y() >= height() - edge;
                if (left || right || top || bottom)
                {
                    if (top)
                    {
                        *result = left ? HTTOPLEFT : (right ? HTTOPRIGHT : HTTOP);
                    }
                    else if (bottom)
                    {
                        *result = left ? HTBOTTOMLEFT : (right ? HTBOTTOMRIGHT : HTBOTTOM);
                    }
                    else
                    {
                        *result = left ? HTLEFT : HTRIGHT;
                    }
                    return true;
                }
            }

            if (m_titleBar->isDragArea(titlePosition))
            {
                *result = HTCAPTION;
                return true;
            }
            *result = HTCLIENT;
            return true;
        }
    }
#endif

    return QWidget::nativeEvent(eventType, message, result);
}

int AnWindow::frameShadowMargin() const
{
    return isMaximized() || isFullScreen() ? 0 : m_windowLayout->contentsMargins().left();
}

QPainterPath AnWindow::frameOutline() const
{
    const int margin = frameShadowMargin();
    const QRectF panel = QRectF(rect()).adjusted(margin, margin, -margin, -margin);
    const int radius = margin > 0 ? FrameCornerRadius : 0;
    QPainterPath outline;
    outline.addRoundedRect(panel, radius, radius);
    return outline;
}

void AnWindow::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    const QColor background = AnThemeManager::instance().color(AnThemeColor::Background);
    if (!m_framelessEnabled)
    {
        painter.fillRect(rect(), background);
        return;
    }

    painter.setCompositionMode(QPainter::CompositionMode_Source);
    painter.fillRect(rect(), Qt::transparent);
    painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(Qt::NoPen);

    const int margin = frameShadowMargin();
    const QPainterPath outline = frameOutline();
    const QRectF panel = outline.boundingRect();
    const int cornerRadius = margin > 0 ? FrameCornerRadius : 0;
    QColor shadow = m_shadowColor;
    for (int spread = margin; spread > 0; --spread)
    {
        const qreal distance = qreal(spread) / margin;
        const qreal weight = (1 - distance) * (1 - distance);
        shadow.setAlpha(qRound(m_shadowColor.alpha() * 3 * weight / margin));
        painter.setBrush(shadow);
        painter.drawRoundedRect(panel.adjusted(-spread, -spread, spread, spread), cornerRadius + spread, cornerRadius + spread);
    }

    painter.setBrush(background);
    painter.drawPath(outline);
    if (m_borderColor.alpha() > 0 && !isMaximized() && !isFullScreen())
    {
        const qreal borderRadius = qMax(0.0, cornerRadius - 0.5);
        painter.setBrush(Qt::NoBrush);
        painter.setPen(QPen(m_borderColor, 1));
        painter.drawRoundedRect(panel.adjusted(0.5, 0.5, -0.5, -0.5), borderRadius, borderRadius);
    }
}
