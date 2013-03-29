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

import QtQuick 1.1
import org.kde.plasma.core 0.1 as PlasmaCore
import org.kde.plasma.components 0.1 as PlasmaComponents
import artikulate 1.0

Item {
    id: root

    property Course course
    property PhonemeGroup currentPhonemeGroup

    signal unitSelected(variant unit)

    width: phonemeGroupList.width
    height: phonemeGroupList.height

    PhonemeGroupModel {
        id: phonemeGroupModel
        course: root.course
    }

    PhonemeUnitModel {
        id: phonemeUnitModel
        course: root.course
        phonemeGroup: root.currentPhonemeGroup
    }

    onCourseChanged: {
        root.currentPhonemeGroup = null
    }

    Component {
        id: groupDelegate

        Column {
            id: content

            property PhonemeGroup phonemeGroup: model.dataRole

            height: groupSelectButton.height + unitList.height
            width: 200

            PlasmaComponents.ToolButton {
                id: groupSelectButton
                text : model.title

                onClicked: {
                    root.currentPhonemeGroup = phonemeGroup
                }
            }

            ListView {
                id: unitList
                visible: root.currentPhonemeGroup == phonemeGroup
                height: unitList.visible ? 30 * phonemeUnitModel.count : 0
                width: 200

                model: phonemeUnitModel
                delegate: unitDelegate
            }
        }
    }

    Component {
        id: unitDelegate

        PlasmaComponents.ToolButton {
            text : model.title
            property Unit unit: model.dataRole
            onClicked: {
                root.unitSelected(unit)
            }
        }
    }

    ListView {
        id: phonemeGroupList

        height: 300
        width: 200
        model: phonemeGroupModel
        delegate: groupDelegate
    }
}