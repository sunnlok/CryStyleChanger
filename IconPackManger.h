#pragma once
#include "IconPack.h"


struct PathWrapper {
	QString path;

	bool Serialize(Serialization::IArchive& ar);
};

class IconPackManager
{
public:



	bool SerializePacks(Serialization::IArchive& ar);
	void ReloadIcons();

	void ReloadPacks();
protected:


	void AddPacksToSearchPaths(std::vector<TIconPackPtr>& newPacks);
	void ApplyIconPacks(std::vector<TIconPackPtr>& newPacks);

	std::vector<TIconPackPtr> LoadPacksFromPaths(std::vector<PathWrapper>& paths);

	std::vector<TIconPackPtr>	m_iconPacks;
	std::vector<PathWrapper>	m_packPaths;

};