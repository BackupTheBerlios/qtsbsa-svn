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
#include "DlgVorspann.h"
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
	K_ParamterLaden();
}
void QFrankQtSBSADlgHaupt::K_ParamterLaden()
{
	QSettings Programmparamter(QSettings::IniFormat,QSettings::UserScope,"QtSBSA","Parameter");
	K_Parameter->WindowsSDKPfadSetzen(Programmparamter.value("Verzeichnisse/WindowsSDK","").toString());
	K_Parameter->WixPfadSetzen(Programmparamter.value("Verzeichnisse/Wix","").toString());
	K_Parameter->QtPfadSetzen(Programmparamter.value("Verzeichnisse/Qt","").toString());
	K_Parameter->ZielverzeichnisSetzen(Programmparamter.value("Verzeichnisse/Mergemodule","").toString());

	K_Parameter->ManifestentwicklerSetzen(Programmparamter.value("Manifest/Entwickler","").toString());
	K_Parameter->CPUTypeSetzen(Programmparamter.value("Manifest/CPU","").toString());

	K_Parameter->MergemodulentwicklerSetzen(Programmparamter.value("Mergemodul/Entwickler","").toString());

	K_Parameter->ZertSHA1Setzen(Programmparamter.value("Zertifikat/SHA1","").toString());
	K_Parameter->publicKeyTokenSetzen(Programmparamter.value("Zertifikat/publickeyToken","").toString());
}
void QFrankQtSBSADlgHaupt::K_ParameterSpeichern()
{
	QSettings Programmparamter(QSettings::IniFormat,QSettings::UserScope,"QtSBSA","Parameter");
	Programmparamter.setValue("Verzeichnisse/WindowsSDK",K_Parameter->WindowsSDKPfadHohlen());
	Programmparamter.setValue("Verzeichnisse/Wix",K_Parameter->WixPfadHohlen());
	Programmparamter.setValue("Verzeichnisse/Qt",K_Parameter->QtPfadHohlen());
	Programmparamter.setValue("Verzeichnisse/Mergemodule",K_Parameter->ZielverzeichnisHohlen());

	Programmparamter.setValue("Manifest/Entwickler",K_Parameter->ManifestentwicklerHohlen());
	Programmparamter.setValue("Manifest/CPU",K_Parameter->CPUTypeHohlen());

	Programmparamter.setValue("Mergemodul/Entwickler",K_Parameter->MergemodulentwicklerHohlen());

	Programmparamter.setValue("Zertifikat/SHA1",K_Parameter->ZertSHA1Hohlen());
	Programmparamter.setValue("Zertifikat/publickeyToken",K_Parameter->publicKeyTokenHohlen());
}
void QFrankQtSBSADlgHaupt::on_Menue_Einstellungen_triggered()
{
	QFrankQtSBSADlgEinstellungen* Einstellungen =new QFrankQtSBSADlgEinstellungen(K_Parameter,this);
	Einstellungen->exec();
	if(Einstellungen->result()==QDialog::Accepted)
		K_ParameterSpeichern();		
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
void QFrankQtSBSADlgHaupt::on_Menue_VorspannFuerProgramme_triggered()
{
	QFrankQtSBSADlgVorspann Vorspann=new QFrankQtSBSADlgVorspann(this);
	Vorspann.exec();
}
/*void QFrankQtSBSADlgHaupt::on_sfBox_accepted()
{
	K_Parameter->WindowsSDKPfadSetzen(txtWindowsSDKPfad->text());
	K_Parameter->QtPfadSetzen(txtQtPfad->text());
	K_Parameter->ZertSHA1Setzen(txtZertifikat->text().remove(':'));
	K_Parameter->CPUTypeSetzen(awProzessor->currentText());
	K_Parameter->ZielverzeichnisSetzen(txtZielPfad->text());
	K_Parameter->EntwicklerSetzen(txtEntwicklername->text());
	QFrankQtSBSADlgFortschritt Fortschritt(this,K_Parameter);
	Fortschritt.exec();	
}*/
