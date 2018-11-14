/*
 *  Copyright 2012  Sebastian Gottfried <sebastiangottfried@web.de>
 *  Copyright 2016  Andreas Cord-Landwehr <cordlandwehr@kde.org>
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
import QtQuick.Layouts 1.2

Item {
    id: root

    property alias content: contentArea.children
    property int innerMargin: 20
    signal opended
    signal closed

    function open() {
        root.state = "open"
        opended()
    }

    function close() {
        root.state = "closed"
        closed()
    }

    function isOpen() {
        return root.state == "open"
    }

    clip: true
    visible: bg.opacity > 0

    /* swallow all mouse events */
    MouseArea {
        anchors.fill: parent
        hoverEnabled: true
    }

    Rectangle {
        id: bg
        anchors.fill: parent
        color: theme.backgroundColor
        opacity: 1.0
    }

    ColumnLayout {
        anchors {
            fill: parent
            topMargin: 0
            rightMargin: 0
            bottomMargin: 0
            leftMargin: 0
        }
        Item {
            id: contentArea
            Layout.fillWidth: true
            Layout.fillHeight: true
            // the content
        }
        RowLayout {
            Item {
                Layout.fillWidth: true
            }
            FlatButton {
                id: closeButton
                text: i18n("Close")
                iconName: "dialog-close"
                iconSize: 22
                fontSize: 14
                onClicked: {
                    close()
                }
            }
        }
    }

    state: "closed"

    states: [
        State {
            name: "open"
            PropertyChanges { target: bg; opacity: 1.0 }
        },
        State {
            name: "closed"
            PropertyChanges { target: bg; opacity: 0 }
        }
    ]
}