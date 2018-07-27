/*
# QT Notepad program
# Copyright (C) 2018  Lusberg
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
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow) //creates a class and assigns it to UI member
{
    ui->setupUi(this);
    this->setCentralWidget(ui->textEdit); //widget full screen
    connect(ui->textEdit->document(), &QTextDocument::contentsChanged, this, &MainWindow::documentWasModified);
    QString fileName = ("Untitled");
    currentFile = fileName;
    //documentWasModified();
    setWindowTitle(QT_Notepad + fileName + "[*]");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionNew_triggered()
{
    if (!ui->textEdit->document()->isModified() | ((!mod) & (!isWindowModified))) {
        currentFile.clear();
        ui->textEdit->setText(QString());
        QString fileName = ("Untitled");
        currentFile = fileName;
        documentWasModified();
        setWindowTitle(QT_Notepad + fileName + "[*]");
    } else {
        QMessageBox msgBox(this); //'this' sets QMessageBox parent to QMainWindow (inherits style)
        msgBox.setWindowTitle("New File");
        msgBox.setText("Save current file?");
        msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Yes);
        int ret = msgBox.exec();
        QString fileName = ("Untitled");

        switch (ret) {
        case QMessageBox::Yes:
            MainWindow::on_actionSave_As_triggered();
            currentFile.clear(); //clears currentFile name
            ui->textEdit->setText(QString()); //clears text editor
            currentFile = fileName;
            documentWasModified();
            setWindowTitle(QT_Notepad + fileName + "[*]");
            break;
        case QMessageBox::No:
            currentFile.clear();
            ui->textEdit->setText(QString());
            currentFile = fileName;
            documentWasModified();
            setWindowTitle(QT_Notepad + fileName + "[*]");
            break;
        case QMessageBox::Cancel:
            break; // do nothing
        default:
            break;
        }
    }
}

void MainWindow::on_actionOpen_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "", tr("Text Files (*.txt);;C++ Files (*.cpp *.h)"));
    QFile file(fileName);
    currentFile = fileName;

    if (fileName != "") {
        if(!file.open(QIODevice::ReadOnly | QFile::Text)) {
            QMessageBox::warning(this, tr("Error"), tr("Could not open file: ") + file.errorString());
            return;
        }

    QTextStream in(&file);
    QString text = in.readAll();
    ui->textEdit->setText(text);
    file.close();
    documentWasModified();
    setWindowTitle(QT_Notepad + fileName + "[*]");
    }
}

void MainWindow::on_actionSave_As_triggered()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save As"), "", tr("Text Files (*.txt);;C++ Files (*.cpp *.h)"));
    QFile file(fileName);
    currentFile = fileName;

    if (fileName != "") {
        if(!file.open(QFile::WriteOnly | QFile::Text)) {
            QMessageBox::warning(this, tr("Error"), tr("Could not save file: ") + file.errorString());
            return;
        }
    QTextStream out(&file);
    QString text = ui->textEdit->toPlainText();
    out << text;
    file.close();

    documentWasModified();
    mod = false;
    isWindowModified = false;
    setWindowModified(false); //removes *, but remains modified after saving and modifying
                              //setting mod = false and using as control statement with & isWindowModified = false
                              //to fix the bug.
    setWindowTitle(QT_Notepad + fileName + "[*]");
    }
}

void MainWindow::on_actionPrint_triggered()
{
    QPrinter printer;
    printer.setPrinterName("Printer Name");
    QPrintDialog pDialog(&printer, this);

    if(pDialog.exec() == QDialog::Rejected){
        //QMessageBox::warning(this, tr("Error"), tr("Could not print file: "));
        return;
    }

    ui->textEdit->print(&printer);
}

void MainWindow::on_actionExit_triggered()
{
    if (!ui->textEdit->document()->isModified() | ((!mod) & (!isWindowModified))) {
        QApplication::quit();
    } else {
        QMessageBox msgBox(this); //'this' sets QMessageBox parent to QMainWindow (inherits style)
        msgBox.setWindowTitle("Exit");
        msgBox.setText("Save file before exit?");
        msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Yes);
        int ret = msgBox.exec();

        switch (ret) {
        case QMessageBox::Yes:
            MainWindow::on_actionSave_As_triggered();
            QApplication::quit();
            break;
        case QMessageBox::No:
            QApplication::quit();
            break;
        case QMessageBox::Cancel:
            break;
        default:
            break;
        }
    }
}

void MainWindow::on_actionCopy_triggered()
{
    ui->textEdit->copy();
}

void MainWindow::on_actionPaste_triggered()
{
    ui->textEdit->paste();
}

void MainWindow::on_actionCut_triggered()
{
    ui->textEdit->cut();
}

void MainWindow::on_actionUndo_triggered()
{
    ui->textEdit->undo();
}

void MainWindow::on_actionRedo_triggered()
{
    ui->textEdit->redo();
}

void MainWindow::on_actionQT_Notepad_triggered()  //tr() marks the string for translation
{
    QMessageBox::about(this, tr("About QT Notepad"),
    tr("<b>QT Notepad©</b><br> "
        "<br>"
        "QT Notepad by <b>Lusberg</b> (github.com/Lusberg)<br> "
        "macOS-iCons by <b>USBA</b> (github.com/USBA)<br> "
        "<br>"
        "Copyright 2018 Lusberg<br> "
        "<br>"
        "QT Notepad is distributed under the terms of the GNU LGPL v3"));
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (exitProcess()) {
        event->accept();
    } else {
        event->ignore();
    }
}

bool MainWindow::exitProcess()
{
    if (!ui->textEdit->document()->isModified() | ((!mod) & (!isWindowModified))) {
        return true;
    } else {
        QMessageBox msgBox(this); //'this' sets QMessageBox parent to QMainWindow (inherits style)
        msgBox.setWindowTitle("Exit");
        msgBox.setText("Save file before exit?");
        msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Yes);
        int ret = msgBox.exec();

        switch (ret) {
        case QMessageBox::Yes:
            MainWindow::on_actionSave_As_triggered();
            return true;
        case QMessageBox::No:
            return true;
        case QMessageBox::Cancel:
            return false;
        default:
            return true;
        }
    }
}

void MainWindow::documentWasModified()
{
    setWindowModified(ui->textEdit->document()->isModified());
    isWindowModified = true;
}
