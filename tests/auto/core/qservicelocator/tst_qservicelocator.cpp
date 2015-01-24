/****************************************************************************
**
** Copyright (C) 2015 Klaralvdalens Datakonsult AB (KDAB).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the Qt3D module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia.  For licensing terms and
** conditions see http://qt.digia.com/licensing.  For further information
** use the contact form at http://qt.digia.com/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Digia gives you certain additional
** rights.  These rights are described in the Digia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include <QtTest/QtTest>
#include <Qt3DCore/qservicelocator.h>
#include <Qt3DCore/qopenglinformationservice.h>
#include <Qt3DCore/qsysteminformationservice.h>

#include <QScopedPointer>

using namespace Qt3D;

class DummyService : public QAbstractServiceProvider
{
public:
    DummyService()
        : QAbstractServiceProvider(QServiceLocator::UserService + 1,
                                   QStringLiteral("Dummy Service"))
    {}

    int data() const { return 10; }
    float moreData() const { return 3.14159f; }
};


class DummySystemInfoService : public QSystemInformationService
{
public:
    DummySystemInfoService()
        : QSystemInformationService(QStringLiteral("Dummy System Information Service"))
    {}

    QStringList aspectNames() const Q_DECL_FINAL { return QStringList(); }
    int threadPoolThreadCount() const Q_DECL_FINAL { return 4; }
};


class tst_QServiceLocator : public QObject
{
    Q_OBJECT

public:
    tst_QServiceLocator() : QObject() {}
    ~tst_QServiceLocator() {}

private slots:
    void construction();
    void defaultServices();
    void addRemoveDefaultService();
    void addRemoveUserService();
};

void tst_QServiceLocator::construction()
{
    QServiceLocator locator;
    QVERIFY(locator.serviceCount() == QServiceLocator::DefaultServiceCount);
}

void tst_QServiceLocator::defaultServices()
{
    QServiceLocator locator;
    QOpenGLInformationService *glInfo = locator.openGLInformation();
    QVERIFY(glInfo != Q_NULLPTR);
    QVERIFY(glInfo->description() == QStringLiteral("Null OpenGL Information Service"));

    QSystemInformationService *sysInfo = locator.systemInformation();
    QVERIFY(sysInfo != Q_NULLPTR);
    QVERIFY(sysInfo->description() == QStringLiteral("Null System Information Service"));
    QVERIFY(sysInfo->threadPoolThreadCount() == 0);
}

void tst_QServiceLocator::addRemoveDefaultService()
{
    // Create locator and register a custom service that replaces a default service
    QScopedPointer<DummySystemInfoService> dummy(new DummySystemInfoService);
    QServiceLocator locator;
    locator.registerServiceProvider(QServiceLocator::SystemInformation, dummy.data());
    QVERIFY(locator.serviceCount() == QServiceLocator::DefaultServiceCount);

    // Get the service from the locator and check it works as expected
    QSystemInformationService *service = locator.systemInformation();
    QVERIFY(service == dummy.data());
    QVERIFY(service->threadPoolThreadCount() == 4);

    // Ensure the other default services work
    QOpenGLInformationService *glInfo = locator.openGLInformation();
    QVERIFY(glInfo != Q_NULLPTR);
    QVERIFY(glInfo->description() == QStringLiteral("Null OpenGL Information Service"));
    QVERIFY(glInfo->format() == QSurfaceFormat());

    // Remove custom service
    locator.unregisterServiceProvider(QServiceLocator::SystemInformation);
    QVERIFY(locator.serviceCount() == QServiceLocator::DefaultServiceCount);

    // Check the dummy service still exists
    QVERIFY(dummy->threadPoolThreadCount() == 4);
}

void tst_QServiceLocator::addRemoveUserService()
{
    // Create locator and register a custom service
    QScopedPointer<DummyService> dummy(new DummyService);
    QServiceLocator locator;
    locator.registerServiceProvider(dummy->type(), dummy.data());
    QVERIFY(locator.serviceCount() == QServiceLocator::DefaultServiceCount + 1);

    // Get the service from the locator and check it works as expected
    DummyService *service = locator.service<DummyService>(dummy->type());
    QVERIFY(service == dummy.data());
    QVERIFY(service->data() == 10);
    QVERIFY(qFuzzyCompare(service->moreData(), 3.14159f));

    // Ensure the default services work
    QSystemInformationService *sysInfo = locator.systemInformation();
    QVERIFY(sysInfo != Q_NULLPTR);
    QVERIFY(sysInfo->description() == QStringLiteral("Null System Information Service"));
    QVERIFY(sysInfo->threadPoolThreadCount() == 0);

    // Remove custom service
    locator.unregisterServiceProvider(dummy->type());
    QVERIFY(locator.serviceCount() == QServiceLocator::DefaultServiceCount);

    // Check the dummy service still exists
    QVERIFY(dummy->data() == 10);
}

QTEST_MAIN(tst_QServiceLocator)

#include "tst_qservicelocator.moc"
