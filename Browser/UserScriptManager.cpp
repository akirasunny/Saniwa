#include "UserScriptManager.h"
#include <memory>
#include <QtCore/QFile>

void UserScriptManager::addUserScript(QWebEngineScriptCollection *collection) {
    std::shared_ptr<QWebEngineScript> script(new QWebEngineScript);
    script->setName(u8"ToukenResizer");
    //load js file from res
    QFile file(":/js/resizeframe.js");
    file.open(QIODevice::ReadOnly);
    QByteArray dump = file.readAll();
    file.close();
    //set source
    script->setSourceCode(QString::fromUtf8(dump));
    script->setRunsOnSubFrames(true);
    script->setWorldId(QWebEngineScript::ApplicationWorld);
    script->setInjectionPoint(QWebEngineScript::Deferred);
    collection->insert(*script);
}