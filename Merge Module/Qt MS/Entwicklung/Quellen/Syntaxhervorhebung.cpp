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

#include "Syntaxhervorhebung.h"

using namespace QFrank;

QtSBSASyntaxhervorhebung::QtSBSASyntaxhervorhebung(QTextDocument *eltern):QSyntaxHighlighter(eltern)
{
	K_Blau=new QTextCharFormat();
	K_Blau->setForeground(Qt::blue);
	K_Gruen=new QTextCharFormat();
	K_Gruen->setForeground(Qt::darkGreen);
	K_Rot=new QTextCharFormat();
	//K_Rot->setForeground(Qt::darkRed);
	K_Rot->setForeground(Qt::red);
}
QtSBSASyntaxhervorhebung::~QtSBSASyntaxhervorhebung()
{
	delete K_Blau;
	delete K_Gruen;
	delete K_Rot;
}
void QtSBSASyntaxhervorhebung::highlightBlock(const QString &text)
{	
	QRegExp Ausdruck;
	Ausdruck.setPatternSyntax(QRegExp::RegExp2);
	int Position,Farbe;
	QString Muster;
	enum Farben{Blau=0x01,Gruen=0x02,Rot=0x03};
	QMultiMap<int,QString> Musterliste;
	
	Musterliste.insert(Blau,"^#[\\w]+");
	Musterliste.insert(Blau,"\\bif\\b");
	Musterliste.insert(Blau,"\\bbreak\\b");
	Musterliste.insert(Blau,"\\bwhile\\b");
	Musterliste.insert(Blau,"\\bint\\b");

	Musterliste.insert(Gruen,"//");
	
	Musterliste.insert(Rot,"\"[.\\\\\"\\w\\[\\],\\*\\-']+\"");
	//Musterliste.insert(Rot,"\".*[^\\\\]\"");
	Musterliste.insert(Rot,"'[^']+'");
	
	Q_FOREACH(Farbe,Musterliste.uniqueKeys())
	{
		Q_FOREACH(Muster,Musterliste.values(Farbe))
		{
			Ausdruck.setPattern(Muster);
			Position=0;
			while((Position=Ausdruck.indexIn(text,Position,QRegExp::CaretAtOffset))!=-1)
			{
				switch(Farbe)
				{
				case Blau:
							setFormat(Position,Ausdruck.matchedLength(),*K_Blau);
							break;
				case Gruen:
							setFormat(Position,text.length(),*K_Gruen);
							break;
				case Rot:
							setFormat(Position,Ausdruck.matchedLength(),*K_Rot);
							break;								
				}
				Position += Ausdruck.matchedLength();
			}
		}
	}
		
}
