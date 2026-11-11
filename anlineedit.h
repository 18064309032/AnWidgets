#ifndef AN_LINEEDIT_H
#define AN_LINEEDIT_H

#include <QLineEdit>

/**
 * @brief 提供主题配色、圆角和校验状态的单行输入框。
 */
class AnLineEdit : public QLineEdit
{
    Q_OBJECT

public:
    /**
     * @brief 定义输入框显示的校验状态。
     */
    enum class ValidationState
    {
        Normal,
        Warning,
        Error
    };
    Q_ENUM(ValidationState)

    Q_PROPERTY(int cornerRadius READ cornerRadius WRITE setCornerRadius NOTIFY cornerRadiusChanged)
    Q_PROPERTY(ValidationState validationState READ validationState WRITE setValidationState NOTIFY validationStateChanged)

    /**
     * @brief 构造单行输入框。
     * @param parent 管理生命周期的父对象，可为 nullptr。
     */
    explicit AnLineEdit(QWidget *parent = nullptr);

    /**
     * @brief 构造带初始文本的单行输入框。
     * @param text 输入框的初始文本。
     * @param parent 管理生命周期的父对象，可为 nullptr。
     */
    explicit AnLineEdit(const QString &text, QWidget *parent = nullptr);

    /**
     * @brief 获取输入框的圆角半径。
     * @return 圆角半径，单位为像素。
     */
    int cornerRadius() const;

    /**
     * @brief 设置输入框的圆角半径。
     * @param radius 圆角半径，单位为像素，负数按零处理。
     */
    void setCornerRadius(int radius);

    /**
     * @brief 获取输入框当前显示的校验状态。
     * @return 当前校验状态。
     */
    ValidationState validationState() const;

    /**
     * @brief 设置输入框显示的校验状态。
     * @param state 由调用方提供的校验状态。
     */
    void setValidationState(ValidationState state);

signals:
    /**
     * @brief 在圆角半径改变时发出通知。
     * @param radius 更新后的圆角半径。
     */
    void cornerRadiusChanged(int radius);

    /**
     * @brief 在校验状态改变时发出通知。
     * @param state 更新后的校验状态。
     */
    void validationStateChanged(ValidationState state);

protected:
    /**
     * @brief 绘制原生输入内容和主题圆角边框。
     * @param event 当前绘制事件。
     */
    void paintEvent(QPaintEvent *event) override;

    /**
     * @brief 在焦点或启用状态变化时刷新边框。
     * @param event 当前控件事件。
     * @return QLineEdit 的事件处理结果。
     */
    bool event(QEvent *event) override;

private:
    friend class AnEditorColorEffect;

    /**
     * @brief 获取当前焦点和校验状态对应的主题边框颜色。
     * @return 输入框的边框颜色。
     */
    QColor borderColor() const;

    /**
     * @brief 连接主题通知并应用初始样式。
     */
    void initialize();

    /**
     * @brief 输入框的圆角半径，单位为像素。
     */
    int m_cornerRadius = 6;

    /**
     * @brief 输入框当前显示的校验状态。
     */
    ValidationState m_validationState = ValidationState::Normal;
};

#endif
