#include "KDialog"

#include <KPushButton>

#include <QBoxLayout>

KDialog::KDialog( QWidget *parent )
  : QDialog( parent )
{
  init();
}

KDialog::~KDialog()
{
}

void KDialog::init()
{
  mTopLayout = 0;

  mUser1Button = 0;
  mCloseButton = 0;
  mOkButton = 0;
  mApplyButton = 0;
  mCancelButton = 0;
}

void KDialog::setButtons( QFlags<KDialog::ButtonCode> buttonMask )
{
  if ( buttonMask & User1 ) {
    mUser1Button = new KPushButton( "User1", this );
    connect( mUser1Button, SIGNAL( clicked() ), SLOT( slotUser1() ) );
  } else {
    delete mUser1Button;
    mUser1Button = 0;
  }

  if ( buttonMask & Ok ) {
    mOkButton = new KPushButton( "Ok", this );
    connect( mOkButton, SIGNAL( clicked() ), SLOT( slotOk() ) );
  } else {
    delete mOkButton;
    mOkButton = 0;
  }

  if ( buttonMask & Apply ) {
    mApplyButton = new KPushButton( "Apply", this );
    connect( mApplyButton, SIGNAL( clicked() ), SLOT( slotApply() ) );
  } else {
    delete mApplyButton;
    mApplyButton = 0;
  }

  if ( buttonMask & Cancel ) {
    mCancelButton = new KPushButton( "Cancel", this );
    connect( mCancelButton, SIGNAL( clicked() ), SLOT( slotCancel() ) );
  } else {
    delete mCancelButton;
    mCancelButton = 0;
  }

  if ( buttonMask & Close ) {
    mCloseButton = new KPushButton( "Close", this );
    connect( mCloseButton, SIGNAL( clicked() ), SLOT( slotClose() ) );
  } else {
    delete mCloseButton;
    mCloseButton = 0;
  }
}

void KDialog::setMainWidget( QWidget *widget )
{
  delete mTopLayout;
  mTopLayout = new QVBoxLayout( this );
  
  mTopLayout->addWidget( widget );
  
  QBoxLayout *buttonLayout = new QHBoxLayout;
  mTopLayout->addLayout( buttonLayout );
  
  if ( mUser1Button ) buttonLayout->addWidget( mUser1Button );
  if ( mOkButton ) buttonLayout->addWidget( mOkButton );
  if ( mApplyButton ) buttonLayout->addWidget( mApplyButton );
  if ( mCancelButton ) buttonLayout->addWidget( mCancelButton );
  if ( mCloseButton ) buttonLayout->addWidget( mCloseButton );
}

void KDialog::setButtonText( ButtonCode id, const QString &text )
{
  KPushButton *b = button( id );
  if ( b ) {
    b->setText( text );
  }
}

KPushButton *KDialog::button( ButtonCode id ) const
{
  KPushButton *button = 0;
  switch ( id ) {
    case Ok:
      button = mOkButton;
      break;
    case Apply:
      button = mApplyButton;
      break;
    case User1:
      button = mUser1Button;
      break;
    case Cancel:
      button = mCancelButton;
      break;
    case Close:
      button = mCloseButton;
      break;
    default:
      break;      
  }
  return button;
}

void KDialog::slotOk()
{
  accept();
}

void KDialog::slotApply()
{
}

void KDialog::slotCancel()
{
  reject();
}

void KDialog::slotClose()
{
  accept();
}

void KDialog::slotUser1()
{
}
