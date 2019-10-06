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
#include "core/ieditablecourse.h"

#include <QObject>

class SkeletonResourcePrivate;
class IResourceRepository;

/**
 * @brief The SkeletonResource class is a decorator for EditableCourseResource
 */
class ARTIKULATECORE_EXPORT SkeletonResource : public IEditableCourse
{
    Q_OBJECT
    Q_INTERFACES(ICourse)

public:
    static std::shared_ptr<SkeletonResource> create(const QUrl &path, IResourceRepository *repository);
    ~SkeletonResource() override;

    QString id() const override;
    void setId(QString id) override;
    QString foreignId() const override;
    void setForeignId(QString id) override;
    QString title() const override;
    void setTitle(QString title) override;
    QString i18nTitle() const override;
    void setI18nTitle(QString title) override;
    QString description() const override;
    void setDescription(QString description) override;
    std::shared_ptr<ILanguage> language() const override;
    QString languageTitle() const override;
    void setLanguage(std::shared_ptr<ILanguage> language) override;
    QVector<std::shared_ptr<Unit>> units() override;
    QUrl file() const override;
    bool exportToFile(const QUrl &filePath) const override;
    Q_INVOKABLE bool createPhraseAfter(IPhrase *previousPhrase) override;
    Q_INVOKABLE bool deletePhrase(IPhrase *phrase) override;
    std::shared_ptr<Unit> addUnit(std::shared_ptr<Unit> unit) override;
    bool sync() override;
    void updateFrom(std::shared_ptr<ICourse>) override;
    bool isModified() const override;
    std::shared_ptr<IEditableCourse> self() const override;

private:
    /**
     * Create course resource from file.
     */
    explicit SkeletonResource(const QUrl &path, IResourceRepository *repository);
    void setSelf(std::shared_ptr<ICourse> self) override;
    const QScopedPointer<SkeletonResourcePrivate> d;
};

#endif
