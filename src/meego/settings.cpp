#include "settings.h"

Settings::Settings()
{
}

Settings::~Settings()
{
}

Settings *Settings::self()
{
  return new Settings();
}
