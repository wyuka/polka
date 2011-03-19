#include "KInputDialog"

QString KInputDialog::getText( const QString &title, const QString &text,
  const QString &value, bool *ok )
{
  return QInputDialog::getText( 0, title, text, QLineEdit::Normal, value, ok );
}
 
