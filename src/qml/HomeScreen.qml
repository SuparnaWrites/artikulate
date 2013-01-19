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

FocusScope {
    id: screen

    property LanguageModel languageModel
    property CourseModel courseModel

    QtObject {
        id: d
    }

    function start() {}
    function reset() {
        //TODO
    }


    Column {
        anchors.fill: parent

        PlasmaComponents.ToolBar {
            id: header
            width: parent.width
            tools: Row {
                anchors.leftMargin: 3
                anchors.rightMargin: 3
                spacing: 5

                PlasmaComponents.ToolButton {
                    id: configureButton
                    iconSource: "configure"
                    onClicked: {
                        var position = mapToItem(null, 0, height)
                        showMenu(position.x, position.y)
                    }
                }
            }
        }
        Row {
            spacing: 20

            Column {
                Text { text: "Select Language" }
                LanguageSelector {
                    languageModel : screen.languageModel
                }
            }

            Column {
                Text { text: "Select Course" }
                CourseSelector {
                    courseModel : screen.courseModel
                }
            }
        }
    }
}
