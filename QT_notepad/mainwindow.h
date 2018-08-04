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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMimeData>

class MdiChild;
class QAction;
class QMenu;
class QMdiArea;
class QMdiSubWindow;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();

    bool openFile(const QString &fileName);

protected:
    void closeEvent(QCloseEvent *event) override;
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dropEvent(QDropEvent *event) override;


private slots:
    void newFile();
    void open();
    void save();
    void saveAs();
    void updateRecentFileActions();
    void openRecentFile();

#ifndef QT_NO_CLIPBOARD
    void cut();
    void copy();
    void paste();
    void undo();
    void redo();
#endif
    void about();
    void updateMenus();
    void updateWindowMenu();
    MdiChild *createMdiChild();
    //void switchLayoutDirection();

private:
    enum { MaxRecentFiles = 5 };

    void createActions();
    void createStatusBar();
    void readSettings();
    void writeSettings();
    bool loadFile(const QString &fileName);
    static bool hasRecentFiles();
    void prependToRecentFiles(const QString &fileName);
    void setRecentFilesVisible(bool visible);
    MdiChild *activeMdiChild() const;
    QMdiSubWindow *findMdiChild(const QString &fileName) const;

    QMdiArea *mdiArea;

    QMenu *windowMenu;
    QAction *newAct;
    QAction *saveAct;
    QAction *saveAsAct;
    QAction *recentFileActs[MaxRecentFiles];
    QAction *recentFileSeparator;
    QAction *recentFileSubMenuAct;
#ifndef QT_NO_CLIPBOARD
    QAction *cutAct;
    QAction *copyAct;
    QAction *pasteAct;
    QAction *undoAct;
    QAction *redoAct;
#endif
    QAction *closeAct;
    QAction *closeAllAct;
    QAction *tileAct;
    QAction *cascadeAct;
    QAction *nextAct;
    QAction *previousAct;
    QAction *windowMenuSeparatorAct;
};

#endif
