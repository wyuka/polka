// This file is generated by kconfig_compiler from polka.kcfg.
// All changes you do to this file will be lost.
#ifndef SETTINGS_H
#define SETTINGS_H

#include <QtCore>

class Settings
{
  public:

    static Settings *self();
    ~Settings();

    /**
      Set Enable syncing with remote data repository
    */
    static
    void setRemoteSyncingEnabled( bool v )
    {
      Q_UNUSED( v );
    }

    /**
      Get Enable syncing with remote data repository
    */
    static
    bool remoteSyncingEnabled()
    {
      return false;
    }

    /**
      Set History of shown groups
    */
    static
    void setHistory( const QStringList & v )
    {
      Q_UNUSED( v )
    }

    /**
      Get History of shown groups
    */
    static
    QStringList history()
    {
      return QStringList();
    }

    /**
      Set If fancy mode is enabled
    */
    static
    void setFancyMode( bool v )
    {
      Q_UNUSED( v )
    }

    /**
      Get If fancy mode is enabled
    */
    static
    bool fancyMode()
    {
      return true;
    }

    /**
      Set Enable magic buttons
    */
    static
    void setEnableMagic( bool v )
    {
      Q_UNUSED( v )
    }

    /**
      Get Enable magic buttons
    */
    static
    bool enableMagic()
    {
      return true;
    }

    void readConfig()
    {
    }

    void writeConfig()
    {
    }

  protected:
    Settings();

    // Preferences
    bool mRemoteSyncingEnabled;

    // State
    QStringList mHistory;
    bool mFancyMode;

    // Hidden
    bool mEnableMagic;

  private:
};

#endif

