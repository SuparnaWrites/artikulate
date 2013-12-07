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
import artikulate 1.0
import org.kde.plasma.core 0.1 as PlasmaCore
import org.kde.plasma.components 0.1 as PlasmaComponents

Item
{
    id: editor

    property Language currentLanguage: editorProfile.language
    property Course currentCourse: editorProfile.course
    property Unit currentUnit: editorProfile.unit
    property Course currentSkeleton
    property string currentLanguageName: i18n("Unselected")
    property string currentCourseName: i18n("Unselected")
    property string currentUnitName: i18n("Unselected")

    signal closeEditor()

    width: 400 //parent.width
    height: 400 //parent.height

    onCurrentCourseChanged: {
        editorProfile.unit = null
        if (editorProfile.course == null) {
            editor.currentCourseName = i18n("Unselected");
        }
        else {
            editor.currentCourseName = editorProfile.course.title;
        }
    }

    onCurrentLanguageChanged: {
        editorProfile.unit = null
        editorProfile.course = null
        if (editorProfile.language == null) {
            currentLanguageName = i18n("Unselected");
        }
        else {
            editor.currentLanguageName = editorProfile.language.title;
        }
    }

    onCurrentUnitChanged: {
        if (editorProfile.unit == null) {
            editor.currentUnitName = i18n("Unselected");
        }
        else {
            editor.currentUnitName = editorProfile.unit.title;
        }
    }

    ApplicationBackground {
        id: background
        anchors.fill: parent
    }

    UnitModel {
        id: selectedUnitModel
        course: editorProfile.course
    }

    PlasmaComponents.ToolBar {
        id: breadcrumb
        anchors { top: editor.top; left: editor.left}
        width: editor.width
        height: 30

        tools: Row {
            width: parent.width
            anchors { leftMargin: 3; rightMargin: 3 }
            spacing: 5

            Row {
                id: selectorInformation
                width: 300
                height: parent.height
                anchors.verticalCenter: parent.verticalCenter

                Row {
                    visible: !editCourseSelector.isSkeleton
                    Text {
                        anchors.verticalCenter: parent.verticalCenter
                        text: i18n("<strong>Language:</strong> %1", editor.currentLanguageName)
                    }
                    PlasmaComponents.ToolButton { // unselect-button for language
                        id: unselectLanguage
                        anchors.verticalCenter: parent.verticalCenter
                        iconSource: "dialog-close"
                        flat: true
                        enabled: editorProfile.language != null
                        onClicked: {
                            editCourseSelector.unselect()
                        }
                    }
                    Text {
                        anchors { verticalCenter: parent.verticalCenter }
                        text: i18n("<strong>Course:</strong> %1", editor.currentCourseName)
                    }
                    PlasmaComponents.ToolButton { // sync button for skeleton
                        anchors.verticalCenter: parent.verticalCenter
                        text: i18n("Update from Course Prototype")
                        iconSource: "svn-update"
                        flat: true
                        enabled: { editorProfile.course != null }
                        onClicked: {
                            globalResourceManager.updateCourseFromSkeleton(editorProfile.course);
                        }
                    }
                }
                Row {
                    visible: editCourseSelector.isSkeleton
                    Text {
                        anchors.verticalCenter: parent.verticalCenter
                        text: i18n("<strong>Skeleton:</strong> %1", editor.currentCourseName)
                    }
                    PlasmaComponents.ToolButton { // unselect-button for language
                        anchors.verticalCenter: parent.verticalCenter
                        iconSource: "dialog-close"
                        flat: true
                        enabled: editorProfile.course != null
                        onClicked: {
                            editCourseSelector.unselect()
                        }
                    }
                }
            }
            Row {
                width: breadcrumb.width - selectorInformation.width - 10
                layoutDirection: Qt.RightToLeft
                Row {
                    PlasmaComponents.ToolButton {
                        anchors.verticalCenter: parent.verticalCenter
                        text: i18n("OK")
                        visible: {
                            if (editorProfile.course != null) return editorProfile.course.modified;
                            else return false;
                        }
                        enabled: {
                            if (editorProfile.course != null) return editorProfile.course.modified;
                            else return false;
                        }
                        iconSource: "dialog-ok-apply"
                        onClicked: {
                            editorProfile.course.sync();
                            editCourseSelector.unselect()
                        }
                    }
                    PlasmaComponents.ToolButton {
                        anchors.verticalCenter: parent.verticalCenter
                        text: i18n("Cancel")
                        visible: {
                            if (editorProfile.course != null) return editorProfile.course.modified;
                            else return false;
                        }
                        enabled: {
                            if (editorProfile.course != null) return editorProfile.course.modified;
                            else return false;
                        }
                        iconSource: "dialog-cancel"
                        onClicked: {
                            globalResourceManager.reloadCourseOrSkeleton(editorProfile.course)
                            editCourseSelector.unselect()
                        }
                    }
                    PlasmaComponents.ToolButton {
                        anchors.verticalCenter: parent.verticalCenter
                        text: i18n("Close Course")
                        visible: {
                            if (editorProfile.course != null) return !editorProfile.course.modified;
                            else return false;
                        }
                        iconSource: "dialog-close"
                        onClicked: {
                            editCourseSelector.unselect()
                        }
                    }
                }
                Row {
                    PlasmaComponents.ToolButton { // unselect-button for language
                        anchors.verticalCenter: parent.verticalCenter
                        iconSource: "go-up"
                        text: i18n("Close Editor")
                        visible: {
                            if (editorProfile.course != null) return false;
                            else return true;
                        }
                        flat: true
                        onClicked: {
                            editCourseSelector.unselect()
                            closeEditor()
                        }
                    }
                }
            }
        }
    }

    Column {
        id: main

        anchors {
            top: breadcrumb.bottom
            left: editor.left
            topMargin: 30
            leftMargin: 30
        }
        width: editor.width - 60
        height: editor.height - breadcrumb.height - 80
        spacing: 10

        visible: globalResourceManager.isRepositoryManager()

        Text {
            id: editorTitle
            text: i18n("Editor")
            font.pointSize: 28;
        }
        Row {
            id: currentCourseTitle
            spacing: 10
            Text {
                text: {
                    if (editCourseSelector.isSkeleton)
                        i18n("Course Skeleton:")
                    else
                        i18n("Course:")
                }
                font.pointSize: 20;
            }
            Text {
                text: currentCourseName
                font.pointSize: 20;
            }
        }

        EditorSelector {
            id: editCourseSelector
            width: editor.width - 60
            height: main.height - unitSelectorCaption.height - editorTitle.height - currentCourseTitle.height - 20
            onSelectedLanguageChanged: {
                editorProfile.language = selectedLanguage
            }
            onSelectedCourseChanged: {
                editorProfile.language = selectedLanguage
                editorProfile.course = selectedCourse
            }
        }

        Item {
            anchors { top: currentCourseTitle.bottom; topMargin: 30 }
            visible: editorProfile.course != null

            Column {
                id: unitSelectorColumn
                width: 200
                visible: (editorProfile.unit == null)

                Text {
                    id: unitSelectorCaption
                    text: i18n("Units")
                    font.pointSize: 20;
                }
                PlasmaComponents.ToolButton {
                    id: unitAddButton
                    text: i18n("Add Unit")
                    iconSource: "document-new"
                    enabled: editorProfile.course != null
                    onClicked: {
                        editorProfile.course.createUnit()
                    }
                }
                UnitSelector {
                    id: unitSelector
                    height: main.height - unitSelectorCaption.height - unitAddButton.height - editorTitle.height - currentCourseTitle.height
                    unitModel: selectedUnitModel
                    onUnitSelected: {
                        editorProfile.unit = unit;
                    }
                }
            }

            Column {
                visible: (editorProfile.unit != null)

                UnitEditor {
                    width: main.width
                    height: main.height - editorTitle.height - 80
                    unit: editorProfile.unit
                    onCloseUnit: {
                        editorProfile.unit = null
                    }
                }
            }
        }
    }

    Column {
        anchors {
            top: breadcrumb.bottom
            left: editor.left
            topMargin: 10
            leftMargin: 10
            bottomMargin: 10
        }
        spacing: 10
        visible: !globalResourceManager.isRepositoryManager()
        Text {
            text: i18n("No Course Repository Selected")
            font.pointSize: 28
        }
        Text {
            text: i18n("Editor is only available when Artikulate works on a course data repository. Please see program settings.")
            wrapMode: Text.WordWrap
        }
    }
}
