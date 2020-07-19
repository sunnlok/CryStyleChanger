#include "StdAfx.h"
#include "IconPackManger.h"

#include <CryString/CryPath.h>

#include <QDir>
#include <QPixmapCache>
#include <QApplication>
#include <QWidget>
#include <QToolButton>
#include <QFileDialog>
#include <QtGui/private/qicon_p.h>

#include "CrySerialization/Decorators/Resources.h"
#include <Serialization/Decorators/EditorActionButton.h>

void IconPackManager::AddPacksToSearchPaths(std::vector<TIconPackPtr>& newPacks)
{
	QStringList newSearchPaths;
	for (auto& pack : newPacks)
		pack->AddToSearchPath(newSearchPaths);

	newSearchPaths.append(":/icons");

	QDir::setSearchPaths("icons", newSearchPaths);
}



void IconPackManager::ApplyIconPacks(std::vector<TIconPackPtr>& newPacks)
{
	AddPacksToSearchPaths(newPacks);
	ReloadIcons();
	m_iconPacks = std::move(newPacks);
	//It might be that we unregistered a file again that we loaded before, so reload
	for (auto& pPack : m_iconPacks)
		pPack->LoadResources();
}


template<typename T>
void PatchQtObjectIcon(T* pClass, std::vector<TIconPackPtr>& packs)
{
	QIcon icon = pClass->icon();

	if (icon.isNull() || !icon.data_ptr())
		return;


	QIconEngine* pEngine = static_cast<QIconPrivate*>(icon.data_ptr())->engine;


	if (!pEngine || pEngine->key() != QLatin1String("CryPixmapIconEngine"))
		return;

	CryPixmapIconEngine* pCryIconEngine = static_cast<CryPixmapIconEngine*>(pEngine);

	auto sizes = pCryIconEngine->availableSizes();
	QString iconName;
	if (!sizes.isEmpty())
	{
		bool bFound = false;
		for (auto& size : sizes)
		{
			for (auto mode = QIcon::Mode::Normal; mode <= QIcon::Mode::Selected; mode = (QIcon::Mode)(mode + 1))
			{

				for (auto state = QIcon::State::On; state <= QIcon::State::Off; state = (QIcon::State)(state + 1))
				{
					auto entry = pCryIconEngine->bestMatch(size, mode, state, false);
					if (!entry->fileName.isEmpty())
					{
						iconName = entry->fileName;
						bFound = true;
						break;
					}
				}
				if (bFound)
					break;
			}
			if (bFound)
				break;
		}		
	}


	if (iconName.isEmpty())
		return;

	const char* replaceWith = "icons:";

	if (!iconName.startsWith(":/icons/"))
	{
		for (auto& pPack : packs)
		{
			if (pPack->TryReplacePackPath(iconName, replaceWith))
				break;
		}
	}
	else
	{
		iconName.replace(":/icons/", "icons:");
	}

	pClass->setIcon(CryIcon(iconName));
}


void IconPackManager::ReloadIcons()
{
	QPixmapCache::clear();

	//Try to find everything that might have an icon and re-apply it
	auto widgets = qApp->allWidgets();

	for (QWidget* widget : widgets)
	{
		bool isButton = widget->inherits("QAbstractButton");
		if (isButton)
		{
			auto button = static_cast<QAbstractButton*>(widget);
			PatchQtObjectIcon(button, m_iconPacks);
		}

		auto actions = widget->actions();
		for (auto pAction : actions)
		{
			PatchQtObjectIcon(pAction, m_iconPacks);
		}
		//widget->repaint();
	}
}

void IconPackManager::ReloadPacks()
{
	for (auto& pPack : m_iconPacks)
		pPack->UnloadResources();

	auto packs = LoadPacksFromPaths(m_packPaths);
	ApplyIconPacks(packs);
}

std::vector<TIconPackPtr> IconPackManager::LoadPacksFromPaths(std::vector<PathWrapper>& paths)
{
	std::vector<TIconPackPtr> packs;
	packs.reserve(paths.size());

	for (auto& path : paths) {
		auto pack = IconPack::LoadIconPack(path.path);
		if (pack)
			packs.push_back(std::move(pack));
	}
		
	return packs;
}

bool IconPackManager::SerializePacks(Serialization::IArchive& ar)
{
	ar(m_packPaths, "iconPacks", "Icon Packs");

	if (ar.isInput())
	{
		ReloadPacks();
	}

	ar(Serialization::ActionButton([&] { ReloadPacks(); }), "realoadIcons", "Reload Icons");

	return true;
}

bool PathWrapper::Serialize(Serialization::IArchive& ar)
{
	string tmp = path.toLocal8Bit().data();
	ar(Serialization::MakeResourceSelector<>(tmp, "IconPack"), "iconPack", "Icon Pack");
	path = QString(tmp.c_str());

	return true;
}

SResourceSelectionResult IconPackSelector(const SResourceSelectorContext& context, const char* previousValue)
{
	SResourceSelectionResult result{ false, previousValue };

	// start in Sounds folder if no sound is selected
	string startPath = PathUtil::GetPathWithoutFilename(previousValue);
	if (startPath.empty())
		startPath = PathUtil::Make(PathUtil::GetEnginePath(), "Editor/Icons");

	auto fileName = QFileDialog::getOpenFileName(context.parentWidget, "Open Icon Pack", startPath.c_str(), "Sandbox Icon Pack (*.crysip)");

	if (!fileName.isEmpty())
	{
		result.selectedResource = fileName.toStdString().c_str();
		result.selectionAccepted = true;
	}

	return result;
}
REGISTER_RESOURCE_SELECTOR("IconPack", IconPackSelector, "")