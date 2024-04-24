// SPDX-FileCopyrightText: 2024 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <DCheckBox>
#include <DFrame>
#include <DTipLabel>
#include <DSpinner>

DWIDGET_USE_NAMESPACE
class TrashCleanResultItemWidget : public DFrame
{
    Q_OBJECT
public:
    explicit TrashCleanResultItemWidget(QWidget *parent = nullptr);
    ~TrashCleanResultItemWidget();

    // 设置标题和提示
    void setHeadText(const QString &text);
    void setHeadTipText(const QString &text);

    // 设置右侧提示信息与使用空间大小
    void setTailText(const QString &text);
    void setUsedSpace(const QString &text);

    // 设置/获取 勾选框状态
    void setCheckBoxStatus(bool isChecked);
    bool getCheckBoxStatus() const;
    void setCheckBoxVisable(bool isVisable);

    // 子项目扫描完成时显示小图标
    void setIconVisable(bool isVisable);

    void setScanning();
    void setCleanDone();

    void setAsRoot();
    void setAsChild();

    void rootSelected();
    void resetElidedTextByFontChange();

    bool getCheckBoxEnable();
    void setUncheckable();

public Q_SLOTS:
    void setScanResult(const QString &);
    void checkBoxSelected();

Q_SIGNALS:
    void sendBoxClicked(bool);

protected:
    void paintEvent(QPaintEvent *event);

private:
    bool m_isRootItem; // 作为根结点展示
    DCheckBox *m_checkBox; // 作为子项是否选中
    DLabel *m_iconLable; // 扫描结束使用小图标
    DLabel *m_headLabel; // 扫描项目名称
    DTipLabel *m_headTipLabel; // 项目说明
    DTipLabel *m_tailLabel; // 状态标签
    DLabel *m_usageLabel; // 大小标签
    DFrame *m_contentFrame;
    TrashCleanResultItemWidget *m_root; // 根检查项的页面
    bool m_isWorking;

    // 由于需要对界面文字遮挡问题做出处理
    // 现在需要保存被设置文字的原始值
    QString m_headStr;
    QString m_headTipStr;
    QString m_tailStr;
    QString m_usageStr;
};
