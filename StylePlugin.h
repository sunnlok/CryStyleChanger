// Copyright 2001-2019 Crytek GmbH / Crytek Group. All rights reserved.

#pragma once
#include "IPlugin.h"
#include <EditorFramework/Preferences.h>


class QFileSystemWatcher;


struct IconPack {

	bool Serialize(yasli::Archive& ar);

	bool operator==(const IconPack& other) const { return other.path == path; }

	string path;
};

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
private:

	string m_styleSheetPath;

	std::unique_ptr<QFileSystemWatcher> m_pWatcher;
	std::vector<IconPack>				m_iconPacks;
};


class CStylePreferences : public SPreferencePage
{
public:
	CStylePreferences();
	bool Serialize(yasli::Archive& ar);



	const string& GetStyleSheetPath() { return m_stylePath; }
	const std::vector<IconPack>& GetIconPacks() { return m_iconPacks; }

protected:
	string					m_stylePath;
	std::vector<IconPack>	m_iconPacks;
};