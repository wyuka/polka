/*
    This file is part of KDE.

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301,
    USA.
*/

#include "commenteditor.h"

#include <KLocale>

CommentEditor::CommentEditor( QWidget *parent )
  : KDialog( parent )
{
  setCaption( i18n("Add comment") );
  setButtons( User1 | Ok | Cancel );
  setModal( true );

  setButtonText( User1, i18n("Clear") );
  connect( this, SIGNAL( user1Clicked() ), SLOT( clearComment() ) );

  m_edit = new QTextEdit;
  setMainWidget( m_edit );

  m_edit->setFocus();
}

void CommentEditor::setComment( const QString &text )
{
  setCaption( i18n("Edit comment") );

  m_edit->setPlainText( text );
}

QString CommentEditor::comment() const
{
  return m_edit->toPlainText();
}

void CommentEditor::clearComment()
{
  m_edit->clear();
  m_edit->setFocus();
}
