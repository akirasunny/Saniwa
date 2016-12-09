#pragma once

#include <QWebEngineScript>
#include <QWebEngineScriptCollection>
class UserScriptManager {
public:
    static void addUserScript(QWebEngineScriptCollection* collection);
};