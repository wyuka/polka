#include "KRandom"

#include <QUuid>

namespace KRandom {

QString randomString( int length )
{
  return QUuid::createUuid().toString().mid( length );
}

}
