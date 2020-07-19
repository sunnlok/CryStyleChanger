// Copyright 2001-2019 Crytek GmbH / Crytek Group. All rights reserved.

#pragma once
#include "IPlugin.h"
#include <EditorFramework/Preferences.h>


class QFileSystemWatcher;
class IconPackManager;


class CStylePlugin : public IPlugin , public IAutoEditorNotifyListener
{
public:
	CStylePlugin();
	~CStylePlugin();

	int32       GetPluginVersion() { return 1; }
	const char* GetPluginName() { return "Style Changer"; }
	const char* GetPluginDescription() { return "Plugin used to change the sandbox style and icons"; }

	void OnStyleSettingsChanged();

	void OnEditorNotifyEvent(EEditorNotifyEvent event) override;

	void ReloadStyle();
	void ReloadIcons();



	IconPackManager* GetIconPackManager() const { return &*m_pPackManager; }
private:

	string m_styleSheetPath;

	std::unique_ptr<QFileSystemWatcher> m_pWatcher;

	std::unique_ptr<IconPackManager> m_pPackManager;
};


class CStylePreferences : public SPreferencePage
{
public:
	CStylePreferences();
	bool Serialize(yasli::Archive& ar);

	const string& GetStyleSheetPath() { return m_stylePath; }

protected:
	string					m_stylePath;
	

	

};