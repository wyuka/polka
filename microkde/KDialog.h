#ifndef MICROKDE_KDIALOG_H
#define MICROKDE_KDIALOG_H

#include <QDialog>

class KConfigGroup;
class KPushButton;
class QBoxLayout;

class KDialog : public QDialog {
    Q_OBJECT
  public:
    enum ButtonCode
    {
      None    = 0x00000000,
      Help    = 0x00000001, ///< Show Help button. (this button will run the help set with setHelp)
      Default = 0x00000002, ///< Show Default button.
      Ok      = 0x00000004, ///< Show Ok button. (this button accept()s the dialog; result set to QDialog::Accepted)
      Apply   = 0x00000008, ///< Show Apply button.
      Try     = 0x00000010, ///< Show Try button.
      Cancel  = 0x00000020, ///< Show Cancel-button. (this button reject()s the dialog; result set to QDialog::Rejected)
      Close   = 0x00000040, ///< Show Close-button. (this button closes the dialog)
      No      = 0x00000080, ///< Show No button. (this button closes the dialog and sets the result to KDialog::No)
      Yes     = 0x00000100, ///< Show Yes button. (this button closes the dialog and sets the result to KDialog::Yes)
      Reset   = 0x00000200, ///< Show Reset button
      Details = 0x00000400, ///< Show Details button. (this button will show the detail widget set with setDetailsWidget)
      User1   = 0x00001000, ///< Show User defined button 1.
      User2   = 0x00002000, ///< Show User defined button 2.
      User3   = 0x00004000, ///< Show User defined button 3.
      NoDefault = 0x00008000 ///< Used when specifying a default button; indicates that no button should be marked by default.
    };
    Q_DECLARE_FLAGS(ButtonCodes, ButtonCode)

    KDialog( QWidget *parent = 0 );
    virtual ~KDialog();

    void setCaption( const QString & ) {}

    void setButtons( QFlags<KDialog::ButtonCode> buttonMask );

    void setMainWidget( QWidget *widget );

    void restoreDialogSize( KConfigGroup * ) {}
    void saveDialogSize( const KConfigGroup & ) {}

    KPushButton *button( ButtonCode id ) const;

    void setButtonText( ButtonCode id, const QString & );

  protected slots:
    void slotOk();
    void slotApply();
    void slotCancel();
    void slotClose();
    void slotUser1();

  private:
    void init();

    QBoxLayout *mTopLayout;
    
    KPushButton *mUser1Button;
    KPushButton *mCloseButton;
    KPushButton *mOkButton;
    KPushButton *mApplyButton;
    KPushButton *mCancelButton;    
};

Q_DECLARE_OPERATORS_FOR_FLAGS(KDialog::ButtonCodes)

#endif
