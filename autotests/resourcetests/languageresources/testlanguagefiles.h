/*
 *  Copyright 2013  Oindrila Gupta <oindrila.gupta92@gmail.com>
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

#ifndef TESTLANGUAGEFILES_H
#define TESTLANGUAGEFILES_H

#include <QObject>

class QXmlSchema;
class QDomDocument;
class QUrl;

class TestLanguageFiles : public QObject
{
    Q_OBJECT

public:
    TestLanguageFiles();

private slots:
    /**
     * Called before every test case.
     */
    void init();

    /**
     * Called after every test case.
     */
    void cleanup();

    /**
     * Test if language XSD specification is valid.
     */
    void languageSchemeValidationTest();

    /**
     * Test if id of each phoneme is unique in every language file.
     */
    void checkIdUniqueness();
};

#endif