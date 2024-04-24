// SPDX-FileCopyrightText: 2024 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "window/mainwindow.h"
#include "defenderdbusservice.h"

#include <DApplication>
#include <DWidgetUtil>

DWIDGET_USE_NAMESPACE
DCORE_USE_NAMESPACE
int main(int argc, char *argv[])
{
    DApplication a(argc, argv);
    a.setAttribute(Qt::AA_UseHighDpiPixmaps); // 高清图
    a.setOrganizationName("deepin");
    a.setApplicationName("deepin-pc-manager");
    a.setApplicationVersion(DApplication::buildVersion((QMAKE_VERSION))); // pro 文件中的版本
    a.loadTranslator();
    a.setApplicationDisplayName(QObject::tr("PC Manager"));
    a.setStyle("chameleon");
    a.setWindowIcon(QIcon::fromTheme("deepin-pc-manager"));
    a.setProductIcon(QIcon::fromTheme("deepin-pc-manager"));
    a.setApplicationDescription(
        QApplication::translate("main",
                                "Deepin PC Manager is an application tool designed to help users "
                                "quickly manage, maintain, and optimize computer systems."));
    a.setQuitOnLastWindowClosed(false);

    MainWindow w;
    Dtk::Widget::moveToCenter(&w);

    // regist dbus service
    DefenderDBusService dbus(&w);

    w.show();
    return a.exec();
}
