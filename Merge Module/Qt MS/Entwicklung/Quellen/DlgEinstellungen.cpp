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

#include "DlgEinstellungen.h"
#include <windows.h>
#include <Wincrypt.h>
#include <Cryptuiapi.h>

QFrankQtSBSADlgEinstellungen::QFrankQtSBSADlgEinstellungen(QWidget *eltern):QDialog(eltern)
{
	setupUi(this);
	//Eingabeüberprüfung
	txtEntwicklername->setValidator(new QRegExpValidator(QRegExp("[a-zA-Z0-9]+"),this)); //Nur die Zeichen A-Z,a-z oder 0-9 sind ok
	txtPublickeyToken->setValidator(new QRegExpValidator(QRegExp("[a-f0-9]{16}"),this)); //Nur a-f und 0-9 und das ganz müssen 16 Zeichen sein
	setWindowFlags(windowFlags()^Qt::WindowContextHelpButtonHint);
	//Zentrieren
	QDesktopWidget *Desktop = QApplication::desktop(); //neue X und Y Koordinate
	int x=(Desktop->width()-this->width())/2;
	int y=(Desktop->height()-this->height())/2;
	//jetzt das Fenster verschieben
	this->move(x,y);
	K_Verzeichnisauswahl=new QFileDialog(this,Qt::Window); 
	K_Verzeichnisauswahl->setFileMode(QFileDialog::DirectoryOnly);
}
void QFrankQtSBSADlgEinstellungen::on_sfBox_clicked(QAbstractButton *schaltflaeche)
{
	if(sfBox->buttonRole(schaltflaeche)==QDialogButtonBox::ApplyRole)
	{
		if(AlleAngabenVollstaendig())
			accept();
		else			
		QMessageBox::critical(this,trUtf8("Eingaben unvollständig"),trUtf8("Sie haben mindestes ein Feld nicht korrekt ausgefüllt."));
	}
	else
		reject();
}
bool QFrankQtSBSADlgEinstellungen::AlleAngabenVollstaendig()
{
	//Sind alle Felder richtig ausgefüllt?
	if(txtQtPfad->text().isEmpty() || txtWixPfad->text().isEmpty() || txtWindowsSDKPfad->text().isEmpty() || 
	   txtZielPfad->text().isEmpty() || !txtEntwicklername->hasAcceptableInput() || txtMergemodulAutor->text().isEmpty() ||
	   txtZertifikat->text().isEmpty() || !txtPublickeyToken->hasAcceptableInput())
		return false;
	return true;
}
void QFrankQtSBSADlgEinstellungen::on_sfQtPfadSuchen_clicked()
{
	K_Verzeichnisauswahl->setAcceptMode(QFileDialog::AcceptOpen);
	K_Verzeichnisauswahl->setWindowTitle(trUtf8("Bitte das Verzeichnis mit den Qt Bibliotheken auswählen."));
	K_Verzeichnisauswahl->exec();
	if(K_Verzeichnisauswahl->result()==QDialog::Accepted)
		txtQtPfad->setText(K_Verzeichnisauswahl->selectedFiles().first());
}
void QFrankQtSBSADlgEinstellungen::on_sfWixPfadSuchen_clicked()
{
	K_Verzeichnisauswahl->setAcceptMode(QFileDialog::AcceptOpen);
	K_Verzeichnisauswahl->setWindowTitle(trUtf8("Bitte das Verzeichnis in dem sich Wix3 befindet auswählen."));
	K_Verzeichnisauswahl->exec();
	if(K_Verzeichnisauswahl->result()==QDialog::Accepted)
		txtWixPfad->setText(K_Verzeichnisauswahl->selectedFiles().first());
}
void QFrankQtSBSADlgEinstellungen::on_sfWindowsSDKPfadSuchen_clicked()
{
	K_Verzeichnisauswahl->setAcceptMode(QFileDialog::AcceptOpen);
	K_Verzeichnisauswahl->setWindowTitle(tr("Werkzeuge vom Windows SDK suchen."));
	K_Verzeichnisauswahl->exec();
	if(K_Verzeichnisauswahl->result()==QDialog::Accepted)
		txtWindowsSDKPfad->setText(K_Verzeichnisauswahl->selectedFiles().first());
}
void QFrankQtSBSADlgEinstellungen::on_sfZielpfadSuchen_clicked()
{
	K_Verzeichnisauswahl->setAcceptMode(QFileDialog::AcceptSave);
	K_Verzeichnisauswahl->setWindowTitle(trUtf8("Bitte das Verzeichnis für die Mergemodule auswählen."));
	K_Verzeichnisauswahl->exec();
	if(K_Verzeichnisauswahl->result()==QDialog::Accepted)
		txtZielPfad->setText(K_Verzeichnisauswahl->selectedFiles().first());
}
void QFrankQtSBSADlgEinstellungen::on_sfZertifikatSuchen_clicked()
{
	HCERTSTORE Zertifikatsspeicher=NULL;
	PCCERT_CONTEXT Auswahldialog= NULL; 
	Zertifikatsspeicher=CertOpenStore(CERT_STORE_PROV_SYSTEM,0,NULL,CERT_SYSTEM_STORE_CURRENT_USER,(TCHAR*)QString("My").utf16());
	if(Zertifikatsspeicher==NULL)
	{
		QMessageBox::critical(this,trUtf8("Fehler beim öffnen des Zertifikatsspeichers."),trUtf8("Der Zertifikatsspeicher konnte nicht geöffnet werden."));
		return;
	}
	Auswahldialog=CryptUIDlgSelectCertificateFromStore(Zertifikatsspeicher,this->winId(),(TCHAR*)trUtf8("Bitte das Zertifikat zum signieren auswählen").utf16(),
														(TCHAR*)tr("Welches Zertifikat soll genutzt werden?").utf16(),CRYPTUI_SELECT_LOCATION_COLUMN,0,NULL);
	if(Auswahldialog==NULL)
	{
		QMessageBox::warning(this,tr("Fehler bei der Auswahl des Zertifikates."),trUtf8("Es wurde kein Zertifikat ausgewählt."));
		CertCloseStore(Zertifikatsspeicher,CERT_CLOSE_STORE_FORCE_FLAG);
		return;
	}
	//Für die Manifeste muss der Schlüssel min. 2048 Bits lang sein.
	ulong Schluessellaenge=CertGetPublicKeyLength(X509_ASN_ENCODING|PKCS_7_ASN_ENCODING,&Auswahldialog->pCertInfo->SubjectPublicKeyInfo);
	if(Schluessellaenge<2048)
	{
		QMessageBox::critical(this,tr("Fehler im Zertifikat"),trUtf8("Für Manifeste beträgt die minimale Schlüssellänge 2048 Bit.\r\n"
																	 "Bei dem gewählten Zertifikat beträgt diese jedoch nur %1 Bit").arg(Schluessellaenge));
		return;
	}
	else
	{		
		//SHA-1 hat 160Bit
		ulong LaengeDesFeldes=160/8;
		QByteArray Datenfeld;
		Datenfeld.resize(LaengeDesFeldes);
		if(!CertGetCertificateContextProperty(Auswahldialog,CERT_SHA1_HASH_PROP_ID,Datenfeld.data(),&LaengeDesFeldes))
		{
			QMessageBox::critical(this,tr("Fehler im Zertifikat"),trUtf8("Die SHA-1 Prüfsumme wurde nicht empfangen."));
		}
		else
		{
			txtZertifikat->setText(K_FeldNachHex(Datenfeld));/*
			QByteArray OeffentlicherSchluessel((char*)Auswahldialog->pCertInfo->SubjectPublicKeyInfo.PublicKey.pbData,
												Auswahldialog->pCertInfo->SubjectPublicKeyInfo.PublicKey.cbData);
	
			//So davon jetzte die SHA1 Prüfsumme
			HCRYPTPROV Cryptkontext;
			if(!CryptAcquireContext(&Cryptkontext,NULL,NULL,PROV_RSA_FULL,CRYPT_VERIFYCONTEXT))
			{
				QMessageBox::critical(this,tr("Fehler beim ermitteln des publickeyTokens"),trUtf8("Es konnte keine Verbindung zum Windows Verschlüsselungssystem"
																								  " aufgebaut werden."));
				txtZertifikat->setText("");		
			}
			else
			{
				HCRYPTHASH Cryptohash;
				if(!CryptCreateHash(Cryptkontext,CALG_SHA1,0,0,&Cryptohash))
				{
					QMessageBox::critical(this,tr("Fehler beim ermitteln des publickeyTokens"),tr("Es konnte kein SHA1 Objekt angefordert werden."));
					txtZertifikat->setText("");	
				}
				else
				{
					//So jetzte können wir eine SHA1 Prüfsumme erstellen
					if(!CryptHashData(Cryptohash,(BYTE*)OeffentlicherSchluessel.data(),OeffentlicherSchluessel.size(),0))
					{
						QMessageBox::critical(this,tr("Fehler beim ermitteln des publickeyTokens"),trUtf8("Der öffentliche Schlüssel konnte nicht an die"
																										  " Prüfsummenberechnung übergeben werden."));
						txtZertifikat->setText("");
					}
					else
					{
						//SHA1 erzeugen
						QByteArray Puffer;
						DWORD Puffergroesse;
						DWORD Parameterlaenge=sizeof(DWORD);
						//Puffergröße ermitteln
						if(!CryptGetHashParam(Cryptohash,HP_HASHSIZE,(BYTE*)&Puffergroesse,&Parameterlaenge,0))
						{
							QMessageBox::critical(this,tr("Fehler beim ermitteln des publickeyTokens"),
													trUtf8("Der Puffer für den publickeyToken konnte nicht erstellt werden."));
							txtZertifikat->setText("");
						}
						else
						{					
							Puffer.resize(Puffergroesse);
							if(!CryptGetHashParam(Cryptohash,HP_HASHVAL,(BYTE*)Puffer.data(),&Puffergroesse,0))
							{
								QMessageBox::critical(this,tr("Fehler beim ermitteln des publickeyTokens"),tr("Der publickeyToken konnte nicht abgeholt werden."));
								txtZertifikat->setText("");
							}
							else
							{	
								//Das geht noch nicht so wie es soll
								K_Parameter->publicKeyTokenSetzen(K_FeldNachHex(Puffer.right(8)).remove(':'));
								//K_Parameter->publicKeyTokenSetzen("c0677197e04ed00a");
								qDebug()<<K_FeldNachHex(OeffentlicherSchluessel);
								qDebug()<<"Soll c0677197e04ed00a";
								qDebug()<<"Ist:"<<qPrintable(K_Parameter->publicKeyTokenHohlen());						
							}					
						}
					}
					CryptDestroyHash(Cryptohash);
				}
			CryptReleaseContext(Cryptkontext,0);*/
		}	
	}	
	//alles wieder schließen
	CertFreeCertificateContext(Auswahldialog);
	CertCloseStore(Zertifikatsspeicher,CERT_CLOSE_STORE_FORCE_FLAG);	
}
QString QFrankQtSBSADlgEinstellungen::K_FeldNachHex(const QByteArray &feld)
{
	QString tmp="";
	uchar low,high;
	for(int x=0;x<feld.size();x++)
	{
		//Byte zerlegen
		high=((feld.at(x) & 0xf0) >>4)+0x30;
		low=(feld.at(x) & 0x0f)+0x30;
		if(high>0x39)
			high=high+0x07;
		if(low>0x39)
			low=low+0x07;
		tmp.append(high);
		tmp.append(low);
		tmp.append(":");
	}
	return tmp.left(tmp.size()-1);
}
