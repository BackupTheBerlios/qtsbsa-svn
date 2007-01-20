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

#include "DlgFortschritt.h"
#include <QtGui>

QFrankQt4MergemoduleDlgFortschritt::QFrankQt4MergemoduleDlgFortschritt(QWidget *eltern) : QDialog(eltern)
{
	setupUi(this);
	setWindowFlags(windowFlags()^Qt::WindowSystemMenuHint);
	setWindowFlags(windowFlags()^Qt::WindowContextHelpButtonHint);
	QDesktopWidget *Desktop = QApplication::desktop(); //neue X und Y Koordinate
	int x=(Desktop->width()-this->width())/2;
	int y=(Desktop->height()-this->height())/2;
	//jetzt das Fenster verschieben
	this->move(x,y);
	K_darfGeschlossenWerden=false;	
}
void QFrankQt4MergemoduleDlgFortschritt::on_sfSchliessen_clicked()
{
	K_darfGeschlossenWerden=true;
	reject();
}
bool QFrankQt4MergemoduleDlgFortschritt::event(QEvent *ereignis)
{
	bool bearbeitet=false;
	if(ereignis->type()==QEvent::Close)
	{
		bearbeitet=true;
		if(K_darfGeschlossenWerden)
			ereignis->accept();
		else
			ereignis->ignore();
	}
	return bearbeitet;
}
