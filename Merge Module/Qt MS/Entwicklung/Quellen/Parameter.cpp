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

#include "Parameter.h"

QFrankQt4MergemoduleParameter::QFrankQt4MergemoduleParameter(QObject* eltern):QObject(eltern)
{
}
void QFrankQt4MergemoduleParameter::ZertSHA1Setzen(const QString &sha1)
{
	K_ZertSHA1=sha1;
	K_ZertSHA1.remove(":"); //Trennzeichen entfernen
	K_publicKeyToken=K_ZertSHA1.right(16);//Die letzen 8 Bytes von der SHA-1 Prüfsumme ist der pkt.	
}
