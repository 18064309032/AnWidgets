#include <QColor>

#include "anthememanager.h"

AnThemeManager::AnThemeManager()
    : QObject(nullptr)
    , m_currentTheme(AnTheme::Light)
{
    initThemeColors();
}

AnThemeManager::~AnThemeManager() = default;

AnThemeManager &AnThemeManager::instance()
{
    static AnThemeManager newInstance;
    return newInstance;
}

AnTheme AnThemeManager::currentTheme() const
{
    return m_currentTheme;
}

void AnThemeManager::applyPreset(AnTheme theme)
{
    if (m_currentTheme == theme) 
	{
		return;
	}
	
    m_currentTheme = theme;
    m_overrides.clear();
    emit themeChanged(m_currentTheme);
}

QColor AnThemeManager::color(AnThemeColor key)const
{
    if (m_overrides.contains(key))
	{
        return m_overrides.value(key);
	}
	
    switch (m_currentTheme)
	{
    case AnTheme::Light:     
		return m_lightColors.value(key, Qt::white);
    case AnTheme::Dark:      
		return m_darkColors.value(key, Qt::white);
    case AnTheme::BlueNight: 
		return m_blueNightColors.value(key, Qt::white);
    }
    return Qt::white;
}

QColor AnThemeManager::lighter(AnThemeColor key, int factor)const
{
    return color(key).lighter(factor);
}

QColor AnThemeManager::darker(AnThemeColor key, int factor)const
{
    return color(key).darker(factor);
}

QColor AnThemeManager::withAlpha(AnThemeColor key, int alpha)const
{
    QColor c = color(key);
    c.setAlpha(alpha);
    return c;
}

void AnThemeManager::setColor(AnThemeColor key, QColor value)
{
    m_overrides[key] = value;
    emit themeChanged(m_currentTheme);
}

void AnThemeManager::resetColor(AnThemeColor key)
{
    m_overrides.remove(key);
    emit themeChanged(m_currentTheme);
}

void AnThemeManager::resetAll()
{
    m_overrides.clear();
    emit themeChanged(m_currentTheme);
}

void AnThemeManager::initThemeColors()
{
    // ════════════════════════════════════════════════
    // Light
    // ════════════════════════════════════════════════
    m_lightColors[AnThemeColor::Background]      = QColor("#FFFFFF");
    m_lightColors[AnThemeColor::Surface]         = QColor("#F5F5F5");
    m_lightColors[AnThemeColor::Input]           = QColor("#FFFFFF");
    m_lightColors[AnThemeColor::PopupBackground] = QColor("#FFFFFF");
    m_lightColors[AnThemeColor::Divider]         = QColor("#E8E8E8");
    m_lightColors[AnThemeColor::Border]          = QColor("#D0D0D0");

    m_lightColors[AnThemeColor::TextPrimary]     = QColor("#1A1A1A");
    m_lightColors[AnThemeColor::TextSecondary]   = QColor("#555555");
    m_lightColors[AnThemeColor::TextMuted]       = QColor("#9E9E9E");
    m_lightColors[AnThemeColor::TextDisabled]    = QColor("#BDBDBD");
    m_lightColors[AnThemeColor::TextOnAccent]    = QColor("#FFFFFF");

    m_lightColors[AnThemeColor::Primary]         = QColor("#1976D2");
    m_lightColors[AnThemeColor::Hover]           = QColor("#E3F2FD");
    m_lightColors[AnThemeColor::Pressed]         = QColor("#1565C0");
    m_lightColors[AnThemeColor::Disabled]        = QColor("#BDBDBD");
    m_lightColors[AnThemeColor::Selection]       = QColor("#BBDEFB");

    m_lightColors[AnThemeColor::Success]         = QColor("#388E3C");
    m_lightColors[AnThemeColor::Info]            = QColor("#0288D1");
    m_lightColors[AnThemeColor::Warning]         = QColor("#F57C00");
    m_lightColors[AnThemeColor::Error]           = QColor("#D32F2F");

    m_lightColors[AnThemeColor::IconPrimary]     = QColor("#1A1A1A");
    m_lightColors[AnThemeColor::IconSecondary]   = QColor("#555555");
    m_lightColors[AnThemeColor::IconOnAccent]    = QColor("#FFFFFF");

    // ════════════════════════════════════════════════
    // Dark
    // ════════════════════════════════════════════════
    m_darkColors[AnThemeColor::Background]      = QColor("#141414");
    m_darkColors[AnThemeColor::Surface]         = QColor("#1E1E1E");
    m_darkColors[AnThemeColor::Input]           = QColor("#2D2D30");
    m_darkColors[AnThemeColor::PopupBackground] = QColor("#252526");
    m_darkColors[AnThemeColor::Divider]         = QColor("#3F3F46");
    m_darkColors[AnThemeColor::Border]          = QColor("#555555");

    m_darkColors[AnThemeColor::TextPrimary]     = QColor("#D4D4D4");
    m_darkColors[AnThemeColor::TextSecondary]   = QColor("#9E9E9E");
    m_darkColors[AnThemeColor::TextMuted]       = QColor("#6E6E6E");
    m_darkColors[AnThemeColor::TextDisabled]    = QColor("#5A5A5A");
    m_darkColors[AnThemeColor::TextOnAccent]    = QColor("#FFFFFF");

    m_darkColors[AnThemeColor::Primary]         = QColor("#007ACC");
    m_darkColors[AnThemeColor::Hover]           = QColor("#2A2D2E");
    m_darkColors[AnThemeColor::Pressed]         = QColor("#0E639C");
    m_darkColors[AnThemeColor::Disabled]        = QColor("#5A5A5A");
    m_darkColors[AnThemeColor::Selection]       = QColor("#264F78");

    m_darkColors[AnThemeColor::Success]         = QColor("#4EC9B0");
    m_darkColors[AnThemeColor::Info]            = QColor("#569CD6");
    m_darkColors[AnThemeColor::Warning]         = QColor("#CCA700");
    m_darkColors[AnThemeColor::Error]           = QColor("#F48771");

    m_darkColors[AnThemeColor::IconPrimary]     = QColor("#D4D4D4");
    m_darkColors[AnThemeColor::IconSecondary]   = QColor("#9E9E9E");
    m_darkColors[AnThemeColor::IconOnAccent]    = QColor("#FFFFFF");

    // ════════════════════════════════════════════════
    // BlueNight
    // ════════════════════════════════════════════════
    m_blueNightColors[AnThemeColor::Background]      = QColor("#0F1419");
    m_blueNightColors[AnThemeColor::Surface]         = QColor("#1A1F2E");
    m_blueNightColors[AnThemeColor::Input]           = QColor("#222A3A");
    m_blueNightColors[AnThemeColor::PopupBackground] = QColor("#222A3A");
    m_blueNightColors[AnThemeColor::Divider]         = QColor("#253045");
    m_blueNightColors[AnThemeColor::Border]          = QColor("#2E3D55");

    m_blueNightColors[AnThemeColor::TextPrimary]     = QColor("#E6F1FF");
    m_blueNightColors[AnThemeColor::TextSecondary]   = QColor("#8AA4C2");
    m_blueNightColors[AnThemeColor::TextMuted]       = QColor("#4A5F7A");
    m_blueNightColors[AnThemeColor::TextDisabled]    = QColor("#3E4F66");
    m_blueNightColors[AnThemeColor::TextOnAccent]    = QColor("#FFFFFF");

    m_blueNightColors[AnThemeColor::Primary]         = QColor("#4FC3F7");
    m_blueNightColors[AnThemeColor::Hover]           = QColor("#1E2A3A");
    m_blueNightColors[AnThemeColor::Pressed]         = QColor("#0288D1");
    m_blueNightColors[AnThemeColor::Disabled]        = QColor("#3E4F66");
    m_blueNightColors[AnThemeColor::Selection]       = QColor("#1565C0");

    m_blueNightColors[AnThemeColor::Success]         = QColor("#66BB6A");
    m_blueNightColors[AnThemeColor::Info]            = QColor("#29B6F6");
    m_blueNightColors[AnThemeColor::Warning]         = QColor("#FFB74D");
    m_blueNightColors[AnThemeColor::Error]           = QColor("#EF5350");

    m_blueNightColors[AnThemeColor::IconPrimary]     = QColor("#E6F1FF");
    m_blueNightColors[AnThemeColor::IconSecondary]   = QColor("#8AA4C2");
    m_blueNightColors[AnThemeColor::IconOnAccent]    = QColor("#FFFFFF");
}