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

Column {
    id: root
    property Language selectedLanguage
    property Course selectedCourse
    property bool isSkeleton

    function unselect() {
        selectedLanguage = null
        selectedCourse = null
    }

    spacing: 20
    visible: selectedCourse == null

    onSelectedLanguageChanged: {
        if (selectedLanguage != null) {
            selectedCourse = null
            isSkeleton = false
        }
    }

    PlasmaComponents.ButtonColumn {
        id: modeSelector
        exclusive: true

        PlasmaComponents.ToolButton {
            id: editCourses
            checkable: true
            text: i18n("Edit Courses")
            onCheckedChanged: {
                if (checked) {
                    root.isSkeleton = false
                }
            }
        }
        PlasmaComponents.ToolButton {
            id: editSkeletons
            checkable: true
            text: i18n("Edit Skeletons")
            onCheckedChanged: {
                if (checked) {
                    root.isSkeleton = true
                }
            }
        }
    }

    Column {
        height: root.height
        width: root.width
        visible: editCourses.checked
        Column {
            id: languageColumn
            spacing: 5
            visible: selectedLanguage == null

            Text {
                id: textSelectLanguage
                text: i18n("Select Course Language:")
                font.pointSize: 20;
            }
            LanguageSelector {
                id: languageSelector
                width: root.width
                height: root.height - modeSelector.height - textSelectLanguage.height - 20
                hideEmpty: false
                resourceManager: globalResourceManager
                onLanguageSelected: {
                    root.selectedLanguage = language
                }
            }
        }

        Column {
            spacing: 5
            visible: {
                if (selectedLanguage == null) return false;
                if (selectedCourse != null) return false;
                return true;
            }
            Row {
                id: rowLanguageInformation
                spacing: 10
                PlasmaComponents.ToolButton {
                    anchors.verticalCenter: parent.verticalCenter
                    text: i18n("Language: ")
                    iconSource: "go-up"
                    font.pointSize: 20;
                    onClicked: {
                        unselect()
                    }
                }
                Text {
                    id: unitTitle
                    anchors.verticalCenter: parent.verticalCenter
                    text: {
                        if (root.selectedLanguage) {
                            return root.selectedLanguage.title
                        } else {
                            return i18n("unselected");
                        }
                    }
                    font.pointSize: 20;
                }
            }

            PlasmaComponents.ToolButton {
                id: buttonAddCourse
                text: i18n("New Course")
                iconSource: "document-new"
                enabled: selectedLanguage != null
                onClicked: {
                    globalResourceManager.newCourseDialog();
                }
            }
            CourseSelector {
                id: courseSelector
                width: root.width
                height: root.height - modeSelector.height - rowLanguageInformation.height - buttonAddCourse.height - 40
                resourceManager: globalResourceManager
                language: selectedLanguage
                onCourseSelected: {
                    root.selectedCourse = course
                }
            }
        }
    }

    Column {
        visible: editSkeletons.checked
        spacing: 5

        Text {
            text: i18n("Select Skeleton:")
            font.pointSize: 20;
        }
        SkeletonSelector {
            id: skeletonSelector
            width: root.width
            height: root.height - modeSelector.height - 20
            resourceManager: globalResourceManager
            onSkeletonSelected: {
                root.selectedCourse = skeleton
            }
        }
    }
}
