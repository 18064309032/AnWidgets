#include <QApplication>
#include <QMainWindow>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QComboBox>
#include <QGroupBox>
#include <QDebug>

#include "antypedef.h"
#include "anthememanager.h"
#include "anpushbutton.h"

// ── 自绘 Card ──
class AnCard : public QFrame
{
    Q_OBJECT
public:
    explicit AnCard(QWidget *parent = nullptr) : QFrame(parent)
    {
        setFrameShape(QFrame::NoFrame);
        setAttribute(Qt::WA_StyledBackground);
        connect(&AnThemeManager::instance(), &AnThemeManager::themeChanged,
                this, [this]() { update(); });
    }

protected:
    void paintEvent(QPaintEvent *) override
    {
        QPainter p(this);
        p.setRenderHint(QPainter::Antialiasing);
        auto &tm = AnThemeManager::instance();

        p.setBrush(tm.color(AnThemeColor::Surface));
        p.setPen(QPen(tm.color(AnThemeColor::Border), 1));
        p.drawRoundedRect(rect().adjusted(1, 1, -1, -1), 10, 10);
    }
};

// ── 自绘 GroupBox ──
class AnGroupBox : public QGroupBox
{
    Q_OBJECT
public:
    explicit AnGroupBox(const QString &title, QWidget *parent = nullptr)
        : QGroupBox(title, parent)
    {
        connect(&AnThemeManager::instance(), &AnThemeManager::themeChanged,
                this, [this]() { update(); });
    }

protected:
    void paintEvent(QPaintEvent *) override
    {
        QPainter p(this);
        p.setRenderHint(QPainter::Antialiasing);
        auto &tm = AnThemeManager::instance();

        QRect r = rect().adjusted(1, 1, -1, -1);
        p.setPen(QPen(tm.color(AnThemeColor::Border), 1));
        p.setBrush(Qt::NoBrush);
        p.drawRoundedRect(r, 8, 8);

        QFontMetrics fm(font());
        int titleW = fm.horizontalAdvance(title()) + 16;
        int titleH = fm.height();
        QRect titleBg(16, 0, titleW, titleH);
        p.fillRect(titleBg, tm.color(AnThemeColor::Background));

        p.setPen(tm.color(AnThemeColor::TextPrimary));
        p.drawText(titleBg.adjusted(8, 0, -8, 0), Qt::AlignVCenter, title());
    }
};

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QMainWindow w;
    w.setWindowTitle("AnWidgets Demo");
    w.resize(960, 720);

    auto &tm = AnThemeManager::instance();

    // ── 基础 QSS（只设窗口背景和 ComboBox）──
    auto refreshQss = [&]() {
        QString qss;
        qss += QString("QMainWindow { background: %1; }")
                   .arg(tm.color(AnThemeColor::Background).name());
        qss += QString("QLabel { color: %1; }")
                   .arg(tm.color(AnThemeColor::TextPrimary).name());
        qss += QString(
            "QComboBox {"
            "  background: %1; color: %2; border: 1px solid %3;"
            "  border-radius: 4px; padding: 6px 12px; min-width: 140px;"
            "}"
            "QComboBox:hover { border-color: %4; }"
        ).arg(
            tm.color(AnThemeColor::Input).name(),
            tm.color(AnThemeColor::TextPrimary).name(),
            tm.color(AnThemeColor::Border).name(),
            tm.color(AnThemeColor::Primary).name()
        );
        a.setStyleSheet(qss);
    };
    refreshQss();
    QObject::connect(&tm, &AnThemeManager::themeChanged, refreshQss);

    QWidget *central = new QWidget(&w);
    w.setCentralWidget(central);

    QVBoxLayout *root = new QVBoxLayout(central);
    root->setContentsMargins(32, 24, 32, 24);
    root->setSpacing(20);

    // 标题
    QLabel *title = new QLabel("🎨 AnWidgets Demo — 全自绘控件");
    QFont titleFont;
    titleFont.setPointSize(20);
    titleFont.setBold(true);
    title->setFont(titleFont);
    root->addWidget(title);

    // 主题切换
    QHBoxLayout *switchRow = new QHBoxLayout();
    switchRow->addWidget(new QLabel("主题："));
    QComboBox *themeCombo = new QComboBox();
    themeCombo->addItem("☀️  Light", static_cast<int>(AnTheme::Light));
    themeCombo->addItem("🌙  Dark", static_cast<int>(AnTheme::Dark));
    themeCombo->addItem("🌌  BlueNight", static_cast<int>(AnTheme::BlueNight));
    switchRow->addWidget(themeCombo);
    switchRow->addStretch();
    root->addLayout(switchRow);

    QObject::connect(themeCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
                     [](int index) {
        AnThemeManager::instance().applyPreset(static_cast<AnTheme>(index));
    });

    // 按钮区
    AnGroupBox *btnGroup = new AnGroupBox("AnPushButton 三种类型", central);
    QVBoxLayout *btnLayout = new QVBoxLayout(btnGroup);
    btnLayout->setSpacing(12);

    QHBoxLayout *row1 = new QHBoxLayout();
    row1->addWidget(new QLabel("Primary："));
    AnPushButton *p1 = new AnPushButton("确定");
    p1->setButtonType(AnButtonType::Primary);
    row1->addWidget(p1);
    AnPushButton *p2 = new AnPushButton("保存");
    p2->setButtonType(AnButtonType::Primary);
    row1->addWidget(p2);
    AnPushButton *p3 = new AnPushButton("禁用");
    p3->setButtonType(AnButtonType::Primary);
    p3->setEnabled(false);
    row1->addWidget(p3);
    row1->addStretch();
    btnLayout->addLayout(row1);

    QHBoxLayout *row2 = new QHBoxLayout();
    row2->addWidget(new QLabel("Secondary："));
    AnPushButton *s1 = new AnPushButton("取消");
    s1->setButtonType(AnButtonType::Secondary);
    row2->addWidget(s1);
    AnPushButton *s2 = new AnPushButton("编辑");
    s2->setButtonType(AnButtonType::Secondary);
    row2->addWidget(s2);
    AnPushButton *s3 = new AnPushButton("禁用");
    s3->setButtonType(AnButtonType::Secondary);
    s3->setEnabled(false);
    row2->addWidget(s3);
    row2->addStretch();
    btnLayout->addLayout(row2);

    QHBoxLayout *row3 = new QHBoxLayout();
    row3->addWidget(new QLabel("Ghost："));
    AnPushButton *g1 = new AnPushButton("文字链接");
    g1->setButtonType(AnButtonType::Ghost);
    row3->addWidget(g1);
    AnPushButton *g2 = new AnPushButton("查看更多 →");
    g2->setButtonType(AnButtonType::Ghost);
    row3->addWidget(g2);
    row3->addStretch();
    btnLayout->addLayout(row3);

    root->addWidget(btnGroup);

    // Card
    AnCard *card = new AnCard(central);
    QVBoxLayout *cardLayout = new QVBoxLayout(card);
    cardLayout->setContentsMargins(24, 20, 24, 20);
    cardLayout->setSpacing(12);

    QLabel *cardTitle = new QLabel("📋 自绘 Card 容器");
    QFont cardTitleFont;
    cardTitleFont.setPointSize(13);
    cardTitleFont.setBold(true);
    cardTitle->setFont(cardTitleFont);
    cardLayout->addWidget(cardTitle);
    cardLayout->addWidget(new QLabel("这是一个完全自绘的卡片容器，Surface + Border 圆角 10px。"));
    cardLayout->addWidget(new QLabel("所有颜色从 ThemeManager 取，切主题自动刷新。"));

    QHBoxLayout *cardBtns = new QHBoxLayout();
    cardBtns->addStretch();
    AnPushButton *cancelBtn = new AnPushButton("取消");
    cancelBtn->setButtonType(AnButtonType::Secondary);
    cardBtns->addWidget(cancelBtn);
    AnPushButton *saveBtn = new AnPushButton("保存");
    saveBtn->setButtonType(AnButtonType::Primary);
    cardBtns->addWidget(saveBtn);
    cardLayout->addLayout(cardBtns);

    root->addWidget(card);
    root->addStretch();

    tm.applyPreset(AnTheme::Light);
    themeCombo->setCurrentIndex(0);

    w.show();
    return a.exec();
}

#include "main.moc"
