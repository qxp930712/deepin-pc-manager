/*
 * Copyright (C) 2019 ~ 2021 UnionTech Software Technology Co., Ltd.
 *
 * Author:     caitao <caitao@uniontech.com>
 *
 * Maintainer: caitao <caitao@uniontech.com>
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
#include "searchlineedit.h"
#include "animatedlineedit.h"
#include "common/compixmap.h"

#include <DGuiApplicationHelper>

#include <QIcon>
#include <QPaintEvent>
#include <QPainter>
#include <QTimer>
#include <QVariantAnimation>
#include <QDebug>
#include <QLabel>
#include <QVBoxLayout>
#include <QAction>

DGUI_USE_NAMESPACE

#define TOP_LABEL_ICON_SIZE QSize(30, 30) // 顶层标签图标尺寸

SearchLineEdit::SearchLineEdit(QWidget *parent)
    : DFrame(parent)
    , m_topWidget(nullptr)
    , m_searchIconLable(nullptr)
    , m_topTextLable(nullptr)
    , m_lineEdit(nullptr)
    , m_searchIconAction(nullptr)
{
    initUi();
    initConnection();
}

SearchLineEdit::~SearchLineEdit()
{
}

void SearchLineEdit::setParentAccessibleName(const QString &name)
{
    m_topWidget->setAccessibleName(name + "_topWidget");
    m_searchIconLable->setAccessibleName(name + "_topWidget_searchIconLable");
    m_topTextLable->setAccessibleName(name + "_topWidget_topTextLable");
    m_lineEdit->setAccessibleName(name + "_lineEdit");
}

void SearchLineEdit::resizeEvent(QResizeEvent *event)
{
    m_lineEdit->resize(size());

    DFrame::resizeEvent(event);
}

bool SearchLineEdit::eventFilter(QObject *obj, QEvent *event)
{
    if (m_topWidget == obj && QEvent::Type::MouseButtonPress == event->type()) {
        // 当点击顶层控件后，隐藏顶层，显示编辑框
        m_topWidget->hide();
        m_lineEdit->setFocus(Qt::FocusReason::MouseFocusReason);
        m_lineEdit->addAction(m_searchIconAction, AnimatedLineEdit::ActionPosition::LeadingPosition);
        m_lineEdit->startAnimation();
    } else if (m_lineEdit == obj && QEvent::Type::FocusOut == event->type()) {
        if (m_lineEdit->text().isEmpty()) {
            // 当失去焦点并且编辑框内容为空时，显示顶层
            m_topWidget->show();
            m_lineEdit->removeAction(m_searchIconAction);
            m_lineEdit->clearFocus();
        }
    }

    return QWidget::eventFilter(obj, event);
}

void SearchLineEdit::updateIcon()
{
    m_searchIconLable->setPixmap(QIcon::fromTheme(ICON_SEARCH).pixmap(TOP_LABEL_ICON_SIZE));
    m_searchIconAction->setIcon(QIcon::fromTheme(ICON_SEARCH));
}

void SearchLineEdit::initUi()
{
    setFocusPolicy(Qt::FocusPolicy::ClickFocus);
    setMinimumHeight(36);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);
    setLayout(mainLayout);

    m_topWidget = new QWidget(this);
    m_topWidget->installEventFilter(this);
    m_topWidget->setContentsMargins(0, 0, 0, 0);

    QHBoxLayout *topLayout = new QHBoxLayout(m_topWidget);
    topLayout->setContentsMargins(0, 0, 0, 0);
    topLayout->setSpacing(0);
    m_topWidget->setLayout(topLayout);
    mainLayout->addWidget(m_topWidget);

    m_searchIconLable = new QLabel(this);
    m_searchIconLable->setContentsMargins(0, 0, 0, 0);
    m_searchIconLable->setPixmap(QIcon::fromTheme(ICON_SEARCH).pixmap(TOP_LABEL_ICON_SIZE));
    topLayout->addWidget(m_searchIconLable, 0, Qt::AlignRight);

    topLayout->addSpacing(5);
    m_topTextLable = new QLabel(this);
    m_topTextLable->setContentsMargins(0, 0, 0, 0);
    m_topTextLable->setText(tr("Search"));
    topLayout->addWidget(m_topTextLable, 0, Qt::AlignLeft);
    topLayout->addSpacing(20);

    m_lineEdit = new AnimatedLineEdit(this);
    m_lineEdit->setClearButtonEnabled(true);
    m_lineEdit->installEventFilter(this);
    m_searchIconAction = new QAction(this);
    m_searchIconAction->setIcon(QIcon::fromTheme(ICON_SEARCH));
    m_lineEdit->removeAction(m_searchIconAction);

    m_lineEdit->lower();
}

void SearchLineEdit::initConnection()
{
    connect(DGuiApplicationHelper::instance(), &DGuiApplicationHelper::themeTypeChanged, this, [this](DGuiApplicationHelper::ColorType themeType) {
        Q_UNUSED(themeType);
        this->updateIcon();
    });
    connect(m_lineEdit, &AnimatedLineEdit::textChanged, this, &SearchLineEdit::textChanged);
}