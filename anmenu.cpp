#include "anmenu.h"
#include "anmenustyle.h"
#include "anthememanager.h"

AnMenu::AnMenu(QWidget *parent)
    : AnMenu(QString(), parent)
{
}

AnMenu::AnMenu(const QString &title, QWidget *parent)
    : QMenu(title, parent)
{
    setWindowFlags(windowFlags() | Qt::FramelessWindowHint | Qt::NoDropShadowWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);
    setStyle(new AnMenuStyle(this));
    connect(&AnThemeManager::instance(), &AnThemeManager::themeChanged, this, QOverload<>::of(&AnMenu::update));
}
