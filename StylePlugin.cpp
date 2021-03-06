// Copyright 2001-2019 Crytek GmbH / Crytek Group. All rights reserved.
#include "StdAfx.h"
#include "StylePlugin.h"

#include <CryCore/Platform/platform_impl.inl>
#include <Commands/ICommandManager.h>
#include <QDir>
#include "CryString/CryPath.h"
#include "CrySerialization/Decorators/Resources.h"
#include "QFileDialog"
#include "QApplication"
#include "EditorStyleHelper.h"
#include <Serialization/Decorators/EditorActionButton.h>

#include <QFileSystemWatcher>
#include <QPixmapCache>
#include <QToolButton>
#include <QObject>
#include <QIconEngine>
#include <QtGui/private/qicon_p.h>
#include "QResource"
#include "IconPackManger.h"


CStylePreferences gStylePreferences;

static CStylePlugin* g_pStylePlugin = nullptr;


void OnStyleFileChanged() {
	g_pStylePlugin->ReloadStyle();
}
#pragma optimize( "", off )
CStylePlugin::CStylePlugin()
	: m_pPackManager(std::make_unique<IconPackManager>())
{
	g_pStylePlugin = this;

	m_pWatcher = std::make_unique<QFileSystemWatcher>();
	gStylePreferences.signalSettingsChanged.Connect(this, &CStylePlugin::OnStyleSettingsChanged);
	QObject::connect(&*m_pWatcher, &QFileSystemWatcher::fileChanged, OnStyleFileChanged);

}

#pragma optimize( "", on )
CStylePlugin::~CStylePlugin()
{
	g_pStylePlugin = nullptr;

}

void CStylePlugin::OnStyleSettingsChanged()
{
	auto& styleSheet = gStylePreferences.GetStyleSheetPath();

	if (!styleSheet.empty() && styleSheet != m_styleSheetPath)
	{
		m_pWatcher->removePath(m_styleSheetPath.c_str());
		m_styleSheetPath = styleSheet;
		m_pWatcher->addPath(m_styleSheetPath.c_str());
		ReloadStyle();
	}
}


void CStylePlugin::OnEditorNotifyEvent(EEditorNotifyEvent event)
{
	if (event == EEditorNotifyEvent::eNotify_OnInit)
	{
		OnStyleSettingsChanged();
	}
	else if (event == EEditorNotifyEvent::eNotify_OnMainFrameInitialized)
	{
		//Force a style reload
		OnStyleSettingsChanged();
		ReloadStyle();
		ReloadIcons();
	}
	else if (event == eNotify_OnQuit)
	{
		gStylePreferences.signalSettingsChanged.DisconnectObject(this);
	}
}

void CStylePlugin::ReloadStyle()
{
	if (m_styleSheetPath.empty())
		return;
	

	QFile styleSheetFile(m_styleSheetPath.c_str());

	if (styleSheetFile.open(QFile::ReadOnly))
	{
		qApp->setStyleSheet(styleSheetFile.readAll());
		qApp->setPalette(GetStyleHelper()->palette());
	}
}


void CStylePlugin::ReloadIcons()
{
	m_pPackManager->ReloadIcons();
}

REGISTER_PLUGIN(CStylePlugin);



constexpr char* g_styleSheetPath = "Editor/Styles/stylesheet.qss";
constexpr char* g_iconsFolder = "Editor/Icons/";

CStylePreferences::CStylePreferences()
	: SPreferencePage("Style Preferences", "General/Style")
	, m_stylePath(PathUtil::Make(PathUtil::GetEnginePath(), g_styleSheetPath))
{

}

bool CStylePreferences::Serialize(yasli::Archive& ar)
{
	ar(Serialization::MakeResourceSelector<>(m_stylePath, "Style"), "stylesheet", "Stylesheet");

	g_pStylePlugin->GetIconPackManager()->SerializePacks(ar);
	
	if (ar.isInput())
		signalSettingsChanged();

	return true;
}


REGISTER_PREFERENCES_PAGE_PTR(CStylePreferences, &gStylePreferences)

SResourceSelectionResult StyleSelector(const SResourceSelectorContext& context, const char* previousValue)
{
	SResourceSelectionResult result{ false, previousValue };

	// start in Sounds folder if no sound is selected
	string startPath = PathUtil::GetPathWithoutFilename(previousValue);
	if (startPath.empty())
		startPath = PathUtil::Make(PathUtil::GetEnginePath(), "Editor/Styles");

	auto fileName = QFileDialog::getOpenFileName(context.parentWidget, "Open Stylesheet", startPath.c_str(), "QT Stylesheets (*.qss)");

	if (!fileName.isEmpty())
	{
		result.selectedResource = fileName.toStdString().c_str();
		result.selectionAccepted = true;
	}

	return result;
}
REGISTER_RESOURCE_SELECTOR("Style", StyleSelector, "")