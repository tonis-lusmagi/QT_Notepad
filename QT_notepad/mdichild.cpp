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

#include <QtWidgets>

#include "mdichild.h"

MdiChild::MdiChild()
{
    setAttribute(Qt::WA_DeleteOnClose);
    isUntitled = true;
    this->setFontPointSize(11);
    this->setFontFamily("Consolas");
    setAcceptDrops(false); //dragndrop on text area
}

void MdiChild::newFile()
{
    static int sequenceNumber = 1;

    isUntitled = true;
    curFile = tr("untitled%1").arg(sequenceNumber++); //curFile = tr("untitled%1.txt").arg(sequenceNumber++);
    setWindowTitle(curFile + "[*]");

    connect(document(), &QTextDocument::contentsChanged,
            this, &MdiChild::documentWasModified);
}

bool MdiChild::loadFile(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("QT Notepad"),
                             tr("Cannot read file %1:\n%2.")
                             .arg(fileName)
                             .arg(file.errorString()));
        return false;
    }

    QTextStream in(&file);
    QApplication::setOverrideCursor(Qt::WaitCursor);
    setPlainText(in.readAll());
    QApplication::restoreOverrideCursor();

    setCurrentFile(fileName);

    connect(document(), &QTextDocument::contentsChanged,
            this, &MdiChild::documentWasModified);

    return true;
}

bool MdiChild::save()
{
    if (isUntitled) {
        return saveAs();
    } else {
        return saveFile(curFile);
    }
}

bool MdiChild::saveAs()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save As"),
                                                    curFile);
    //QString fileName = QFileDialog::getSaveFileName(this, tr("Save As"), "", tr("Text Files (*.txt);;C++ Files (*.cpp *.h)"));
    if (fileName.isEmpty())
        return false;

    return saveFile(fileName);
}

bool MdiChild::saveFile(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("QT Notepad"),
                             tr("Cannot write file %1:\n%2.")
                             .arg(QDir::toNativeSeparators(fileName), file.errorString()));
        return false;
    }

    QTextStream out(&file);
    QApplication::setOverrideCursor(Qt::WaitCursor);
    out << toPlainText();
    QApplication::restoreOverrideCursor();

    setCurrentFile(fileName);
    return true;
}

QString MdiChild::userFriendlyCurrentFile()
{
    return strippedName(curFile);
}

void MdiChild::closeEvent(QCloseEvent *event)
{
    if (maybeSave()) {
        event->accept();
    } else {
        event->ignore();
    }
}

void MdiChild::documentWasModified()
{
    setWindowModified(document()->isModified());
}

bool MdiChild::maybeSave()
{
    if (!document()->isModified())
        return true;
    const QMessageBox::StandardButton ret
            = QMessageBox::warning(this, tr("QT Notepad"),
                                   tr("%1 has been modified.\n"
                                      "Do you want to save your changes?")
                                   .arg(userFriendlyCurrentFile()),
                                   QMessageBox::Save | QMessageBox::Discard
                                   | QMessageBox::Cancel);
    switch (ret) {
    case QMessageBox::Save:
        return save();
    case QMessageBox::Cancel:
        return false;
    default:
        break;
    }
    return true;
}

void MdiChild::setCurrentFile(const QString &fileName)
{
    curFile = QFileInfo(fileName).canonicalFilePath();
    isUntitled = false;
    document()->setModified(false);
    setWindowModified(false);
    setWindowTitle(userFriendlyCurrentFile() + "[*]");
}

QString MdiChild::strippedName(const QString &fullFileName)
{
    return QFileInfo(fullFileName).fileName();
}
