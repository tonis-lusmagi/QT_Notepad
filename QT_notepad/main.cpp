/*
# QT Notepad program
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

#include "mainwindow.h"
#include <QApplication>
#include <QtSvg>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv); //creates application object
    MainWindow w;
    w.show();

    return a.exec(); //loops the app
}
