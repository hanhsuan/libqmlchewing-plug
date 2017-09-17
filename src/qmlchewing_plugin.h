#ifndef QMLCHEWING_PLUGIN_H
#define QMLCHEWING_PLUGIN_H

#include <QQmlExtensionPlugin>

class QmlChewingPlugin : public QQmlExtensionPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QQmlExtensionInterface")

public:
    void registerTypes(const char *uri);
};

#endif // QMLCHEWING_PLUGIN_H
