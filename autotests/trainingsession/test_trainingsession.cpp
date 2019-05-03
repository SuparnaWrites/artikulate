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

#include "test_trainingsession.h"
#include "src/core/trainingsession.h"
#include "src/core/icourse.h"
#include "src/core/language.h"
#include "src/core/unit.h"
#include "src/core/trainingaction.h"
#include "liblearnerprofile/src/profilemanager.h"
#include <QTest>
#include <QSignalSpy>

// assumption: during a training session the units and phrases of a course do not change
//   any change of such a course shall result in a reload of a training session

class CourseStub : public ICourse
{
public:
    CourseStub(Language *language, QVector<Unit *> units)
        : m_language(language)
        , m_units(units)
    {
    }
    ~CourseStub() override;

    QString id() const override
    {
        return "courseid";
    }
    QString title() const override
    {
        return "title";
    }
    QString i18nTitle() const override
    {
        return "i18n title";
    }
    QString description() const override
    {
        return "description of the course";
    }
    Language * language() const override
    {
        return m_language;
    }
    QList<Unit *> unitList() override
    {
        return m_units.toList();
    }
    QUrl file() const override
    {
        return QUrl();
    }

private:
    Language *m_language{nullptr};
    QVector<Unit *> m_units;
};

// define one virtual method out of line to pin CourseStub to this translation unit
CourseStub::~CourseStub() = default;

void TestTrainingSession::init()
{
    // TODO initialization of test case
}

void TestTrainingSession::cleanup()
{
    // TODO cleanup after test run
}

void TestTrainingSession::createTrainingSessionWithoutUnits()
{
    Language language;
    CourseStub course(&language, QVector<Unit *>());
    LearnerProfile::ProfileManager manager;
    TrainingSession session(&manager);
    session.setCourse(&course);
    QVERIFY(&course == session.course());
}

void TestTrainingSession::createTrainingSessionWithEmptySounds()
{
    Language language;
    Unit unitA;
    Unit unitB;
    Phrase phraseA1;
    Phrase phraseA2;
    Phrase phraseB1;
    Phrase phraseB2;
    // note: phrases without soundfiles are skipped in session generation
    phraseA1.setId("A1");
    phraseA2.setId("A2");
    phraseB1.setId("B1");
    phraseB2.setId("B2");
    phraseA1.setSound(QUrl::fromLocalFile("/tmp/a1.ogg"));
    unitA.addPhrase(&phraseA1);
    unitA.addPhrase(&phraseA2);
    unitB.addPhrase(&phraseB1);
    unitB.addPhrase(&phraseB2);

    CourseStub course(&language, QVector<Unit *>({&unitA, &unitB}));
    LearnerProfile::ProfileManager manager;
    TrainingSession session(&manager);
    session.setCourse(&course);

    // test number of actions
    auto actions = session.trainingActions();
    QCOMPARE(actions.count(), 1);
    QCOMPARE(actions.at(0)->actions().count(), 1);
}

void TestTrainingSession::createTrainingSessionWithEmptyUnits()
{
    Language language;
    Unit firstUnit;
    Unit secondUnit;

    CourseStub course(&language, QVector<Unit *>({&firstUnit, &secondUnit}));
    LearnerProfile::ProfileManager manager;
    TrainingSession session(&manager);
    session.setCourse(&course);
    QVERIFY(&course == session.course());
}

void TestTrainingSession::createTrainingSessionWithUnitsAndPhrases()
{
    Language language;
    Unit unit;
    Phrase firstPhrase;
    Phrase secondPhrase;
    unit.addPhrase(&firstPhrase);
    unit.addPhrase(&secondPhrase);

    CourseStub course(&language, QVector<Unit *>({&unit}));
    LearnerProfile::ProfileManager manager;
    TrainingSession session(&manager);
    session.setCourse(&course);
    QVERIFY(&course == session.course());
}

void TestTrainingSession::iterateCourse()
{
    Language language;
    Unit unitA;
    Unit unitB;
    Phrase phraseA1;
    Phrase phraseA2;
    Phrase phraseB1;
    Phrase phraseB2;
    // note: phrases without soundfiles are skipped in session generation
    phraseA1.setId("A1");
    phraseA2.setId("A2");
    phraseB1.setId("B1");
    phraseB2.setId("B2");
    phraseA1.setSound(QUrl::fromLocalFile("/tmp/a1.ogg"));
    phraseA2.setSound(QUrl::fromLocalFile("/tmp/a1.ogg"));
    phraseB1.setSound(QUrl::fromLocalFile("/tmp/b1.ogg"));
    phraseB2.setSound(QUrl::fromLocalFile("/tmp/b2.ogg"));
    unitA.addPhrase(&phraseA1);
    unitA.addPhrase(&phraseA2);
    unitB.addPhrase(&phraseB1);
    unitB.addPhrase(&phraseB2);

    CourseStub course(&language, QVector<Unit *>({&unitA, &unitB}));
    LearnerProfile::ProfileManager manager;
    TrainingSession session(&manager);
    session.setCourse(&course);

    // session assumed to initialize with first units's first phrase
    QCOMPARE(session.activeUnit(), &unitA);
    QCOMPARE(session.activePhrase(), &phraseA1);
    QVERIFY(&course == session.course());

    // test direct unit setters
    session.setUnit(&unitA);
    QCOMPARE(session.activeUnit(), &unitA);
    session.setUnit(&unitB);
    QCOMPARE(session.activeUnit(), &unitB);

    // test direct phrase setters
    session.setPhrase(&phraseA1);
    QCOMPARE(session.activePhrase(), &phraseA1);
    QCOMPARE(session.activeUnit(), &unitA);
    session.setPhrase(&phraseB1);
    QCOMPARE(session.activePhrase(), &phraseB1);
    QCOMPARE(session.activeUnit(), &unitB);

    // test number of actions
    auto actions = session.trainingActions();
    QCOMPARE(actions.count(), 2);
    QCOMPARE(actions.at(0)->actions().count(), 2);
    QCOMPARE(actions.at(1)->actions().count(), 2);

    // test phrase iterators: accept iterator
    session.setPhrase(&phraseA1);
    QCOMPARE(session.activeUnit(), &unitA);
    QCOMPARE(session.activePhrase(), &phraseA1);
    QVERIFY(session.hasNext());
    session.accept();
    QCOMPARE(session.activeUnit(), &unitA);
    QCOMPARE(session.activePhrase(), &phraseA2);
    session.accept();
    QCOMPARE(session.activePhrase(), &phraseB1);
    session.accept();
    QCOMPARE(session.activePhrase(), &phraseB2);
    QVERIFY(!session.hasNext());

    // test phrase iterators: skip iterator
    session.setPhrase(&phraseA1);
    QCOMPARE(session.activeUnit(), &unitA);
    QCOMPARE(session.activePhrase(), &phraseA1);
    QVERIFY(!session.hasPrevious());
    QVERIFY(session.hasNext());
    session.skip();
    QCOMPARE(session.activeUnit(), &unitA);
    QCOMPARE(session.activePhrase(), &phraseA2);
    session.skip();
    QCOMPARE(session.activePhrase(), &phraseB1);
    session.skip();
    QCOMPARE(session.activePhrase(), &phraseB2);
    QVERIFY(session.hasPrevious());
    QVERIFY(!session.hasNext());

    // test completed signal
    QSignalSpy spy(&session, SIGNAL(completed()));
    session.setPhrase(&phraseB1);
    session.accept();
    QCOMPARE(spy.count(), 0);
    session.accept();
    QCOMPARE(spy.count(), 1);
}

QTEST_GUILESS_MAIN(TestTrainingSession)
