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

#ifndef UNITMODEL_H
#define UNITMODEL_H

#include <QAbstractListModel>

class Course;
class Unit;
class QSignalMapper;


class UnitModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(Course *course READ course WRITE setCourse NOTIFY courseChanged)

public:
    enum unitRoles {
        TitleRole = Qt::UserRole + 1,
        NumberPhrasesRole,
        IdRole,
        DataRole
    };

    explicit UnitModel(QObject *parent = 0);
    void setCourse(Course *course);
    Course * course() const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

signals:
    void unitChanged(int index);
    void courseChanged();

private slots:
    void onUnitAboutToBeAdded(Unit *unit, int index);
    void onUnitAdded();
    void onUnitsAboutToBeRemoved(int first, int last);
    void onUnitsRemoved();
    void emitUnitChanged(int row);

private:
    void updateMappings();
    Course *m_course;
    QSignalMapper *m_signalMapper;
};

#endif // UNITMODEL_H
