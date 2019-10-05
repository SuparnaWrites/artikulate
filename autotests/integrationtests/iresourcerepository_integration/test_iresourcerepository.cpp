/*
 *  Copyright 2019  Andreas Cord-Landwehr <cordlandwehr@kde.org>
 *
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License as
 *  published by the Free Software Foundation; either version 2 of
 *  the License or (at your option) version 3 or any later version
 *  accepted by the membership of KDE e.V. (or its successor approved
 *  by the membership of KDE e.V.), which shall act as a proxy
 *  defined in Section 14 of version 3 of the license.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "test_iresourcerepository.h"
#include <QTest>
#include <QSignalSpy>
#include <QObject>

#include "core/resourcerepository.h"
#include "core/contributorrepository.h"
#include "core/language.h"
#include "core/icourse.h"
#include "core/unit.h"
#include "../src/settings.h"

TestIResourceRepository::TestIResourceRepository()
    : m_repositoryLocation(QUrl::fromLocalFile(qApp->applicationDirPath() + "/../autotests/integrationtests/data/"))
{
    qDebug() << "Running test with repository root:" << m_repositoryLocation;
}

void TestIResourceRepository::init()
{
    // check that test data is deployed at the expected location
    QVERIFY(QFile::exists(m_repositoryLocation.toLocalFile() + "/courses/de/de.xml"));
    QVERIFY(QFile::exists(m_repositoryLocation.toLocalFile() + "/courses/fr/fr.xml"));
}

void TestIResourceRepository::resourceRepository()
{
    ResourceRepository repository(QUrl::fromLocalFile(m_repositoryLocation.toLocalFile() + "/courses/"));
    QCOMPARE(repository.storageLocation(), QUrl::fromLocalFile(m_repositoryLocation.toLocalFile() + "/courses/"));
    performInterfaceTests(&repository);
}

void TestIResourceRepository::contributorRepository()
{
    ContributorRepository repository(QUrl::fromLocalFile(m_repositoryLocation.toLocalFile() + "/contributorrepository/")); // contributor repository requires subdirectory "courses"
    QCOMPARE(repository.storageLocation(), QUrl::fromLocalFile(m_repositoryLocation.toLocalFile() + "/contributorrepository/"));
    performInterfaceTests(&repository);
}

void TestIResourceRepository::performInterfaceTests(IResourceRepository *interface)
{
    QVERIFY(interface->languages().count() > 0); // automatically load languages
    QCOMPARE(interface->courses().count(), 0); // load courses only on demand

    // test adding
    QSignalSpy spyAboutToBeAdded(dynamic_cast<QObject *>(interface), SIGNAL(courseAboutToBeAdded(std::shared_ptr<ICourse>, int)));
    QSignalSpy spyAdded(dynamic_cast<QObject *>(interface), SIGNAL(courseAdded()));
    QCOMPARE(spyAboutToBeAdded.count(), 0);
    QCOMPARE(spyAdded.count(), 0);
    interface->reloadCourses(); // initial loading of courses
    QCOMPARE(interface->courses().count(), 2);
    QCOMPARE(spyAboutToBeAdded.count(), 2);
    QCOMPARE(spyAdded.count(), 2);

    // test reloading of courses
    interface->reloadCourses(); // initial loading of courses
    QCOMPARE(interface->courses().count(), 2);

    // test removal
    // note: repository does not provide removal of courses, yet

    // test access of courses grouped by language
    auto languages = interface->languages();
    std::shared_ptr<ILanguage> german;
    for (auto language : interface->languages()) {
        if (language->id() == "de") {
            german = language;
            break;
        }
    }
    QVERIFY(german != nullptr); // ensure that German language was found
    QCOMPARE(interface->courses(german->id()).count(), 1); // there is exactly one German course
    QCOMPARE(interface->courses(nullptr).count(), 2); // all courses in total are 2
    QVERIFY(interface->courses().first()->units().size() > 0);
}

QTEST_GUILESS_MAIN(TestIResourceRepository)
