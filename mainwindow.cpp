/*
 * Copyright © 2015 nastys
 *
 * This file is part of redTools.
 * redTools is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * redTools is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with redTools.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QProcess>
#include <QMessageBox>
#include <QFileDialog>
#include <QFileInfo>
#include <QInputDialog>
#include <QDesktopServices>

QFile pv("/usr/bin/pv");

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_toRED_clicked()
{
    QString backuppath=QFileDialog::getOpenFileName(this, "LOAD emuNAND");
    if(backuppath=="")
        return;

    QString saveas=QFileDialog::getSaveFileName(this, "Select where to SAVE the redNAND backup", "redNAND.RND","All files (*)");
    if(saveas=="")
        return;

    runShellCommand("rm \""+saveas+"\"");

    runShellCommand("fallocate -l 512 \""+saveas+"\"");
    if(pv.exists())
        runShellCommand("dd if=\""+backuppath+"\" | pv | dd of=\""+saveas+"\" seek=1");
    else
        runShellCommand("dd if=\""+backuppath+"\" of=\""+saveas+"\" seek=1");

    QMessageBox::information(this, "Convert to redNAND", "Done!");
}

void MainWindow::runShellCommand(QString command)
{
    QFile xterm("/usr/bin/xterm");
    QProcess shell;
    if(xterm.exists())
        shell.start("xterm", QStringList() << "-e" << command);
    else
        shell.start("bash", QStringList() << "-c" << command);
    shell.waitForFinished(-1);
}

void MainWindow::on_toEMU_clicked()
{
    QString backuppath=QFileDialog::getOpenFileName(this, "LOAD redNAND");
    if(backuppath=="")
        return;

    QString saveas=QFileDialog::getSaveFileName(this, "Select where to SAVE the emuNAND backup", "emuNAND.BIN","All files (*)");
    if(saveas=="")
        return;

        runShellCommand("rm \""+saveas+"\"");

        if(pv.exists())
            runShellCommand("dd bs=512 skip=1 if=\""+backuppath+"\" | pv | dd of=\""+saveas+"\"");
        else
            runShellCommand("dd bs=512 skip=1 if=\""+backuppath+"\" of=\""+saveas+"\"");

        QMessageBox::information(this, "Convert to emuNAND", "Done!");
}
