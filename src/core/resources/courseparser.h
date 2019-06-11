/*
 *  Copyright 2013-2019  Andreas Cord-Landwehr <cordlandwehr@kde.org>
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

#ifndef COURSEPARSER_H
#define COURSEPARSER_H

#include "artikulatecore_export.h"
#include <memory>
#include <QVector>

class ICourse;
class Unit;
class Phrase;
class Phoneme;
class QXmlSchema;
class QJSonDocument;
class QDomDocument;
class QDomElement;
class QXmlStreamReader;
class QString;
class QUrl;

class ARTIKULATECORE_EXPORT CourseParser
{

public:
    /**
     * Load XSD file given by its file name (without ".xsd" suffix). The method searches exclusively
     * the standard install dir for XSD files in subdirectory "schemes/".
     *
     * \param schemeName name of the Xml schema without suffix
     * \return loaded XML Schema
     */
    static QXmlSchema loadXmlSchema(const QString &schemeName);

    /**
     * Load XML file given by \p file that confirms with XML schema \p scheme.
     *
     * \param path is the path to the XML file to be loaded
     * \param scheme is the XML schema describing the DOM
     * \return the loaded DOM document
     */
    static QDomDocument loadDomDocument(const QUrl &path, const QXmlSchema &schema);

    static std::vector<std::unique_ptr<Unit>> parseUnits(const QUrl &path);

    static QDomDocument serializedDocument(ICourse *course, bool trainingExport);
    static QDomElement serializedPhrase(Phrase *phrase, QDomDocument &document);
    static bool exportCourseToGhnsPackage(ICourse *course, const QString &exportPath);

private:
    static std::unique_ptr<Unit> parseUnit(QXmlStreamReader &xml, const QUrl &path, bool &ok);
    static Phrase * parsePhrase(QXmlStreamReader &xml, const QUrl &path, bool &ok);
    static QStringList parsePhonemeIds(QXmlStreamReader &xml, bool &ok);
    static QString parseElement(QXmlStreamReader &xml, bool &ok);
};

#endif
