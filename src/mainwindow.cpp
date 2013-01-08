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

#include "mainwindow.h"
#include "editorview.h"

#include <KMainWindow>
#include <KGlobal>
#include <KIcon>
#include <kdeclarative.h>

#include <QDeclarativeView>
#include <QDeclarativeContext>
#include <QtCore/QCoreApplication>

MainWindow::MainWindow(const QString &file)
    : KMainWindow()
    , m_view(new QDeclarativeView(this))
{
    setWindowIcon(KIcon("artikulate")); // FIXME not present yet
    setWindowTitle(qAppName());
    setCentralWidget(m_view);

    setAutoSaveSettings();

    KDeclarative m_kdeclarative;
    m_kdeclarative.setDeclarativeEngine(m_view->engine());
    m_kdeclarative.initialize();

    m_view->setMinimumSize(1000, 700);
    m_view->setStyleSheet("background-color: transparent;");
    m_view->rootContext()->setContextObject(m_view);
    m_view->setResizeMode(QDeclarativeView::SizeRootObjectToView);
    m_view->setSource(QUrl("qrc:/qml/Main.qml"));
}

MainWindow::~MainWindow()
{}
