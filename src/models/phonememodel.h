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

#ifndef PHONEMEMODEL_H
#define PHONEMEMODEL_H

#include <QAbstractListModel>

class Phoneme;
class Language;
class QSignalMapper;


class PhonemeModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(Language *language READ language WRITE setLanguage NOTIFY languageChanged)

public:
    enum courseRoles {
        TitleRole = Qt::UserRole + 1,
        IdRole,
        DataRole
    };

    explicit PhonemeModel(QObject *parent = 0);
    void setLanguage(Language *language);
    Language * language() const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

signals:
    void phonemeChanged(int index);
    void languageChanged();

private slots:
    void onPhonemeAboutToBeAdded(Phoneme *phoneme, int index);
    void onPhonemeAdded();
    void onPhonemesAboutToBeRemoved(int first, int last);
    void onPhonemesRemoved();
    void emitPhonemeChanged(int row);

private:
    void updateMappings();
    Language *m_language;
    QSignalMapper *m_signalMapper;
};

#endif // PHONEMEMODEL_H