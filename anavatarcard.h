#ifndef AN_AVATARCARD_H
#define AN_AVATARCARD_H

#include <QLabel>
#include <QPixmap>

#include "ancard.h"

/**
 * @brief 展示圆形头像、名称和简介的主题卡片。
 */
class AnAvatarCard : public AnCard
{
    Q_OBJECT

public:
    /**
     * @brief 构造头像卡片。
     * @param parent 管理生命周期的父控件，可为 nullptr。
     */
    explicit AnAvatarCard(QWidget *parent = nullptr);

    /**
     * @brief 设置头像图片。
     * @param avatar 头像图片，空图片会使用名称首字。
     */
    void setAvatar(const QPixmap &avatar);

    /**
     * @brief 设置卡片上的名称。
     * @param name 显示的名称。
     */
    void setName(const QString &name);

    /**
     * @brief 设置卡片上的简介。
     * @param description 显示的简介，空字符串会隐藏简介行。
     */
    void setDescription(const QString &description);

protected:
    /**
     * @brief 绘制卡片背景和圆形头像。
     * @param event 当前绘制事件。
     */
    void paintEvent(QPaintEvent *event) override;

private:
    /**
     * @brief 在布局中为头像预留空间。
     */
    QWidget *m_avatarSlot = nullptr;

    /**
     * @brief 当前头像图片。
     */
    QPixmap m_avatar;

    /**
     * @brief 显示名称的主题标签。
     */
    QLabel *m_nameLabel = nullptr;

    /**
     * @brief 显示简介的主题标签。
     */
    QLabel *m_descriptionLabel = nullptr;
};

#endif
