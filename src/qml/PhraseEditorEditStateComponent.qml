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

import QtQuick 1.1
import org.kde.plasma.core 0.1 as PlasmaCore
import org.kde.plasma.components 0.1 as PlasmaComponents
import artikulate 1.0

Item {
    id: root

    property Phrase phrase

    width: buttons.width
    height: buttons.height

    Component.onCompleted: {
        updateCheckedStates();
    }

    onPhraseChanged: {
        updateCheckedStates();
    }

    function updateCheckedStates()
    {
        switch (root.phrase.editState) {
        case Phrase.Unknown:
            buttonUnknown.checked = true;
            break;
        case Phrase.Translated:
            buttonTranslated.checked = true;
            break;
        case Phrase.Completed:
            buttonCompleted.checked = true;
            break;
        }
    }

    Row {
        id: buttons
        spacing: 10

        Text {
            id: componentTitle
            text: i18n("Edit State:")
            font.pointSize: 14
        }
        PlasmaComponents.ButtonRow {
            anchors { verticalCenter: componentTitle.verticalCenter; }
            PlasmaComponents.Button {
                id: buttonUnknown
                checkable: true
                text: i18n("Unknown")
                onCheckedChanged: {
                    if (!checked) return
                    phrase.editState = Phrase.Unknown
                }
            }
            PlasmaComponents.Button {
                id: buttonTranslated
                checkable: true
                text: i18n("Translated")
                onCheckedChanged: {
                    if (!checked) return
                    phrase.editState = Phrase.Translated
                }
            }
            PlasmaComponents.Button {
                id: buttonCompleted
                checkable: true
                text: i18n("Completed")
                onCheckedChanged: {
                    if (!checked) return
                    phrase.editState = Phrase.Completed
                }
            }
        }
    }
}
