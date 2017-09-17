#include "qmlchewing_plugin.h"
#include "chewing.h"

#include <qqml.h>

void QmlChewingPlugin::registerTypes(const char *uri)
{
    // @uri H.H.chewing
    qmlRegisterType<Chewing>(uri, 1, 0, "Chewing");
}

