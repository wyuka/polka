#include "KMessageBox"

#include <QMessageBox>

void KMessageBox::error( QWidget *parent, 
         const QString &text, 
         const QString &caption, bool notify )
{
  Q_UNUSED( notify )

  QString cap = caption;

  if (cap.isEmpty()) {
    cap = "Error";
  }

  QMessageBox::critical( parent, cap, text );
}

void KMessageBox::information( QWidget *parent,
                          const QString &text,
                          const QString &caption,
                          const QString &,
                          bool )
{
  QString cap = caption;

  if (cap.isEmpty()) {
    cap = "Information";
  }

  QMessageBox::information( parent, cap, text );
}
