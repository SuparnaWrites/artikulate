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

#include "skeletonmodel.h"
#include "core/course.h"
#include "core/resourcemanager.h"
#include "core/skeleton.h"

#include <QAbstractListModel>
#include <QSignalMapper>

#include <KLocale>
#include <KDebug>

SkeletonModel::SkeletonModel(QObject *parent)
    : QAbstractListModel(parent)
    , m_resourceManager(0)
    , m_signalMapper(new QSignalMapper(this))
{
    QHash<int, QByteArray> roles;
    roles[TitleRole] = "title";
    roles[DescriptionRole] = "description";
    roles[IdRole] = "id";
    roles[DataRole] = "dataRole";
    setRoleNames(roles);

    connect(m_signalMapper, SIGNAL(mapped(int)), SLOT(emitSkeletonChanged(int)));
}

void SkeletonModel::setResourceManager(ResourceManager *resourceManager)
{
    if (m_resourceManager == resourceManager) {
        return;
    }

    beginResetModel();

    if (m_resourceManager) {
        m_resourceManager->disconnect(this);
    }

    m_resourceManager = resourceManager;

    if (m_resourceManager) {
        connect(m_resourceManager, SIGNAL(skeletonAboutToBeAdded(Course*,int)), SLOT(onSkeletonAboutToBeAdded(Course*,int)));
        connect(m_resourceManager, SIGNAL(skeletonAdded()), SLOT(onSkeletonAdded()));
        connect(m_resourceManager, SIGNAL(skeletonAboutToBeRemoved(int,int)), SLOT(onSkeletonsAboutToBeRemoved(int,int)));
        connect(m_resourceManager, SIGNAL(skeletonRemoved()), SLOT(onSkeletonsRemoved()));
    }

    endResetModel();

    emit resourceManagerChanged();
}

ResourceManager * SkeletonModel::resourceManager() const
{
    return m_resourceManager;
}

QVariant SkeletonModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid()) {
        return QVariant();
    }

    if (index.row() >= m_resourceManager->skeletonList().count()) {
        return QVariant();
    }

    Skeleton * const skeleton = m_resourceManager->skeletonList().at(index.row());

    switch(role)
    {
    case Qt::DisplayRole:
        return !skeleton->title().isEmpty()?
                QVariant(skeleton->title()): QVariant(i18n("<No title>"));
    case Qt::ToolTipRole:
        return QVariant(i18n("<p>%1</p>", skeleton->title()));
    case TitleRole:
        return skeleton->title();
    case DescriptionRole:
        return skeleton->description();
    case IdRole:
        return skeleton->id();
    case DataRole:
        return QVariant::fromValue<QObject*>(skeleton);
    default:
        return QVariant();
    }
}

int SkeletonModel::rowCount(const QModelIndex &parent) const
{
    if (!m_resourceManager) {
        return 0;
    }

    if (parent.isValid()) {
        return 0;
    }

    return m_resourceManager->skeletonList().count();
}

void SkeletonModel::onSkeletonAboutToBeAdded(Course *skeleton, int index)
{
    connect(skeleton, SIGNAL(titleChanged()), m_signalMapper, SLOT(map()));
    //TODO add missing signals
    beginInsertRows(QModelIndex(), index, index);
}

void SkeletonModel::onSkeletonAdded()
{
    updateMappings();
    endInsertRows();
    emit countChanged();
}

void SkeletonModel::onSkeletonsAboutToBeRemoved(int first, int last)
{
    beginRemoveRows(QModelIndex(), first, last);
}

void SkeletonModel::onSkeletonsRemoved()
{
    endRemoveRows();
    emit countChanged();
}

void SkeletonModel::emitSkeletonChanged(int row)
{
    emit skeletonChanged(row);
    emit dataChanged(index(row, 0), index(row, 0));
}

QVariant SkeletonModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole) {
        return QVariant();
    }
    if (orientation == Qt::Vertical) {
        return QVariant(section + 1);
    }
    return QVariant(i18n("Title"));
}

int SkeletonModel::count() const
{
    return m_resourceManager->skeletonList().count();
}

void SkeletonModel::updateMappings()
{
    int skeletons = m_resourceManager->skeletonList().count();
    for (int i = 0; i < skeletons; i++) {
        m_signalMapper->setMapping(m_resourceManager->skeletonList().at(i), i);
    }
}