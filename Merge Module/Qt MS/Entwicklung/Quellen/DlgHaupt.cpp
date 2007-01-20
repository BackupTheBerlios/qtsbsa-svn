/* Copyright (C) 2007 Frank Büttner frank-buettner@gmx.net

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version
 2 of the License, or (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software Foundation,
 Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.*/

#include "DlgHaupt.h"
#include "DlgFortschritt.h"
#include <QtGui>

QFrankQt4MergemoduleDlgHaupt::QFrankQt4MergemoduleDlgHaupt(QWidget *eltern) : QMainWindow(eltern)
{
	setupUi(this);
	//Zentrieren
	QDesktopWidget *Desktop = QApplication::desktop(); //neue X und Y Koordinate
	int x=(Desktop->width()-this->width())/2;
	int y=(Desktop->height()-this->height())/2;
	//jetzt das Fenster verschieben
	this->move(x,y);
	K_Verzeichnisauswahl=new QFileDialog(this,Qt::Window); 
	K_Verzeichnisauswahl->setFileMode(QFileDialog::DirectoryOnly);
}
void QFrankQt4MergemoduleDlgHaupt::on_sfQtPfadSuchen_clicked()
{
	K_Verzeichnisauswahl->setAcceptMode(QFileDialog::AcceptOpen);
	K_Verzeichnisauswahl->setWindowTitle(trUtf8("Bitte das Verzeichnis mit den Qt4 Bibliotheken auswählen."));
	K_Verzeichnisauswahl->exec();
	if(K_Verzeichnisauswahl->result()==QDialog::Accepted)
		txtQtPfad->setText(K_Verzeichnisauswahl->selectedFiles().first());
}
void QFrankQt4MergemoduleDlgHaupt::on_sfWixPfadSuchen_clicked()
{
	K_Verzeichnisauswahl->setAcceptMode(QFileDialog::AcceptOpen);
	K_Verzeichnisauswahl->setWindowTitle(trUtf8("Bitte das Verzeichnis in dem sich Wix3 befindet auswählen."));
	K_Verzeichnisauswahl->exec();
	if(K_Verzeichnisauswahl->result()==QDialog::Accepted)
		txtWixPfad->setText(K_Verzeichnisauswahl->selectedFiles().first());
}
void QFrankQt4MergemoduleDlgHaupt::on_sfZielpfadSuchen_clicked()
{
	K_Verzeichnisauswahl->setAcceptMode(QFileDialog::AcceptSave);
	K_Verzeichnisauswahl->setWindowTitle(trUtf8("Bitte das Verzeichnis für die Mergemodule auswählen."));
	K_Verzeichnisauswahl->exec();
	if(K_Verzeichnisauswahl->result()==QDialog::Accepted)
		txtZielPfad->setText(K_Verzeichnisauswahl->selectedFiles().first());
}
void QFrankQt4MergemoduleDlgHaupt::on_sfBox_accepted()
{
	QFrankQt4MergemoduleDlgFortschritt Fortschritt(this);
	Fortschritt.exec();	
}
