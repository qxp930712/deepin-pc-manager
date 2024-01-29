/*
 * Copyright (C) 2020 ~ 2021 Deepin Technology Co., Ltd.
 *
 * Author:     caimengci <caimengci@uniontech.com>
 *
 * Maintainer: caimengci <caimengci@uniontech.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "restartdefenderdialog.h"
#include "window/modules/common/invokers/invokerfactory.h"

#include <DLabel>

#define RESTART_CANCEL 0 // 弹框第一个按钮
#define RESTART_OK 1 // 弹框第二个按钮

RestartDefenderDialog::RestartDefenderDialog(DDialog *parent)
    : DDialog(parent)
    , m_daemonservice(nullptr)
{
    // 安全中心心跳服务
    m_daemonservice = InvokerFactory::GetInstance().CreateInvoker("com.deepin.pc.manager.daemonservice",
                                                                  "/com/deepin/pc/manager/daemonservice",
                                                                  "com.deepin.pc.manager.daemonservice",
                                                                  ConnectType::SESSION, this);
    // 初始化界面
    initUi();
}

// 初始化界面
void RestartDefenderDialog::initUi()
{
    setMinimumSize(QSize(200, 100));
    // 弹框样式
    setCloseButtonVisible(true);
    this->setWindowFlag(Qt::WindowStaysOnTopHint);
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    this->setModal(true);

    // 清除布局
    clearContents();
    clearButtons();
    setIcon(QIcon::fromTheme("deepin-defender"));

    // 添加弹框文字
    DLabel *txt = nullptr;
    txt = new DLabel(tr("Changes will take effect after the application restarts"), this);
    // 字体剧中
    txt->setAlignment(Qt::AlignCenter);
    txt->setWordWrap(true);

    // 添加布局
    addContent(txt);
    addButton(tr("Cancel", "button"));
    addButton(tr("Restart", "button"));

    // 按钮点击信号槽
    connect(this, &RestartDefenderDialog::buttonClicked, this, &RestartDefenderDialog::doButtonClicked);
}

RestartDefenderDialog::~RestartDefenderDialog()
{
    Q_EMIT onDialogClose();
}

// 按钮点击槽
void RestartDefenderDialog::doButtonClicked(int index, const QString &text)
{
    Q_UNUSED(text);
    if (index == RESTART_OK) {
        Q_EMIT onRestartClose();
    }
}
