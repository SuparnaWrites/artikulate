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

#ifndef SKELETONRESOURCE_H
#define SKELETONRESOURCE_H

#include "artikulatecore_export.h"
#include "core/icourse.h"

#include <QObject>

class SkeletonResourcePrivate;
class IResourceRepository;

/**
 * @brief The SkeletonResource class is a decorator for EditableCourseResource
 */
class ARTIKULATECORE_EXPORT SkeletonResource : public ICourse
{
    Q_OBJECT
    Q_INTERFACES(ICourse)

public:
    /**
     * Create course resource from file.
     */
    explicit SkeletonResource(const QUrl &path, IResourceRepository *repository);

    ~SkeletonResource() override;

    QString id() const override;

    void setId(const QString &id);

    QString foreignId() const override;
    QString title() const override;

    void setTitle(const QString &title);

    QString i18nTitle() const override;
    QString description() const override;

    void setDescription(const QString &description);

    Language * language() const override;
    QList<Unit *> unitList() override;
    QUrl file() const override;

    void addUnit(Unit *unit);
    void sync();

    bool isModified() const { return true;} //FIXME

private:
    const QScopedPointer<SkeletonResourcePrivate> d;
};

#endif
