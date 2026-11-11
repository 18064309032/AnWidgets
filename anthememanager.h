#ifndef AN_THEMEMANAGER_H
#define AN_THEMEMANAGER_H

#include <QObject>
#include <QHash>

#include "antypedef.h"

#ifndef AN_THEME_COLOR
#define AN_THEME_COLOR(key) AnThemeManager::instance().color(key)
#endif 

/**
 * @brief 管理预设主题、颜色覆盖和主题变更通知。
 */
class AnThemeManager : public QObject
{
	Q_OBJECT
public:
	/**
	 * @brief 析构主题管理器。
	 */
	virtual ~AnThemeManager()override;
	
signals:
    /**
     * @brief 在主题或颜色覆盖变更时发出通知。
     * @param theme 当前主题。
     */
    void themeChanged(AnTheme theme);
	
public:
	/**
	 * @brief 查询颜色角色的实际颜色，优先使用自定义覆盖。
	 * @param key 要查询的颜色角色。
	 * @return 当前生效的颜色。
	 */
	QColor color(AnThemeColor key)const;
	
	/**
	 * @brief 切换预设主题，切换时清除颜色覆盖。
	 * @param theme 要应用的预设主题。
	 */
	void applyPreset(AnTheme theme);
	/**
	 * @brief 获取当前使用的主题。
	 * @return 当前预设主题。
	 */
	AnTheme currentTheme()const;
	
	/**
	 * @brief 按指定因子调整颜色亮度，默认使颜色变亮。
	 * @param key 要调整的颜色角色。
	 * @param factor 亮度调整因子，100 表示原亮度。
	 * @return 调整后的颜色。
	 */
	QColor lighter(AnThemeColor key, int factor = 120)const;
    /**
     * @brief 使用 QColor::darker 调整颜色亮度。
     * @param key 要调整的颜色角色。
     * @param factor 调整因子，100 表示原亮度，大于 100 时变暗。
     * @return 调整后的颜色。
     */
    QColor darker(AnThemeColor key, int factor = 80)const;
    /**
     * @brief 获取指定颜色角色并替换其 Alpha 分量。
     * @param key 要查询的颜色角色。
     * @param alpha Alpha 值，范围为 0 到 255。
     * @return 设置了指定 Alpha 的颜色。
     */
    QColor withAlpha(AnThemeColor key, int alpha)const;
	
	/**
	 * @brief 为指定颜色角色设置自定义覆盖。
	 * @param key 要覆盖的颜色角色。
	 * @param value 用于覆盖的颜色。
	 */
	void setColor(AnThemeColor key, QColor value);
    /**
     * @brief 清除指定颜色角色的覆盖，恢复预设颜色。
     * @param key 要重置的颜色角色。
     */
    void resetColor(AnThemeColor key);
    /**
     * @brief 清除所有自定义颜色覆盖。
     */
    void resetAll();
public:
	/**
	 * @brief 获取主题管理器的唯一实例。
	 * @return 主题管理器实例的引用。
	 */
	static AnThemeManager& instance();
	
private:
	/**
	 * @brief 构造主题管理器并初始化预设配色。
	 */
	AnThemeManager();
	
private:
	/**
	 * @brief 填充各预设主题的颜色表。
	 */
	void initThemeColors();
	
private:
	/**
	 * @brief 当前启用的预设主题。
	 */
	AnTheme m_currentTheme;
	/**
	 * @brief 按颜色角色保存的自定义覆盖值。
	 */
	QHash<AnThemeColor, QColor> m_overrides;
	/**
	 * @brief 浅色主题的预设颜色表。
	 */
	QHash<AnThemeColor, QColor> m_lightColors;
	/**
	 * @brief 深色主题的预设颜色表。
	 */
	QHash<AnThemeColor, QColor> m_darkColors;
	/**
	 * @brief BlueNight 主题的预设颜色表。
	 */
	QHash<AnThemeColor, QColor> m_blueNightColors;
};

#endif // AN_THEMEMANAGER_H