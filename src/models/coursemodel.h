/*
 *  Copyright 2013-2015  Andreas Cord-Landwehr <cordlandwehr@kde.org>
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

#ifndef COURSEMODEL_H
#define COURSEMODEL_H

#include <QAbstractListModel>

class ResourceManager;
class Course;
class CourseResource;
class Language;
class QSignalMapper;


class CourseModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(ResourceManager *resourceManager READ resourceManager WRITE setResourceManager NOTIFY resourceManagerChanged)
    Q_PROPERTY(Language *language READ language WRITE setLanguage NOTIFY languageChanged)
    Q_PROPERTY(int size READ rowCount NOTIFY rowCountChanged)

public:
    enum courseRoles {
        TitleRole = Qt::UserRole + 1,
        DescriptionRole,
        IdRole,
        ContributerResourceRole,
        LanguageRole,
        DataRole
    };

    explicit CourseModel(QObject *parent = nullptr);
    ~CourseModel() override = default;
    /**
     * Reimplemented from QAbstractListModel::roleNames()
     */
    QHash<int,QByteArray> roleNames() const override;
    void setResourceManager(ResourceManager *resourceManager);
    ResourceManager * resourceManager() const;
    void setLanguage(Language *language);
    Language * language() const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    Q_INVOKABLE QVariant course(int index) const;

Q_SIGNALS:
    void courseChanged(int index);
    void resourceManagerChanged();
    void languageChanged();
    void rowCountChanged();

private Q_SLOTS:
    void onCourseResourceAboutToBeAdded(CourseResource *resource, int index);
    void onCourseResourceAdded();
    void onCourseResourceAboutToBeRemoved(int index);
    void emitCourseChanged(int row);

private:
    /**
     * Updates internal mappings of course signals.
     */
    void updateMappings();
    ResourceManager *m_resourceManager;
    Language *m_language;
    QList<CourseResource*> m_resources;
    QSignalMapper *m_signalMapper;
};

#endif // COURSEMODEL_H
