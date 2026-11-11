#ifndef AN_THEMEMANAGER_H
#define AN_THEMEMANAGER_H

#include <QObject>
#include <QHash>
#include "antypedef.h"

#ifndef AN_THEME_COLOR
#define AN_THEME_COLOR(key) AnThemeManager::instance().color(key)
#endif 

class AnThemeManager : public QObject
{
	Q_OBJECT
public:
	virtual ~AnThemeManager()override;
	
signals:
    void themeChanged(AnTheme theme);
	
public:
	QColor color(AnThemeColor key)const;
	
	void applyPreset(AnTheme theme);
	AnTheme currentTheme()const;
	
	QColor lighter(AnThemeColor key, int factor = 120)const;
    QColor darker(AnThemeColor key, int factor = 80)const;
    QColor withAlpha(AnThemeColor key, int alpha)const;
	
	void setColor(AnThemeColor key, QColor value);
    void resetColor(AnThemeColor key);
    void resetAll();
public:
	static AnThemeManager& instance();
	
private:
	AnThemeManager();
	
private:
	void initThemeColors();
	
private:
	AnTheme m_currentTheme;
	QHash<AnThemeColor, QColor> m_overrides;
	QHash<AnThemeColor, QColor> m_lightColors;
	QHash<AnThemeColor, QColor> m_darkColors;
	QHash<AnThemeColor, QColor> m_blueNightColors;
};

#endif // AN_THEMEMANAGER_H