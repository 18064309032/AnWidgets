#ifndef AN_EDITORCOLOREFFECT_P_H
#define AN_EDITORCOLOREFFECT_P_H

#include <QGraphicsEffect>
#include <QWidget>

/**
 * @brief 将原生编辑器的浅色绘制结果转换为当前深色主题。
 */
class AnEditorColorEffect : public QGraphicsEffect
{
public:
    /**
     * @brief 构造编辑区配色效果。
     * @param editor 提供原生绘制结果的编辑控件。
     */
    explicit AnEditorColorEffect(QWidget *editor);

protected:
    /**
     * @brief 绘制经过主题转换的编辑区。
     * @param painter 当前绘制所用的画家。
     */
    void draw(QPainter *painter) override;

private:
    /**
     * @brief 根据主题背景亮度启用或停用转换。
     */
    void refreshTheme();

    /**
     * @brief 提供原生绘制结果的编辑控件。
     */
    QWidget *m_editor;
};

#endif
