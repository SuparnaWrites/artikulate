/*
 *  Copyright 2015-2016  Andreas Cord-Landwehr <cordlandwehr@kde.org>
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

import QtQuick 2.5
import QtQuick.Controls 1.2

MenuBar {
    id: root
    Menu {
        title: i18n("File")
        MenuItem {
            text: i18n("Edit User")
            iconName: "user-properties"
            onTriggered: triggerEditProfile()
        }
        MenuSeparator { }
        MenuItem {
            text: i18n("Quit")
            iconName: "application-exit"
            shortcut: StandardKey.Quit
            onTriggered: triggerAction("file_quit")
        }
    }
    Menu {
        title: i18n("Settings")
        MenuItem {
            text: i18n("Download Courses")
            iconName: "get-hot-new-stuff"
            onTriggered: triggerDownloadCourses()
        }
        MenuSeparator { }
// TODO requires proper support of KKeySequenceWidget's settings
//         MenuItem {
//             text: i18n("Configure Shortcuts...")
//             iconName: "configure-shortcuts"
//             onTriggered: triggerAction("options_configure_keybinding")
//         }
        MenuItem {
            text: i18n("Configure Artikulate...")
            iconName: "settings-configure"
            onTriggered: triggerSettingsDialog()
        }
        MenuSeparator { }
        MenuItem {
            text: i18n("Show Menubar")
            iconName: "show-menu"
            checkable: true
            checked: kcfg_ShowMenuBar
            onTriggered: {
                switchMenuBarVisibility()
            }
        }
    }
    Menu {
        title: i18n("Help")
        MenuItem {
            text: i18n("Artikulate Handbook")
            iconName: "help-contents"
            shortcut: StandardKey.HelpContents
            onTriggered: triggerAction("help_contents")
        }
        MenuSeparator { }
        MenuItem {
            text: i18n("Report Bug")
            iconName: "tools-report-bug"
            onTriggered: triggerAction("help_report_bug")
        }
        MenuSeparator { }
        MenuItem {
            text: i18n("About Artikulate")
            iconName: "artikulate"
            onTriggered: triggerAction("help_about_app")
        }
        MenuItem {
            text: i18n("About KDE")
            iconName: "help-about"
            onTriggered: triggerAction("help_about_kde")
        }
    }
}
