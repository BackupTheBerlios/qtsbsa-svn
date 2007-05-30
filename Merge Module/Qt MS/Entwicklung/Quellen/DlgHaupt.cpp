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

using namespace QFrank;

QtSBSADlgHaupt::QtSBSADlgHaupt(QWidget *eltern) : QMainWindow(eltern)
{
	setupUi(this);
	//Zentrieren
	QDesktopWidget *Desktop = QApplication::desktop(); //neue X und Y Koordinate
	int x=(Desktop->width()-this->width())/2;
	int y=(Desktop->height()-this->height())/2;
	//jetzt das Fenster verschieben
	this->move(x,y);
	K_Parameter=new QtSBSAParameter(this);
	K_ParamterLaden();
}
void QtSBSADlgHaupt::K_ParamterLaden()
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
void QtSBSADlgHaupt::K_ParameterSpeichern()
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
void QtSBSADlgHaupt::on_Menue_Einstellungen_triggered()
{
	QtSBSADlgEinstellungen* Einstellungen =new QtSBSADlgEinstellungen(K_Parameter,this);
	Einstellungen->exec();
	if(Einstellungen->result()==QDialog::Accepted)
		K_ParameterSpeichern();		
}
void QtSBSADlgHaupt::on_Menue_UeberQt_triggered()
{
	QMessageBox::aboutQt(this);
}
void QtSBSADlgHaupt::on_Menue_UeberDasProgramm_triggered()
{
	QtSBSADlgInfo Infodialog=new QtSBSADlgInfo(this);
	Infodialog.exec();
}
void QtSBSADlgHaupt::on_Menue_VorspannFuerProgramme_triggered()
{
	QtSBSADlgVorspann Vorspann=new QtSBSADlgVorspann(this);
	Vorspann.exec();
}
bool QtSBSADlgHaupt::K_AlleParameterGesetzt()
{
	if(!K_Parameter->AllesVorliegend())
	{
		QMessageBox::critical(this,trUtf8("Unvollständige Parameter"),trUtf8("Es liegen nicht alle benötigen Parameter vor.\r\n"
																			 "Bitte legen Sie diese über das Menue Optionen->Einstellungen fest."));
		return false;
	}
	return true;
}
void QtSBSADlgHaupt::on_Menue_MergemoduleFuerQt_triggered()
{
	if(!K_AlleParameterGesetzt())
		return;
	QtSBSADlgFortschritt Fortschritt(this,K_Parameter);
	Fortschritt.exec();	
}
void QtSBSADlgHaupt::on_Menue_AnwendungAnpassen_triggered()
{
	if(!K_AlleParameterGesetzt())
		return;
	//DateiAuswahl
	QString Datei=QFileDialog::getOpenFileName(this,trUtf8("Bitte die Anwendung wählen, die bearbeitet werden soll."),QString(),
											   tr("Anwendungen (*.exe *.dll)"));
	if(Datei.isNull())
		return;
	//noch nicht fertig
	QMessageBox::information(this,tr("Noch nicht fertig"),tr("Diese Funktion ist noch nicht fertig:("));
}
