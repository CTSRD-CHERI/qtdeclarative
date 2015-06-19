/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the QtQml module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL21$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see http://www.qt.io/terms-conditions. For further
** information use the contact form at http://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 or version 3 as published by the Free
** Software Foundation and appearing in the file LICENSE.LGPLv21 and
** LICENSE.LGPLv3 included in the packaging of this file. Please review the
** following information to ensure the GNU Lesser General Public License
** requirements will be met: https://www.gnu.org/licenses/lgpl.html and
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** As a special exception, The Qt Company gives you certain additional
** rights. These rights are described in The Qt Company LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include <QtQml/qqmlextensionplugin.h>
#include <QtQml/qqml.h>

#include "qquickfolderlistmodel.h"

static void initResources()
{
#ifdef QT_STATIC
    Q_INIT_RESOURCE(qmake_Qt_labs_folderlistmodel);
#endif
}

QT_BEGIN_NAMESPACE

//![class decl]
class QmlFolderListModelPlugin : public QQmlExtensionPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QQmlExtensionInterface")

public:
    QmlFolderListModelPlugin(QObject *parent = 0) : QQmlExtensionPlugin(parent) { initResources(); }
    virtual void registerTypes(const char *uri)
    {
        Q_ASSERT(QLatin1String(uri) == QLatin1String("Qt.labs.folderlistmodel"));
#ifndef QT_NO_DIRMODEL
        qmlRegisterType<QQuickFolderListModel>(uri,1,0,"FolderListModel");
        qmlRegisterType<QQuickFolderListModel>(uri,2,0,"FolderListModel");
        qmlRegisterType<QQuickFolderListModel,1>(uri,2,1,"FolderListModel");
        qmlRegisterType<QQuickFolderListModel,2>(uri,2,2,"FolderListModel");
#endif
    }
};
//![class decl]

QT_END_NAMESPACE

#include "plugin.moc"
