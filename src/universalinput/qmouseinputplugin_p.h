// Copyright (C) 2024 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QMOUSEINPUTPLUGIN_P_H
#define QMOUSEINPUTPLUGIN_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include <QtUniversalInput/private/qtuniversalinputglobal_p.h>

#include <QtCore/QObject>

#include <QtCore/qplugin.h>
#include <QtCore/qfactoryinterface.h>

QT_BEGIN_NAMESPACE

class QMouseInput;

#define QMouseInputFactoryInterface_iid "org.qt-project.QtUniversalInput.MouseInput.6.5"

class Q_UNIVERSALINPUT_EXPORT QMouseInputPlugin : public QObject
{
    Q_OBJECT
public:
    virtual QMouseInput *create(const QString &key, const QStringList &paramList) = 0;
};

QT_END_NAMESPACE

#endif // QMOUSEINPUTPLUGIN_P_H
