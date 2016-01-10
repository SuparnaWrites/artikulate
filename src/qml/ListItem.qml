/*
 *  Copyright 2012  Sebastian Gottfried <sebastiangottfried@web.de>
 *  Copyright 2014  Andreas Cord-Landwehr <cordlandwehr@kde.org>
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

import QtQuick 2.1
import QtQuick.Controls 1.2
import org.kde.kquickcontrolsaddons 2.0

Item {
    id: root
    property alias title: label.text
    property string iconName
    property alias label: label
    signal selected
    height: Math.max(label.height + 15 + 15, 15 + 15 + 30)
    state: mouseArea.containsMouse? "hover": ListView.isCurrentItem? "selected": "normal"

    SystemPalette {
        id: palette
        colorGroup: root.enabled ? SystemPalette.Active : SystemPalette.Disabled
    }

    Rectangle {
        id: bg
        border.width: 0
        anchors.fill: parent
    }

    Item {
        id: padding

        anchors {
            fill: parent
            topMargin: 10
            rightMargin: 10
            bottomMargin: 10
            leftMargin: 10
        }

        QIconItem {
            id: iconItem
            visible: !!root.iconName
            anchors {
                left: parent.left
                verticalCenter: parent.verticalCenter
            }
            icon: root.iconName
            width: theme.smallIconSize
            height: theme.smallIconSize
        }

        Label {
            id: label
            elide: Text.ElideRight
            color: palette.buttonText
            anchors {
                left: iconItem.visible? iconItem.right: parent.left
                leftMargin: iconItem.visible ? Math.floor(theme.smallIconSize/2) : 0
                right: parent.right
                verticalCenter: parent.verticalCenter
            }
            height: paintedHeight
        }
    }

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        hoverEnabled: true
        onClicked: root.selected()
    }

    states: [
        State {
            name: "normal"
            PropertyChanges {
                target: bg
                color: palette.base
                opacity: 1
            }
        },
        State {
            name: "hover"
            PropertyChanges {
                target: bg
                color: palette.highlight
                opacity: ListView.isCurrentItem? 0.8 : 0.4
            }
        },
        State {
            name: "selected"
            PropertyChanges {
                target: bg
                color: palette.highlight
                opacity: 1
            }
        }
    ]
}
