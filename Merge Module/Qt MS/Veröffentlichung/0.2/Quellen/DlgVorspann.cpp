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

#include "DlgVorspann.h"
#include "Syntaxhervorhebung.h"

using namespace QFrank;

QtSBSADlgVorspann::QtSBSADlgVorspann(QWidget *eltern):QDialog(eltern)
{
	setupUi(this);
	QtSBSASyntaxhervorhebung *Syntaxhervorhebung=new QtSBSASyntaxhervorhebung(txtVorspann->document());	
	QFile Vorspanntext(":/Vorspann.txt");
	Vorspanntext.open(QIODevice::ReadOnly);
	txtVorspann->setText(Vorspanntext.readAll().constData());
	Vorspanntext.close();
	setWindowFlags(windowFlags()^Qt::WindowContextHelpButtonHint);
	//Zentrieren
	QDesktopWidget *Desktop = QApplication::desktop(); //neue X und Y Koordinate
	int x=(Desktop->width()-this->width())/2;
	int y=(Desktop->height()-this->height())/2;
	//jetzt das Fenster verschieben
	this->move(x,y);
}
