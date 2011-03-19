#include "KDialog"

#include <KPushButton>

KDialog::KDialog( QWidget *parent )
  : QDialog( parent )
{
}

void KDialog::setButtons( ButtonCodes buttonMask )
{
  Q_UNUSED( buttonMask )
}

void KDialog::setMainWidget( QWidget *widget )
{
  Q_UNUSED( widget )
}

void KDialog::setButtonText( ButtonCode id, const QString &text )
{
  Q_UNUSED( id )
  Q_UNUSED( text )
}

KPushButton* KDialog::button( ButtonCode id ) const
{
  Q_UNUSED( id )
  return new KPushButton;
}
