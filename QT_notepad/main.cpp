/*
# QT Notepad
# Copyright (C) 2018  Lusberg
#
# This file is part of QT Notepad.
#
# QT Notepad is free software: you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# QT Notepad is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <QApplication>
#include <QCommandLineParser>
#include <QCommandLineOption>

#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QCoreApplication::setApplicationName("QT Notepad");
    QCoreApplication::setOrganizationName("Lusberg");
    QCoreApplication::setApplicationVersion(QT_VERSION_STR);
    QCommandLineParser parser;
    parser.setApplicationDescription("QT Notepad text editor");
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addPositionalArgument("file", "The file to open.");
    parser.process(app);

    MainWindow mainWin;
    foreach (const QString &fileName, parser.positionalArguments())
        mainWin.openFile(fileName);
    mainWin.show();
    return app.exec();
}
