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

#include "profile.h"
#include <KDebug>

#include "core/language.h"
#include "core/course.h"
#include "core/unit.h"
#include "core/phrase.h"
#include "core/taggroup.h"

Profile::Profile(QObject *parent)
    : QObject(parent)
    , m_language(0)
    , m_course(0)
    , m_unit(0)
    , m_tagGroup(0)
    , m_type(Phrase::Word)
{

}

Language * Profile::language() const
{
    return m_language;
}

void Profile::setLanguage(Language *language)
{
    if (m_language == language) {
        return;
    }
    setCourse(0);
    setUnit(0);
    m_language = language;
    emit languageChanged();
}

Course * Profile::course() const
{
    return m_course;
}

void Profile::setCourse(Course *course)
{
    if (m_course == course) {
        return;
    }
    setUnit(0);
    m_course = course;
    emit courseChanged();
}

Unit * Profile::unit() const
{
    return m_unit;
}

void Profile::setUnit(Unit *unit)
{
    if (m_unit == unit) {
        return;
    }
    m_unit = unit;
    return unitChanged();
}

TagGroup * Profile::tagGroup() const
{
    return m_tagGroup;
}

void Profile::setTagGroup(TagGroup* tagGroup)
{
    if (m_tagGroup == tagGroup) {
        return;
    }
    m_tagGroup = tagGroup;
    emit tagGroupChanged();
}

Phrase::Type Profile::phraseType() const
{
    return m_type;
}

void Profile::setPhraseType(Phrase::Type type)
{
    if (m_type == type) {
        return;
    }
    m_type = type;
    emit phraseTypeChanged(type);
}