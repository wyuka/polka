#ifndef MICROKDE_KSTANDARDDIRS
#define MICROKDE_KSTANDARDDIRS

#include <QStringList>

class KStandardDirs
{
  public:
    static QString locate( const char *type, const QString& filename );
    static QString locateLocal( const char *type, const QString& filename );

    QStringList findAllResources( const QString &, const QString &, bool, bool);
    QString findResourceDir( const QString &, const QString & ); 

    static void setAppDir( const QString & );
    static QString appDir() { return mAppDir; }
  
  private:
    static QString mAppDir;
};

#endif
