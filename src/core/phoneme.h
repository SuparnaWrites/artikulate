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

#ifndef PHONEME_H
#define PHONEME_H

#include "artikulatecore_export.h"
#include <QMap>
#include <QObject>

class QString;

/**
 * \class Phoneme
 * Phonemes are properties that can be assigned to a Phrase to specify its pronunciation characteristics.
 */
class ARTIKULATECORE_EXPORT Phoneme : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString id READ id WRITE setId NOTIFY idChanged)
    Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged)

public:
    explicit Phoneme(QObject *parent = nullptr);
    QString id() const;
    void setId(const QString &id);
    QString title() const;
    void setTitle(const QString &title);

signals:
    void idChanged();
    void titleChanged();

private:
    Q_DISABLE_COPY(Phoneme)
    QString m_id;
    QString m_title;
};

#endif // PHONEME_H
