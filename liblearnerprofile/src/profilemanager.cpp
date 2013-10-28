/*
 *  Copyright 2013  Andreas Cord-Landwehr <cordlandwehr@kde.org>
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

#include "profilemanager_p.h"
#include "profilemanager.h"
#include "learner.h"

#include <QObject>
#include <QList>
#include <KDebug>

using namespace LearnerProfile;

ProfileManager::ProfileManager(QObject* parent)
    : QObject(parent)
    , d(new ProfileManagerPrivate)
{

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

    return learner;
}

void ProfileManager::sync()
{
    d->sync();
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
