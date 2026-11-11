#ifndef	AN_TYPEDEF_H
#define AN_TYPEDEF_H

#include <QHash>

enum class AnTheme
{
    Light,
    Dark,
    BlueNight
};

enum class AnThemeColor 
{
    Background,          // 窗口根：QMainWindow 最外层
    Surface,             // 所有容器：QFrame / QGroupBox / QDialog / 普通卡片
    Input,               // 输入框
    PopupBackground,     // 弹窗：Menu / Tooltip / 下拉框（和 Dialog 不同，Popup 是临时浮层）
    Divider,             // 细分割线
    Border,              // 粗边框

    // ════════ 文字 ════════
    TextPrimary,
    TextSecondary,
    TextMuted,
    TextDisabled,
    TextOnAccent,

    // ════════ 交互 ════════
    Primary,
    Hover,
    Pressed,
    Disabled,
    Selection,

    // ════════ 状态 ════════
    Success,
    Info,
    Warning,
    Error,

    // ════════ 图标 ════════
    IconPrimary,
    IconSecondary,
    IconOnAccent
};

// QHash 需要 qHash 重载 —— 直接转 int 就行
inline uint qHash(AnThemeColor key, uint seed = 0) noexcept
{
    return ::qHash(static_cast<int>(key), seed);
}

#endif // AN_TYPEDEF_H