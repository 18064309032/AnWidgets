#include <QAction>
#include <QApplication>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QItemSelectionModel>
#include <QLabel>
#include <QPainter>
#include <QPushButton>
#include <QSortFilterProxyModel>
#include <QStandardItemModel>
#include <QStackedWidget>
#include <QVBoxLayout>
#include <QWidget>

#include "anavatarcard.h"
#include "ancard.h"
#include "ancheckbox.h"
#include "ancombobox.h"
#include "andoublespinbox.h"
#include "anlineedit.h"
#include "anlistview.h"
#include "anmenu.h"
#include "anmessagebox.h"
#include "annavigationbar.h"
#include "anplaintextedit.h"
#include "anprogressbar.h"
#include "anpushbutton.h"
#include "anradiobutton.h"
#include "ansegmentedcontrol.h"
#include "anslider.h"
#include "anspinbox.h"
#include "answitch.h"
#include "antaskcard.h"
#include "anthemedlabel_p.h"
#include "anthememanager.h"
#include "antoolbutton.h"
#include "anwindow.h"

/**
 * @brief 集中展示 AnWidgets 控件和主题切换的示例窗口。
 */
class AnWidgetsDemo : public AnWindow
{
public:
    /**
     * @brief 创建示例窗口并连接控件交互。
     */
    AnWidgetsDemo()
    {
        setWindowTitle(QStringLiteral("AnWidgets 控件示例"));
        resize(880, 620);
        setFramelessEnabled(true);
        setBorderColor(QColor(161, 183, 207));
        setShadowColor(QColor(62, 91, 126, 96));
        setShadowRadius(16);

        headerLayout()->setContentsMargins(24, 16, 24, 8);
        QLabel *title = new AnThemedLabel(QStringLiteral("AnWidgets 控件示例"), this);
        QFont titleFont = title->font();
        titleFont.setPointSize(18);
        titleFont.setBold(true);
        title->setFont(titleFont);
        headerLayout()->addWidget(title);
        headerLayout()->addStretch();
        headerLayout()->addWidget(new AnThemedLabel(QStringLiteral("主题"), this));

        statusLayout()->setContentsMargins(24, 4, 24, 12);
        m_statusLabel = new AnThemedLabel(QStringLiteral("就绪"), this);
        statusLayout()->addWidget(m_statusLabel);
        statusLayout()->addStretch();

        contentLayout()->setContentsMargins(16, 0, 16, 0);
        QHBoxLayout *workspaceLayout = new QHBoxLayout();
        workspaceLayout->setSpacing(12);
        contentLayout()->addLayout(workspaceLayout);
        m_navigation = new AnNavigationBar(this);
        QStackedWidget *pages = new QStackedWidget(this);
        workspaceLayout->addWidget(m_navigation);
        workspaceLayout->addWidget(pages, 1);

        QWidget *basicPage = new QWidget(pages);
        pages->addWidget(basicPage);
        pages->addWidget(createComponentsPage(pages));
        pages->addWidget(createTaskCardPage(pages));
        QWidget *aboutPage = new QWidget(pages);
        QVBoxLayout *aboutLayout = new QVBoxLayout(aboutPage);
        aboutLayout->setContentsMargins(24, 24, 24, 24);
        aboutLayout->addWidget(new AnThemedLabel(QStringLiteral("关于 AnWidgets"), aboutPage));
        aboutLayout->addWidget(new AnThemedLabel(QStringLiteral("使用 Qt Widgets 构建的桌面工具组件库。"), aboutPage, AnThemeColor::TextSecondary));
        aboutLayout->addStretch();
        pages->addWidget(aboutPage);
        QWidget *settingsPage = new QWidget(pages);
        QFormLayout *settingsLayout = new QFormLayout(settingsPage);
        settingsLayout->setContentsMargins(24, 24, 24, 24);
        pages->addWidget(settingsPage);
        AnAvatarCard *profile = new AnAvatarCard(m_navigation);
        profile->setName(QStringLiteral("AnWidgets"));
        profile->setDescription(QStringLiteral("桌面工具组件库"));
        m_navigation->setHeaderWidget(profile);
        const int components = m_navigation->addGroup(QStringLiteral("控件"));
        m_navigation->addGroupItem(components, QIcon(), QStringLiteral("基础控件"));
        const int tools = m_navigation->addGroup(QStringLiteral("应用组件"));
        m_cardsBranch = m_navigation->addBranch(tools, QIcon(), QStringLiteral("卡片"), true);
        m_navigation->addChildItem(m_cardsBranch, QIcon(), QStringLiteral("卡片与反馈"));
        m_navigation->addChildItem(m_cardsBranch, QIcon(), QStringLiteral("任务卡片"));
        m_navigation->addFooterItem(QIcon(), QStringLiteral("关于"));
        m_navigation->addFooterItem(QIcon(), QStringLiteral("设置"));
        connect(&AnThemeManager::instance(), &AnThemeManager::themeChanged, this, &AnWidgetsDemo::refreshNavigationIcons);
        refreshNavigationIcons();
        connect(m_navigation, &AnNavigationBar::currentChanged, pages, &QStackedWidget::setCurrentIndex);
        connect(pages, &QStackedWidget::currentChanged, m_navigation, &AnNavigationBar::setCurrentIndex);

        QVBoxLayout *layout = new QVBoxLayout(basicPage);
        layout->setContentsMargins(24, 24, 24, 24);
        layout->setSpacing(16);

        QFormLayout *form = new QFormLayout();
        form->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
        form->setVerticalSpacing(12);
        layout->addLayout(form);

        m_themeCombo = new AnComboBox(this);
        m_themeCombo->setMinimumWidth(120);
        m_themeCombo->addItem(QStringLiteral("浅色"), static_cast<int>(AnTheme::Light));
        m_themeCombo->addItem(QStringLiteral("深色"), static_cast<int>(AnTheme::Dark));
        m_themeCombo->addItem(QStringLiteral("蓝夜"), static_cast<int>(AnTheme::BlueNight));
        m_themeCombo->setCurrentIndex(m_themeCombo->findData(static_cast<int>(AnThemeManager::instance().currentTheme())));
        headerLayout()->addWidget(m_themeCombo);

        AnComboBox *settingsTheme = new AnComboBox(settingsPage);
        settingsTheme->setModel(m_themeCombo->model());
        settingsTheme->setCurrentIndex(m_themeCombo->currentIndex());
        settingsLayout->addRow(new AnThemedLabel(QStringLiteral("界面主题"), settingsPage), settingsTheme);
        connect(settingsTheme, QOverload<int>::of(&QComboBox::currentIndexChanged), m_themeCombo, &QComboBox::setCurrentIndex);
        connect(m_themeCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), settingsTheme, &QComboBox::setCurrentIndex);

        m_lineEdit = new AnLineEdit(this);
        m_lineEdit->setPlaceholderText(QStringLiteral("输入内容，右键可打开全选菜单"));
        m_lineEdit->setClearButtonEnabled(true);
        m_lineEdit->setContextMenuPolicy(Qt::CustomContextMenu);
        connect(m_lineEdit, &QWidget::customContextMenuRequested, this, &AnWidgetsDemo::showInputMenu);
        form->addRow(new AnThemedLabel(QStringLiteral("单行输入"), basicPage), m_lineEdit);

        m_validationCombo = new AnComboBox(this);
        m_validationCombo->setMaximumWidth(240);
        m_validationCombo->addItem(QStringLiteral("正常"), static_cast<int>(AnLineEdit::ValidationState::Normal));
        m_validationCombo->addItem(QStringLiteral("警告"), static_cast<int>(AnLineEdit::ValidationState::Warning));
        m_validationCombo->addItem(QStringLiteral("错误"), static_cast<int>(AnLineEdit::ValidationState::Error));
        form->addRow(new AnThemedLabel(QStringLiteral("校验状态"), basicPage), m_validationCombo);

        AnComboBox *formatCombo = new AnComboBox(this);
        formatCombo->setMaximumWidth(240);
        formatCombo->setMaxVisibleItems(6);
        formatCombo->addItems({QStringLiteral("TXT"), QStringLiteral("CSV"), QStringLiteral("TSV"), QStringLiteral("JSON"), QStringLiteral("XML"), QStringLiteral("YAML"), QStringLiteral("HTML"), QStringLiteral("Markdown"), QStringLiteral("PDF"), QStringLiteral("XLSX"), QStringLiteral("PNG"), QStringLiteral("JPEG")});
        form->addRow(new AnThemedLabel(QStringLiteral("输出格式"), basicPage), formatCombo);

        AnCheckBox *checkBox = new AnCheckBox(QStringLiteral("包含子文件夹（三态）"), this);
        checkBox->setTristate(true);
        checkBox->setCheckState(Qt::PartiallyChecked);
        form->addRow(new AnThemedLabel(QStringLiteral("复选框"), basicPage), checkBox);

        QWidget *radioGroup = new QWidget(this);
        QHBoxLayout *radioLayout = new QHBoxLayout(radioGroup);
        radioLayout->setContentsMargins(0, 0, 0, 0);
        AnRadioButton *standard = new AnRadioButton(QStringLiteral("标准"), radioGroup);
        AnRadioButton *compact = new AnRadioButton(QStringLiteral("紧凑"), radioGroup);
        standard->setChecked(true);
        radioLayout->addWidget(standard);
        radioLayout->addWidget(compact);
        radioLayout->addStretch();
        form->addRow(new AnThemedLabel(QStringLiteral("单选框"), basicPage), radioGroup);

        AnSpinBox *spinBox = new AnSpinBox(this);
        spinBox->setMaximumWidth(160);
        spinBox->setRange(1, 100);
        spinBox->setValue(12);
        form->addRow(new AnThemedLabel(QStringLiteral("整数输入"), basicPage), spinBox);

        AnDoubleSpinBox *doubleSpinBox = new AnDoubleSpinBox(this);
        doubleSpinBox->setMaximumWidth(160);
        doubleSpinBox->setRange(0.25, 10.0);
        doubleSpinBox->setDecimals(2);
        doubleSpinBox->setSingleStep(0.25);
        doubleSpinBox->setValue(1.25);
        form->addRow(new AnThemedLabel(QStringLiteral("小数输入"), basicPage), doubleSpinBox);

        m_slider = new AnSlider(Qt::Horizontal, this);
        m_slider->setRange(0, 100);
        m_slider->setValue(40);
        m_slider->setTickPosition(QSlider::TicksBelow);
        m_slider->setTickInterval(10);
        form->addRow(new AnThemedLabel(QStringLiteral("滑块"), basicPage), m_slider);

        m_progressBar = new AnProgressBar(this);
        m_progressBar->setRange(0, 100);
        m_progressBar->setValue(m_slider->value());
        form->addRow(new AnThemedLabel(QStringLiteral("进度条"), basicPage), m_progressBar);

        QHBoxLayout *buttons = new QHBoxLayout();
        AnPushButton *resetButton = new AnPushButton(QStringLiteral("重置进度"), this);
        AnPushButton *busyButton = new AnPushButton(QStringLiteral("切换忙碌状态"), this);
        busyButton->setAppearance(AnPushButton::Appearance::Secondary);
        AnPushButton *clearButton = new AnPushButton(QStringLiteral("清除输入"), this);
        clearButton->setAppearance(AnPushButton::Appearance::Ghost);
        buttons->addWidget(resetButton);
        buttons->addWidget(busyButton);
        buttons->addWidget(clearButton);
        AnToolButton *menuButton = new AnToolButton(this);
        menuButton->setText(QStringLiteral("操作菜单"));
        menuButton->setToolButtonStyle(Qt::ToolButtonTextOnly);
        menuButton->setPopupMode(QToolButton::InstantPopup);
        AnMenu *menu = new AnMenu(menuButton);
        menu->addAction(QStringLiteral("清除输入"), m_lineEdit, &QLineEdit::clear);
        QAction *showProgressText = menu->addAction(QStringLiteral("显示进度文字"));
        showProgressText->setCheckable(true);
        showProgressText->setChecked(m_progressBar->isTextVisible());
        connect(showProgressText, &QAction::toggled, m_progressBar, &QProgressBar::setTextVisible);
        menu->addAction(QStringLiteral("导出结果"))->setEnabled(false);
        menu->addSeparator();
        AnMenu *progressMenu = new AnMenu(QStringLiteral("进度操作"), menu);
        progressMenu->addAction(QStringLiteral("重置进度"), this, &AnWidgetsDemo::resetProgress);
        progressMenu->addAction(QStringLiteral("切换忙碌状态"), this, &AnWidgetsDemo::toggleBusy);
        menu->addMenu(progressMenu);
        menuButton->setMenu(menu);
        buttons->addWidget(menuButton);
        buttons->addStretch();
        layout->addLayout(buttons);
        layout->addStretch();

        connect(m_themeCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &AnWidgetsDemo::selectTheme);
        connect(m_validationCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &AnWidgetsDemo::selectValidation);
        connect(m_slider, &QSlider::valueChanged, m_progressBar, &QProgressBar::setValue);
        connect(resetButton, &QPushButton::clicked, this, &AnWidgetsDemo::resetProgress);
        connect(busyButton, &QPushButton::clicked, this, &AnWidgetsDemo::toggleBusy);
        connect(clearButton, &QPushButton::clicked, m_lineEdit, &QLineEdit::clear);
    }

private:
    /**
     * @brief 绘制示例导航项使用的线性图标。
     * @param index 导航项的索引。
     * @return 当前主题的图标。
     */
    QIcon createNavigationIcon(int index) const
    {
        QPixmap pixmap(32, 32);
        pixmap.setDevicePixelRatio(2);
        pixmap.fill(Qt::transparent);
        QPainter painter(&pixmap);
        painter.setRenderHint(QPainter::Antialiasing);
        painter.setPen(QPen(AnThemeManager::instance().color(AnThemeColor::TextSecondary), 1.4, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
        if (index == 0)
        {
            painter.drawRoundedRect(QRectF(2, 2.5, 5, 5), 1, 1);
            painter.drawRoundedRect(QRectF(9, 2.5, 5, 5), 1, 1);
            painter.drawRoundedRect(QRectF(2, 9.5, 5, 5), 1, 1);
            painter.drawRoundedRect(QRectF(9, 9.5, 5, 5), 1, 1);
        }
        else if (index == 1)
        {
            painter.drawRoundedRect(QRectF(2, 3.5, 12, 9), 1.5, 1.5);
            painter.drawLine(QPointF(2, 6.5), QPointF(14, 6.5));
            painter.drawLine(QPointF(5, 9.5), QPointF(10, 9.5));
        }
        else if (index == 3)
        {
            painter.drawEllipse(QRectF(2, 2, 12, 12));
            painter.drawPoint(QPointF(8, 5));
            painter.drawLine(QPointF(8, 7.5), QPointF(8, 11));
        }
        else if (index == 4)
        {
            painter.drawLine(QPointF(2, 4), QPointF(14, 4));
            painter.drawLine(QPointF(2, 8), QPointF(14, 8));
            painter.drawLine(QPointF(2, 12), QPointF(14, 12));
            painter.drawLine(QPointF(5, 2.5), QPointF(5, 5.5));
            painter.drawLine(QPointF(10, 6.5), QPointF(10, 9.5));
            painter.drawLine(QPointF(7, 10.5), QPointF(7, 13.5));
        }
        else if (index == 6)
        {
            painter.drawLine(QPointF(3, 4), QPointF(13, 4));
            painter.drawLine(QPointF(3, 8), QPointF(13, 8));
            painter.drawLine(QPointF(3, 12), QPointF(13, 12));
        }
        else if (index == 5)
        {
            painter.drawRoundedRect(QRectF(2, 5, 10, 9), 1.5, 1.5);
            painter.drawLine(QPointF(5, 2), QPointF(13, 2));
            painter.drawLine(QPointF(14, 3), QPointF(14, 11));
        }
        else
        {
            painter.drawRoundedRect(QRectF(3.5, 2, 9, 12), 1.5, 1.5);
            painter.drawLine(QPointF(6, 6), QPointF(10, 6));
            painter.drawLine(QPointF(6, 9), QPointF(10, 9));
        }
        painter.end();
        return QIcon(pixmap);
    }

    /**
     * @brief 在主题切换时重新绘制示例导航图标。
     */
    void refreshNavigationIcons()
    {
        for (int index = 0; index < 5; ++index)
        {
            m_navigation->setItemIcon(index, createNavigationIcon(index));
        }
        m_navigation->setBranchIcon(m_cardsBranch, createNavigationIcon(5));
        m_navigation->setToggleIcon(createNavigationIcon(6));
    }

    /**
     * @brief 创建任务卡片示例页。
     * @param parent 管理页面生命周期的父控件。
     * @return 新创建的示例页面。
     */
    QWidget *createTaskCardPage(QWidget *parent)
    {
        QWidget *page = new QWidget(parent);
        QVBoxLayout *layout = new QVBoxLayout(page);
        layout->setContentsMargins(0, 12, 0, 0);

        m_demoTaskCard = new AnTaskCard(page);
        m_demoTaskCard->setMaximumWidth(460);
        m_demoTaskCard->setTitle(QStringLiteral("批量转换文件"));
        pauseDemoTask();
        AnMenu *menu = new AnMenu(m_demoTaskCard);
        menu->addAction(QStringLiteral("继续处理"), this, &AnWidgetsDemo::resumeDemoTask);
        menu->addAction(QStringLiteral("暂停处理"), this, &AnWidgetsDemo::pauseDemoTask);
        m_demoTaskCard->setActionMenu(menu);
        layout->addWidget(m_demoTaskCard);
        layout->addStretch();
        return page;
    }

    /**
     * @brief 在示例中显示任务正在处理。
     */
    void resumeDemoTask()
    {
        m_demoTaskCard->setStatusText(QStringLiteral("正在转换文件…"));
        m_demoTaskCard->setProgressRange(0, 0);
    }

    /**
     * @brief 在示例中显示任务已暂停和已完成的进度。
     */
    void pauseDemoTask()
    {
        m_demoTaskCard->setStatusText(QStringLiteral("已暂停 · 已处理 4 / 10 个文件"));
        m_demoTaskCard->setProgressRange(0, 10);
        m_demoTaskCard->setProgressValue(4);
    }

    /**
     * @brief 创建展示卡片、列表、编辑器和开关的页面。
     * @param parent 管理页面生命周期的父控件。
     * @return 新创建的示例页面。
     */
    QWidget *createComponentsPage(QWidget *parent)
    {
        QWidget *page = new QWidget(parent);
        QVBoxLayout *pageLayout = new QVBoxLayout(page);
        pageLayout->setContentsMargins(0, 12, 0, 0);
        AnCard *card = new AnCard(page);
        pageLayout->addWidget(card);
        pageLayout->addStretch();

        QVBoxLayout *cardLayout = new QVBoxLayout(card);
        cardLayout->setSpacing(12);
        QLabel *heading = new AnThemedLabel(QStringLiteral("任务记录"), card);
        QFont headingFont = heading->font();
        headingFont.setBold(true);
        heading->setFont(headingFont);
        cardLayout->addWidget(heading);

        AnSegmentedControl *taskFilter = new AnSegmentedControl(card);
        taskFilter->addTab(QStringLiteral("全部"));
        taskFilter->addTab(QStringLiteral("运行中"));
        taskFilter->addTab(QStringLiteral("已完成"));
        taskFilter->setMaximumWidth(300);
        connect(taskFilter, &QTabBar::currentChanged, this, &AnWidgetsDemo::filterTasks);
        cardLayout->addWidget(taskFilter);

        QHBoxLayout *content = new QHBoxLayout();
        content->setSpacing(12);
        QVBoxLayout *taskColumn = new QVBoxLayout();
        taskColumn->setSpacing(6);
        taskColumn->addWidget(new AnThemedLabel(QStringLiteral("任务"), card));
        m_taskList = new AnListView(card);
        m_taskList->setMaximumWidth(220);
        m_taskList->setFixedHeight(132);
        m_taskList->setEditTriggers(QAbstractItemView::NoEditTriggers);
        QStandardItemModel *tasks = new QStandardItemModel(m_taskList);

        QStandardItem *scan = new QStandardItem(QStringLiteral("扫描输入文件"));
        scan->setData(QStringLiteral("扫描完成，共找到 12 个文件。"), Qt::UserRole);
        scan->setData(2, Qt::UserRole + 1);
        tasks->appendRow(scan);
        QStandardItem *convert = new QStandardItem(QStringLiteral("转换文件格式"));
        convert->setData(QStringLiteral("等待开始转换。"), Qt::UserRole);
        convert->setData(1, Qt::UserRole + 1);
        tasks->appendRow(convert);
        QStandardItem *exportResults = new QStandardItem(QStringLiteral("导出处理结果"));
        exportResults->setData(QStringLiteral("转换完成后即可导出结果。"), Qt::UserRole);
        exportResults->setData(1, Qt::UserRole + 1);
        tasks->appendRow(exportResults);
        m_taskFilterModel = new QSortFilterProxyModel(m_taskList);
        m_taskFilterModel->setSourceModel(tasks);
        m_taskFilterModel->setFilterRole(Qt::UserRole + 1);
        m_taskList->setModel(m_taskFilterModel);
        taskColumn->addWidget(m_taskList);
        taskColumn->addStretch();
        content->addLayout(taskColumn, 1);

        QVBoxLayout *logColumn = new QVBoxLayout();
        logColumn->setSpacing(6);
        logColumn->addWidget(new AnThemedLabel(QStringLiteral("运行日志"), card));
        m_taskLog = new AnPlainTextEdit(card);
        m_taskLog->setMinimumHeight(160);
        m_taskLog->setReadOnly(true);
        logColumn->addWidget(m_taskLog);
        content->addLayout(logColumn, 2);
        cardLayout->addLayout(content);

        connect(m_taskList->selectionModel(), &QItemSelectionModel::currentChanged, this, &AnWidgetsDemo::showTaskLog);
        filterTasks(0);

        QHBoxLayout *controls = new QHBoxLayout();
        AnSwitch *readOnlySwitch = new AnSwitch(card);
        readOnlySwitch->setText(QStringLiteral("日志只读"));
        readOnlySwitch->setChecked(true);
        connect(readOnlySwitch, &QAbstractButton::toggled, m_taskLog, &QPlainTextEdit::setReadOnly);
        controls->addWidget(readOnlySwitch);
        controls->addStretch();
        AnPushButton *messageButton = new AnPushButton(QStringLiteral("显示消息框"), card);
        messageButton->setAppearance(AnPushButton::Appearance::Secondary);
        connect(messageButton, &QPushButton::clicked, this, &AnWidgetsDemo::showTaskMessage);
        controls->addWidget(messageButton);
        cardLayout->addLayout(controls);

        AnAvatarCard *avatarCard = new AnAvatarCard(page);
        avatarCard->setMaximumWidth(400);
        avatarCard->setName(QStringLiteral("AnWidgets"));
        avatarCard->setDescription(QStringLiteral("桌面工具组件库"));
        pageLayout->insertWidget(pageLayout->count() - 1, avatarCard);
        return page;
    }

    /**
     * @brief 根据当前分段筛选任务。
     * @param index 当前分段的索引，零表示显示全部任务。
     */
    void filterTasks(int index)
    {
        m_taskFilterModel->setFilterFixedString(index > 0 ? QString::number(index) : QString());
        const QModelIndex first = m_taskFilterModel->index(0, 0);
        m_taskList->setCurrentIndex(first);
        showTaskLog(first);
    }

    /**
     * @brief 显示当前任务的日志。
     * @param current 当前任务对应的模型索引。
     */
    void showTaskLog(const QModelIndex &current)
    {
        m_taskLog->setPlainText(current.data(Qt::UserRole).toString());
    }

    /**
     * @brief 打开任务消息框。
     */
    void showTaskMessage()
    {
        AnMessageBox message(QMessageBox::Information, QStringLiteral("任务记录"), QStringLiteral("扫描完成，共找到 12 个文件。"), QMessageBox::Ok, this);
        message.button(QMessageBox::Ok)->setText(QStringLiteral("确定"));
        message.exec();
    }

    /**
     * @brief 在输入框上展示单项圆角菜单。
     * @param position 输入框局部坐标中的弹出位置。
     */
    void showInputMenu(const QPoint &position)
    {
        AnMenu menu(m_lineEdit);
        menu.addAction(QStringLiteral("全选"), m_lineEdit, &QLineEdit::selectAll);
        menu.exec(m_lineEdit->mapToGlobal(position));
    }

    /**
     * @brief 应用所选主题。
     * @param index 主题选项的索引。
     */
    void selectTheme(int index)
    {
        if (index >= 0)
        {
            AnThemeManager::instance().applyPreset(static_cast<AnTheme>(m_themeCombo->itemData(index).toInt()));
        }
    }

    /**
     * @brief 将所选校验状态应用到输入框。
     * @param index 校验状态选项的索引。
     */
    void selectValidation(int index)
    {
        if (index >= 0)
        {
            m_lineEdit->setValidationState(static_cast<AnLineEdit::ValidationState>(m_validationCombo->itemData(index).toInt()));
        }
    }

    /**
     * @brief 退出忙碌状态并将进度归零。
     */
    void resetProgress()
    {
        m_statusLabel->setText(QStringLiteral("就绪"));
        m_progressBar->setRange(0, 100);
        m_slider->setEnabled(true);
        m_slider->setValue(0);
        m_progressBar->setValue(0);
    }

    /**
     * @brief 在确定进度和忙碌动画之间切换。
     */
    void toggleBusy()
    {
        const bool busy = m_progressBar->maximum() == 0;
        m_statusLabel->setText(busy ? QStringLiteral("就绪") : QStringLiteral("处理中"));
        m_slider->setEnabled(busy);
        m_progressBar->setRange(0, busy ? 100 : 0);
        if (busy)
        {
            m_progressBar->setValue(m_slider->value());
        }
    }

    /**
     * @brief 用于切换全局主题的下拉框。
     */
    AnComboBox *m_themeCombo = nullptr;

    /**
     * @brief 用于选择输入校验状态的下拉框。
     */
    AnComboBox *m_validationCombo = nullptr;

    /**
     * @brief 展示输入和校验状态的单行输入框。
     */
    AnLineEdit *m_lineEdit = nullptr;

    /**
     * @brief 用于控制确定进度值的滑块。
     */
    AnSlider *m_slider = nullptr;

    /**
     * @brief 展示确定进度和忙碌动画的进度条。
     */
    AnProgressBar *m_progressBar = nullptr;

    /**
     * @brief 显示示例窗口的当前处理状态。
     */
    QLabel *m_statusLabel = nullptr;

    /**
     * @brief 显示示例页面的侧边导航栏。
     */
    AnNavigationBar *m_navigation = nullptr;

    /**
     * @brief 示例中卡片二级导航入口的索引。
     */
    int m_cardsBranch = -1;

    /**
     * @brief 显示卡片中当前任务的运行日志。
     */
    AnPlainTextEdit *m_taskLog = nullptr;

    /**
     * @brief 显示筛选后的任务列表。
     */
    AnListView *m_taskList = nullptr;

    /**
     * @brief 根据任务状态筛选示例数据。
     */
    QSortFilterProxyModel *m_taskFilterModel = nullptr;

    /**
     * @brief 展示批量转换任务的示例卡片。
     */
    AnTaskCard *m_demoTaskCard = nullptr;
};

/**
 * @brief 启动 AnWidgets 控件示例。
 * @param argc 命令行参数数量。
 * @param argv 命令行参数数组。
 * @return Qt 事件循环的退出代码。
 */
int main(int argc, char *argv[])
{
    QApplication application(argc, argv);
    AnWidgetsDemo demo;
    demo.show();
    return application.exec();
}
