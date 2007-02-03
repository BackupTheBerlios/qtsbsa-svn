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
#include "DlgProgramminfo.h"
#include "DlgEinstellungen.h"
#include "Parameter.h"
#include <QtGui>


QFrankQtSBSADlgHaupt::QFrankQtSBSADlgHaupt(QWidget *eltern) : QMainWindow(eltern)
{
	setupUi(this);
	//Zentrieren
	QDesktopWidget *Desktop = QApplication::desktop(); //neue X und Y Koordinate
	int x=(Desktop->width()-this->width())/2;
	int y=(Desktop->height()-this->height())/2;
	//jetzt das Fenster verschieben
	this->move(x,y);
	K_Parameter=new QFrankQtSBSAParameter(this);
}
void QFrankQtSBSADlgHaupt::on_Menue_Einstellungen_triggered()
{
	QFrankQtSBSADlgEinstellungen Einstellungen =new QFrankQtSBSADlgEinstellungen(this);
	Einstellungen.exec();
	if(Einstellungen.result()==QDialog::Accepted)
		qDebug()<<"ja";
	else
		qDebug()<<"nein";
}
void QFrankQtSBSADlgHaupt::on_Menue_UeberQt_triggered()
{
	QMessageBox::aboutQt(this);
}
void QFrankQtSBSADlgHaupt::on_Menue_UeberDasProgramm_triggered()
{
	QFrankQtSBSADlgInfo Infodialog=new QFrankQtSBSADlgInfo(this);
	Infodialog.exec();
}
void QFrankQtSBSADlgHaupt::on_sfBox_accepted()
{
	/*	
	K_Parameter->WindowsSDKPfadSetzen(txtWindowsSDKPfad->text());
	K_Parameter->QtPfadSetzen(txtQtPfad->text());
	K_Parameter->ZertSHA1Setzen(txtZertifikat->text().remove(':'));
	K_Parameter->CPUTypeSetzen(awProzessor->currentText());
	K_Parameter->ZielverzeichnisSetzen(txtZielPfad->text());
	K_Parameter->EntwicklerSetzen(txtEntwicklername->text());
	QFrankQtSBSADlgFortschritt Fortschritt(this,K_Parameter);
	Fortschritt.exec();
	*/
}
