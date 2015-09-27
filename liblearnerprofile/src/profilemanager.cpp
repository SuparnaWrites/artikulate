/*
 *  Copyright 2013-2014  Andreas Cord-Landwehr <cordlandwehr@kde.org>
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) version 3, or any
 *  later version accepted by the membership of KDE e.V. (or its
 *  successor approved by the membership of KDE e.V.), which shall
 *  act as a proxy defined in Section 6 of version 3 of the license.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "profilemanager.h"
#include "storage.h"
#include "learner.h"

#include <QObject>
#include <QList>
#include <QDebug>
#include <KConfigCore/KConfig>
#include <KConfigCore/KConfigGroup>
#include <QFileDialog>
#include <KLocalizedString>

using namespace LearnerProfile;

///BEGIN: ProfileManagerPrivate
namespace LearnerProfile {
class ProfileManagerPrivate {

public:
    ProfileManagerPrivate();
    ~ProfileManagerPrivate() {}

    void sync();

    QList<Learner*> m_profiles;
    Learner *m_activeProfile;
    QList<LearningGoal*> m_goals;
    KConfig *m_config;
    Storage m_storage;
};
}

LearnerProfile::ProfileManagerPrivate::ProfileManagerPrivate()
    : m_profiles(QList<Learner*>())
    , m_activeProfile(0)
    , m_config(0)
{
    // load all profiles from storage
    m_goals.append(m_storage.loadGoals());
    m_profiles.append(m_storage.loadProfiles(m_goals));

    // set last used profile
    m_config = new KConfig("learnerprofilerc");
    KConfigGroup activeProfileGroup(m_config, "ActiveProfile");
    int lastProfileId = activeProfileGroup.readEntry("profileId", "0").toInt();
    QList<int> activeGoalsCategories = activeProfileGroup.readEntry("activeGoalsCategories", QList<int>());
    QList<QString> activeGoalsIdentifiers = activeProfileGroup.readEntry("activeGoalsIdentifiers", QList<QString>());
    foreach (Learner *learner, m_profiles) {
        if (learner->identifier() == lastProfileId) {
            m_activeProfile = learner;
            // set active goals
            if (activeGoalsCategories.count() == activeGoalsIdentifiers.count()) {
                for (int i = 0; i < activeGoalsCategories.count(); ++i) {
                    m_activeProfile->setActiveGoal(
                        (Learner::Category) activeGoalsCategories.at(i),
                        activeGoalsIdentifiers.at(i));
                }
            } else {
                qCritical() << "Inconsistent goal category / identifier pairs found: aborting.";
            }
            break;
        }
    }
    if (m_activeProfile == 0) {
        qDebug() << "No last active profile found, falling back to first found profile";
        if (m_profiles.size() > 0) {
            m_activeProfile = m_profiles.at(0);
        }
    }
}

void ProfileManagerPrivate::sync()
{
    // sync last used profile data
    if (m_activeProfile) {
        KConfigGroup activeProfileGroup(m_config, "ActiveProfile");
        activeProfileGroup.writeEntry("profileId", m_activeProfile->identifier());

        // compute activer learning goals by category
        QList<int> goalCatogries;
        QList<QString> goalIdentifiers;
        // compute used goals
        foreach (LearningGoal *goal, m_activeProfile->goals()) {
            if (!goalCatogries.contains((int) goal->category())) {
                goalCatogries.append((int) goal->category());
            }
        }
        // compute active goals
        foreach (int category, goalCatogries) {
            goalIdentifiers.append(m_activeProfile->activeGoal((Learner::Category) category)->identifier());
        }
        activeProfileGroup.writeEntry("activeGoalsCategories", goalCatogries);
        activeProfileGroup.writeEntry("activeGoalsIdentifiers", goalIdentifiers);
    }
    else {
        qCritical() << "No active profile selected, aborting sync.";
    }
    m_config->sync();

    //TODO only sync changed learner
    foreach (Learner *learner, m_profiles) {
        m_storage.storeProfile(learner);
    }
}
///END: ProfileManagerPrivate


ProfileManager::ProfileManager(QObject *parent)
    : QObject(parent)
    , d(new ProfileManagerPrivate)
{
    connect (this, SIGNAL(profileAdded(Learner*,int)), this, SIGNAL(profileCountChanged()));
    connect (this, SIGNAL(profileRemoved()), this, SIGNAL(profileCountChanged()));

    foreach (Learner *learner, d->m_profiles) {
        connect (learner, SIGNAL(goalRemoved(Learner*,LearningGoal*)),
                this, SLOT(removeLearningGoal(Learner*,LearningGoal*)));
    }
}

ProfileManager::~ProfileManager()
{
    foreach (Learner *learner, d->m_profiles) {
        learner->deleteLater();
    }
}

QList< Learner* > ProfileManager::profiles() const
{
    return d->m_profiles;
}

int ProfileManager::profileCount() const
{
    return profiles().length();
}

void ProfileManager::openImageFileDialog()
{
    QString imageUrl = QFileDialog::getOpenFileName(0,
        i18n("Open Image"),
        "",
        i18n("Image Files (*.png *.jpg *.bmp)"));
    d->m_activeProfile->importImage(imageUrl);
}

Learner * ProfileManager::addProfile(const QString &name)
{
    Learner *learner = new Learner(this);
    learner->setName(name);

    // set id
    int maxUsedId = 0;
    foreach (Learner *cpLearner, d->m_profiles) {
        if (cpLearner->identifier() >= maxUsedId) {
            maxUsedId = cpLearner->identifier();
        }
    }
    learner->setIdentifier(maxUsedId + 1);

    d->m_profiles.append(learner);
    d->m_storage.storeProfile(learner);
    emit profileAdded(learner, d->m_profiles.count() - 1);

    if (activeProfile() == 0) {
        setActiveProfile(learner);
    }

    connect (learner, SIGNAL(goalRemoved(Learner*,LearningGoal*)),
            this, SLOT(removeLearningGoal(Learner*,LearningGoal*)));

    return learner;
}

void ProfileManager::removeProfile(Learner *learner)
{
    int index = d->m_profiles.indexOf(learner);
    if (index < 0) {
        qWarning() << "Profile was not found, aborting";
        return;
    }
    emit profileAboutToBeRemoved(index);
    d->m_profiles.removeAt(index);
    d->m_storage.removeProfile(learner);

    if (d->m_activeProfile == learner) {
        if (d->m_profiles.count() == 0) {
            setActiveProfile(0);
        }
        else {
            setActiveProfile(d->m_profiles.at(0));
        }
    }
    emit profileRemoved();
}

void ProfileManager::removeLearningGoal(Learner* learner, LearningGoal* goal)
{
    d->m_storage.removeRelation(learner, goal);
}

Learner * ProfileManager::profile(int index)
{
    if (index < 0 || index >= profiles().count()) {
        return 0;
    }
    return profiles().at(index);
}

QList< LearningGoal* > ProfileManager::goals() const
{
    return d->m_goals;
}

void ProfileManager::registerGoal(LearningGoal::Category category, const QString &identifier, const QString &name)
{
    // test whether goal is already registered
    foreach (LearningGoal *cmpGoal, d->m_goals) {
        if (cmpGoal->category() == category && cmpGoal->identifier() == identifier) {
            return;
        }
    }
    LearningGoal *goal = new LearningGoal(category, identifier, this);
    goal->setName(name);
    d->m_goals.append(goal);
    d->m_storage.storeGoal(goal);
}

void ProfileManager::sync()
{
    d->sync();
}

void ProfileManager::sync(Learner *learner)
{
    d->m_storage.storeProfile(learner);
}

Learner * ProfileManager::activeProfile() const
{
    return d->m_activeProfile;
}

void ProfileManager::setActiveProfile(Learner* learner)
{
    if (learner == d->m_activeProfile) {
        return;
    }
    d->m_activeProfile = learner;
    emit activeProfileChanged();
}
