/*
 *  Copyright 2013  Andreas Cord-Landwehr <cordlandwehr@gmail.com>
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

#include "course.h"
#include "unit.h"
#include "language.h"
#include "resourcemanager.h"
#include "taggroup.h"

#include <KDebug>
#include <KLocale>
#include <QStringList>
#include <QPair>
#include <QUuid>

Course::Course(QObject *parent)
    : QObject(parent)
    , m_language(0)
    , m_modified(false)
{
}

Course::~Course()
{
    foreach (Unit *unit, m_unitList) {
        unit->deleteLater();
    }
}

QString Course::id() const
{
    return m_id;
}

void Course::setId(const QString &id)
{
    if (id != m_id) {
        m_id = id;
        emit idChanged();
        setModified();
    }
}

QString Course::title() const
{
    return m_title;
}

void Course::setTitle(const QString &title)
{
    if (QString::compare(title, m_title) != 0) {
        m_title = title;
        emit titleChanged();
        setModified();
    }
}

QString Course::description() const
{
    return m_description;
}

void Course::setDescription(const QString &description)
{
    m_description = description;
    emit descriptionChanged();
}

Language * Course::language() const
{
    return m_language;
}

void Course::setLanguage(Language *language)
{
    m_language = language;
}

KUrl Course::file() const
{
    return m_file;
}

void Course::setFile(const KUrl &file)
{
    m_file = file;
}

QList< Unit* > Course::unitList() const
{
    return m_unitList;
}

void Course::addUnit(Unit *unit)
{
    QList<Unit*>::ConstIterator iter = m_unitList.constBegin();
    while (iter != m_unitList.constEnd()) {
        if (unit->id() == (*iter)->id()) {
            kWarning() << "Unit already contained in this course, aborting";
            return;
        }
        ++iter;
    }
    emit unitAboutToBeAdded(unit, m_unitList.length());
    m_unitList.append(unit);
    connect(unit, SIGNAL(modified()), this, SLOT(setModified()));
    emit unitAdded();
    setModified();
}

Unit * Course::createUnit()
{
    // find first unused id
    QStringList unitIds;
    foreach (Unit *unit, m_unitList) {
        unitIds.append(unit->id());
    }
    QString id = QUuid::createUuid().toString();
    while (unitIds.contains(id)) {
        id = QUuid::createUuid();
        kWarning() << "Unit id generator has found a collision, recreating id.";
    }

    // create unit
    Unit *unit = new Unit(this);
    unit->setCourse(this);
    unit->setId(id);
    unit->setTitle(i18n("New Unit"));
    addUnit(unit);
    return unit;
}

Phrase * Course::createPhrase(Unit *unit)
{
    // find globally unique phrase id inside course
    QStringList phraseIds;
    foreach (Unit *unit, m_unitList) {
        foreach (Phrase *phrase, unit->phraseList(Phrase::AllTypes)) {
            phraseIds.append(phrase->id());
        }
    }
    QString id = QUuid::createUuid().toString();
    while (phraseIds.contains(id)) {
        id = QUuid::createUuid();
        kWarning() << "Phrase id generator has found a collision, recreating id.";
    }

    // create unit
    Phrase *phrase = new Phrase(this);
    phrase->setId(id);
    phrase->setText("");
    phrase->setType(Phrase::Word);
    unit->addPhrase(phrase);

    return phrase;
}

QList< Unit* > Course::syllableUnitList() const
{
    QList<Unit*> list;
    QList< QPair<TagGroup*,Unit*> >::ConstIterator iter = m_syllableUnitList.constBegin();
    while (iter != m_syllableUnitList.end()) {
        list.append(iter->second);
        ++iter;
    }
    return list;
}

Unit * Course::syllableUnit(TagGroup *tagGroup) const
{
    QList< QPair<TagGroup*,Unit*> >::ConstIterator iter = m_syllableUnitList.constBegin();
    while (iter != m_syllableUnitList.end()) {
        if (iter->first == tagGroup) {
            return iter->second;
        }
        ++iter;
    }
    return 0;
}

TagGroup * Course::tagGroup(Unit *unit) const
{
    QList< QPair<TagGroup*,Unit*> >::ConstIterator iter = m_syllableUnitList.constBegin();
    while (iter != m_syllableUnitList.end()) {
        if (iter->second == unit) {
            return iter->first;
        }
        ++iter;
    }
    return 0;
}

void Course::addTagGroup(TagGroup *tagGroup)
{
    QList< QPair<TagGroup*,Unit*> >::ConstIterator iter = m_syllableUnitList.constBegin();
    while (iter != m_syllableUnitList.end()) {
        if (iter->first == tagGroup) {
            kWarning() << "Tag group already contained in this course, aborting";
            return;
        }
        ++iter;
    }
    emit tagGroupAboutToBeAdded(tagGroup, m_syllableUnitList.length());
    m_syllableUnitList.append(qMakePair<TagGroup*,Unit*>(tagGroup, new Unit(this)));
    connect(tagGroup, SIGNAL(modified()), this, SLOT(setModified()));
    emit tagGroupAdded();
    setModified();
}

QList< TagGroup* > Course::tagGroupList() const
{
    QList<TagGroup*> list;
    QList< QPair<TagGroup*,Unit*> >::ConstIterator iter = m_syllableUnitList.constBegin();
    while (iter != m_syllableUnitList.end()) {
        list.append(iter->first);
        ++iter;
    }
    return list;
}

bool Course::modified() const
{
    return m_modified;
}

void Course::setModified(bool modified)
{
    if (m_modified == modified) {
        return;
    }
    m_modified = modified;
    emit modifiedChanged();
}

void Course::sync()
{
    if (!m_file.isValid() || m_file.isEmpty()) {
        kWarning() << "No file path set, aborting sync operation.";
        return;
    }

    // call sync operation
    ResourceManager::sync(this);
    setModified(false);
}
