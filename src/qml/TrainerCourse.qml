/*
 *  Copyright 2013  Andreas Cord-Landwehr <cordlandwehr@kde.org>
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

import QtQuick 1.1
import org.kde.plasma.core 0.1 as PlasmaCore
import org.kde.plasma.components 0.1 as PlasmaComponents
import artikulate 1.0

FocusScope {
    id: root

    property TrainingSession session
    property bool finished: session.isFinished

    signal closeCourse()
    signal closeUnit()

    onFinishedChanged: {
        if (finished) {
            trainerMain.visible = false
            finishedInformation.visible = true
            progressBar.visible = false
        }
        else {
            trainerMain.visible = true
            finishedInformation.visible = false
            progressBar.visible = true
        }
    }

    PlasmaComponents.ToolBar {
        id: header
        width: root.width
        // no tools yet
    }

    Rectangle {
        id: trainingControls
        width: parent.width
        height: Math.max(unitIcon.height,stopButton.height) + 6
        anchors {
            top: header.bottom
            topMargin: 2
        }
        color: theme.backgroundColor

        Row {
            spacing: 20
            anchors {
                topMargin: 3
                top: parent.top
                bottomMargin: 3
                bottom: parent.bottom
                leftMargin: 5
                left: parent.left
            }
            PlasmaCore.IconItem {
                id: unitIcon
                source: "artikulate-course"
                width: theme.mediumIconSize
                height: theme.mediumIconSize
                anchors.verticalCenter: parent.verticalCenter
            }

            PlasmaComponents.Label {
                anchors {
                    verticalCenter: parent.verticalCenter
                }
                height: paintedHeight
                width: root.width - 2*20 - unitIcon.width - stopButton.width - 2*5
                text: {
                    var title = i18n("unselected")
                    if (userProfile.unit != null) {
                        title = userProfile.unit.title
                    }
                    i18n("Unit: %1", title)
                }
                font.pointSize: 1.5 * theme.defaultFont.pointSize
            }

            PlasmaComponents.ToolButton {
                id: stopButton
                anchors {
                    verticalCenter: parent.verticalCenter
                }
                iconSource: "go-up"
                height: 48
                text: i18n("Finish Training")
                onClicked: {
                    closeUnit()
                }
            }
        }
    }

    TrainerSessionScreen {
        id: trainerMain
        width: root.width - 40
        height: root.height - 30 - trainingControls.height - 50 - 50 - progressBar.height - 30
        anchors {
            top: trainingControls.bottom
            left: parent.left
            topMargin: 50
            leftMargin: 20
        }
        unit: userProfile.unit
        session: root.session
    }

    Column {
        id: finishedInformation
        anchors { top: trainingControls.bottom; left: trainingControls.left; topMargin: 20; right: root.right }
        visible: false
        spacing: 10

        Text {
            id: results
            text: i18n("Results")
            font.pointSize: 20
        }

        Row {
            spacing: 15
            anchors { right: parent.right; rightMargin: 30 }

            Rectangle {
                id: rect1
                height: 15
                width: 15
                color: "#2de86c"
            }

            Text {
                id: label1
                text: "Word"
                font.pointSize: 10
            }

            Rectangle {
                id: rect2
                height: 15
                width: 15
                color: "#327bff"
            }

            Text {
                id: label2
                text: "Expression"
                font.pointSize: 10
            }

            Rectangle {
                id: rect3
                height: 15
                width: 15
                color: "#fff13f"
            }

            Text {
                id: label3
                text: "Sentence"
                font.pointSize: 10
            }

            Rectangle {
                id: rect4
                height: 15
                width: 15
                color: "#e85a02"
            }

            Text {
                id: label4
                text: "Paragraph"
                font.pointSize: 10
            }
        }
        TrainerCourseStatistics {
            width: root.width - 60
            height: root.height - 30 - trainingControls.height - 50 - 50
        }
    }

    Column {
        id: progressBar
        anchors {
            bottom: root.bottom
            bottomMargin: 20
            left: trainerMain.left
        }

        spacing: 10
        Text {
            id: trainingText
            text: i18n("Training Progress")
            font.pointSize: theme.defaultFont.pointSize
        }
        TrainerProgressBar {
            width: trainerMain.width
            session: root.session
            sessionType: root.session.currentType
            onTypeSelected: {
                userProfile.phraseType = type
            }
        }
    }
}
