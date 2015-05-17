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
	QString backuppath = QFileDialog::getOpenFileName(this, "LOAD emuNAND");
	if (backuppath == "")
		return;

	QString saveas = QFileDialog::getSaveFileName(this, "Select where to SAVE the redNAND backup", "redNAND.RND","All files (*)");
	if (saveas == "")
		return;

	QFile::remove(saveas);

	//runShellCommand("fallocate -l 512 \""+saveas+"\"");

	copy_file_skip(backuppath, saveas, 512);

	QMessageBox::information(this, "Convert to redNAND", "Done!");
}

void MainWindow::on_toEMU_clicked()
{
	QString backuppath = QFileDialog::getOpenFileName(this, "LOAD redNAND");
	if (backuppath == "")
		return;

	QString saveas = QFileDialog::getSaveFileName(this, "Select where to SAVE the emuNAND backup", "emuNAND.BIN","All files (*)");
	if (saveas == "")
		return;

	QFile::remove(saveas);

	copy_file_skip(backuppath, saveas, 512);

	QMessageBox::information(this, "Convert to emuNAND", "Done!");
}

void MainWindow::copy_file_skip(const QString &in, const QString &out, unsigned int skip)
{
	#define BLOCK_SIZE (8*1024)

	QFile fin(in);
	if (!fin.open(QIODevice::ReadOnly))
		return;

	QFile fout(out);
	if (!fout.open(QIODevice::WriteOnly))
		return;

	fout.seek(skip);

	char buffer[BLOCK_SIZE];
	qint64 nread = 0;

	while ((nread = fin.read(buffer, BLOCK_SIZE)) > 0) {
		fout.write(buffer, nread);
	}

	fin.close();
	fout.close();
}
