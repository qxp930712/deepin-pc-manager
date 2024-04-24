// Copyright (C) 2019 ~ 2019 Deepin Technology Co., Ltd.
// SPDX-FileCopyrightText: 2024 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "filenamewidget.h"
#include "filenamemodel.h"

#include <DFloatingMessage>
#include <DMessageManager>
#include <DFontSizeManager>
#include <DTipLabel>
#include <DDialog>

#include <QVBoxLayout>

FileNameWidget::FileNameWidget(FileNameModel *model, QWidget *parent)
    : DFrame(parent)
    , m_model(model)
    , m_textEdit(nullptr)
    , m_execButton(nullptr)
{
    initUI();
    connect(m_execButton, &DSuggestButton::clicked, this, &FileNameWidget::supportLoogFileName);
}

FileNameWidget::~FileNameWidget()
{
}

void FileNameWidget::initUI()
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setSpacing(10);
    layout->setContentsMargins(20, 20, 20, 20);
    this->setLayout(layout);

    QLabel *title = new QLabel(tr("Long filename support"), this);
    QFont font1 = title->font();
    font1.setBold(true);
    font1.setWeight(QFont::DemiBold);
    DFontSizeManager::instance()->bind(title, DFontSizeManager::T5);
    title->setFont(font1);
    title->setContentsMargins(10, 0, 0, 0);

    QLabel *detailInfo = new QLabel(tr("Extend file name length to 225 chinese characters"), this);
    DFontSizeManager::instance()->bind(detailInfo, DFontSizeManager::T8);
    detailInfo->setContentsMargins(10, 0, 0, 0);

    QLabel *descriptionInfo = new QLabel(tr("Tool Description"), this);
    DFontSizeManager::instance()->bind(descriptionInfo, DFontSizeManager::T8);
    descriptionInfo->setContentsMargins(10, 0, 0, 0);

    DTipLabel *tipLabel = new DTipLabel(tr("Extend the maximum filename length to 255 Chinese characters, only supported on UOS1060 and above systems."), this);
    DFontSizeManager::instance()->bind(tipLabel, DFontSizeManager::T8);
    tipLabel->setContentsMargins(10, 0, 0, 0);
    tipLabel->setAlignment(Qt::AlignLeft);

    DFrame *textFrame = new DFrame(this);
    QVBoxLayout *textLayout = new QVBoxLayout(textFrame);
    textLayout->setContentsMargins(2, 2, 2, 2);
    textLayout->setSpacing(0);
    textFrame->setLayout(textLayout);

    m_textEdit = new QTextEdit(textFrame);
    m_textEdit->setFixedSize(QSize(900, 400));
    m_textEdit->setLineWidth(0);
    textLayout->addWidget(m_textEdit, 0, Qt::AlignCenter);

    m_execButton = new DSuggestButton(this);
    m_execButton->setFixedWidth(180);
    m_execButton->setText(tr("Execute immediately"));

    layout->addWidget(title);
    layout->addWidget(detailInfo);
    layout->addSpacing(10);
    layout->addWidget(descriptionInfo);
    layout->addWidget(tipLabel);
    layout->addWidget(textFrame, 0, Qt::AlignCenter);
    layout->addWidget(m_execButton, 0, Qt::AlignRight);
    layout->addStretch();
}

void FileNameWidget::supportLoogFileName()
{
    DDialog *dlg = new DDialog(this);
    dlg->setOnButtonClickedClose(true);
    // 设置属性当关闭时候删除自己
    dlg->setAttribute(Qt::WA_DeleteOnClose);
    dlg->setIcon(QIcon::fromTheme("deepin-file-name"));
    dlg->setMessage(tr("The operation cannot be undone. Please confirm if you want to proceed?"));
    dlg->addButton(tr("Cancel", "button"), false, DDialog::ButtonType::ButtonNormal);
    dlg->addButton(tr("Continue"), false, DDialog::ButtonType::ButtonRecommend);

    connect(dlg, &DDialog::buttonClicked, this, [&](int index, const QString &text) {
        Q_UNUSED(text);
        if (1 == index) {
            QString result = m_model->startLoogFileNameSupport();
            m_textEdit->setText(result);

            if (result.contains("true")) {
                DFloatingMessage *floMsgSwitchOff = new DFloatingMessage(DFloatingMessage::TransientType);
                floMsgSwitchOff->setDuration(2000);
                floMsgSwitchOff->setIcon(QIcon::fromTheme("dcc_ok_tip"));
                floMsgSwitchOff->setMessage(tr("Long filename support enabled, effective after restarting the computer"));
                DMessageManager::instance()->sendMessage(this, floMsgSwitchOff);
                DMessageManager::instance()->setContentMargens(this, QMargins(0, 0, 0, 20));
            }
        }
    });
    dlg->exec();
}
