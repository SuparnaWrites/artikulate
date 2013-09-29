/*
 *  Copyright 2013  Andreas Cord-Landwehr <cordlandwehr@kde.org>
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

#include "phonemegroupmodel.h"
#include "core/course.h"
#include "core/unit.h"
#include "core/phonemegroup.h"

#include <QAbstractListModel>
#include <QSignalMapper>

#include <KLocale>
#include <KDebug>

PhonemeGroupModel::PhonemeGroupModel(QObject *parent)
    : QAbstractListModel(parent)
    , m_course(0)
    , m_signalMapper(new QSignalMapper(this))
{
    QHash<int, QByteArray> roles;
    roles[TitleRole] = "title";
    roles[IdRole] = "id";
    roles[DataRole] = "dataRole";
    setRoleNames(roles);

    connect(m_signalMapper, SIGNAL(mapped(int)), SLOT(emitPhonemeGroupChanged(int)));
}

void PhonemeGroupModel::setCourse(Course *course)
{
    if (m_course == course) {
        return;
    }

    beginResetModel();

    if (m_course) {
        m_course->disconnect(this);
    }

    m_course = course;

    if (m_course) {
        connect(m_course, SIGNAL(phonemeGroupAboutToBeAdded(PhonemeGroup*,int)), SLOT(onPhonemeGroupAboutToBeAdded(PhonemeGroup*,int)));
        connect(m_course, SIGNAL(phonemeGroupAdded()), SLOT(onPhonemeGroupAdded()));
        connect(m_course, SIGNAL(phonemeGroupAboutToBeRemoved(int,int)), SLOT(onPhonemeGroupsAboutToBeRemoved(int,int)));
        connect(m_course, SIGNAL(phonemeGroupRemoved()), SLOT(onPhonemeGroupsRemoved()));
    }

    endResetModel();

    emit courseChanged();
}

Course * PhonemeGroupModel::course() const
{
    return m_course;
}

QVariant PhonemeGroupModel::data(const QModelIndex& index, int role) const
{
    Q_ASSERT(m_course);

    if (!index.isValid()) {
        return QVariant();
    }

    if (index.row() >= m_course->phonemeGroupList().count()) {
        return QVariant();
    }

    PhonemeGroup * const phonemeGroup = m_course->phonemeGroupList().at(index.row());

    switch(role)
    {
    case Qt::DisplayRole:
        return !phonemeGroup->title().isEmpty()?
                QVariant(phonemeGroup->title()): QVariant(i18nc("@item:inlistbox:", "<no title>"));
    case Qt::ToolTipRole:
        return QVariant(i18n("<p>%1</p>", phonemeGroup->title()));
    case TitleRole:
        return phonemeGroup->title();
    case IdRole:
        return phonemeGroup->id();
    case DataRole:
        return QVariant::fromValue<QObject*>(phonemeGroup);
    default:
        return QVariant();
    }
}

int PhonemeGroupModel::rowCount(const QModelIndex& parent) const
{
    if (!m_course) {
        return 0;
    }

    if (parent.isValid()) {
        return 0;
    }

    return m_course->phonemeGroupList().count();
}

void PhonemeGroupModel::onPhonemeGroupAboutToBeAdded(PhonemeGroup *phonemeGroup, int index)
{
    connect(phonemeGroup, SIGNAL(titleChanged()), m_signalMapper, SLOT(map()));
    //TODO add missing signals
    beginInsertRows(QModelIndex(), index, index);
}

void PhonemeGroupModel::onPhonemeGroupAdded()
{
    updateMappings();
    endInsertRows();
}

void PhonemeGroupModel::onPhonemeGroupsAboutToBeRemoved(int first, int last)
{
    beginRemoveRows(QModelIndex(), first, last);
}

void PhonemeGroupModel::onPhonemeGroupsRemoved()
{
    endRemoveRows();
}

void PhonemeGroupModel::emitPhonemeGroupChanged(int row)
{
    emit phonemeGroupChanged(row);
    emit dataChanged(index(row, 0), index(row, 0));
}

QVariant PhonemeGroupModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole) {
        return QVariant();
    }
    if (orientation == Qt::Vertical) {
        return QVariant(section + 1);
    }
    return QVariant(i18nc("@title:column", "Title"));
}

void PhonemeGroupModel::updateMappings()
{
    int phonemeGroups = m_course->phonemeGroupList().count();
    for (int i = 0; i < phonemeGroups; i++) {
        m_signalMapper->setMapping(m_course->phonemeGroupList().at(i), i);
    }
}
