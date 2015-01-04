/*
 *  Copyright 2014-2015  Andreas Cord-Landwehr <cordlandwehr@gkde.org>
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

#ifndef UNITFILTERMODEL_H
#define UNITFILTERMODEL_H

#include <QSortFilterProxyModel>

class UnitModel;
class QSignalMapper;

class UnitFilterModel : public QSortFilterProxyModel
{
    Q_OBJECT
    Q_PROPERTY(UnitModel *unitModel READ unitModel WRITE setUnitModel NOTIFY unitModelChanged)
    Q_PROPERTY(int filteredCount READ filteredCount NOTIFY filteredCountChanged)

public:
    explicit UnitFilterModel(QObject *parent = 0);
    UnitModel * unitModel() const;
    void setUnitModel(UnitModel* unitModel);
    int filteredCount() const;
    virtual bool lessThan(const QModelIndex &left, const QModelIndex &right) const Q_DECL_OVERRIDE;
    virtual bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const Q_DECL_OVERRIDE;

Q_SIGNALS:
    void unitModelChanged();
    void viewChanged();
    void sortOptionChanged();
    void filteredCountChanged();

private:
    UnitModel *m_unitModel;
};

#endif
