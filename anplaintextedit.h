#ifndef ANPLAINTEXTEDIT_H
#define ANPLAINTEXTEDIT_H

#include <QPlainTextEdit>

/**
 * @brief 提供主题配色和圆角外框的多行纯文本编辑器。
 */
class AnPlainTextEdit : public QPlainTextEdit
{
    Q_OBJECT

public:
    /**
     * @brief 构造多行纯文本编辑器。
     * @param parent 管理生命周期的父对象，可为 nullptr。
     */
    explicit AnPlainTextEdit(QWidget *parent = nullptr);

protected:
    /**
     * @brief 处理外框绘制和焦点变化。
     * @param event 当前控件事件。
     * @return 事件是否已处理。
     */
    bool event(QEvent *event) override;

};

#endif
