/*
 * Copyright (C) 2019 ~ 2021 Uniontech Software Technology Co.,Ltd.
 *
 * Author:     xuxin <xuxina@uniontech.com>
 *
 * Maintainer: xuxin <xuxina@uniontech.com>
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

#ifndef DEFENDER_DATABASE_OPERATION_BASE_H
#define DEFENDER_DATABASE_OPERATION_BASE_H

#include "../../deepin-pc-manager/src/window/modules/common/common.h"

#include <QObject>
#include <QVariantList>
#include <QSqlQuery>
#include <QSqlDatabase>
#include <QSqlError>

// 非线程安全
// 不要跨线程使用
class DefenderDatabaseOperationBase : public QObject
{
    Q_OBJECT
public:
    explicit DefenderDatabaseOperationBase(QObject *parent = nullptr);
    virtual ~DefenderDatabaseOperationBase();

    // database构造与初始化
    bool init();
    // 执行ID指向的命令
    void executeCmd(int commandID, const QVariantList & = {});
    // 结果输出
    void sendResultSignal(int);

Q_SIGNALS:
    // 派生类自定义信号以返回查询结果
    // 自定义数据结构不要使用MAP，会排序改变输入
    // 信号参数封装成QVariantList以提高泛用性
    // 调用者自行解析数据

protected:
    bool executeQuery(const QString &);
    // 数据库初始化
    virtual bool initDataBase();
    // 提供默认的自动生成连接名，但是也允许派生类重写
    virtual QString connectName();
    // 数据库文件位置
    virtual QString getDBPath();

private:
    // 必须重写
    // 均是子类行为，不要加const以免对子类形成约束
    virtual QString tableName();
    virtual bool createTable();
    virtual void sendResult(int, QSqlQuery &);
    virtual QString formatCmd(int, const QVariantList &);

private:
    QSqlQuery m_query;
    QSqlDatabase m_db;
    bool m_isInited;
    QString m_connName;
};

#endif // DEFENDER_DATABASE_OPERATION_BASE_H
