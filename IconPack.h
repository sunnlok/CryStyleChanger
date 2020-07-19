#pragma once
#include "QString"

class IconPack;

using TIconPackPtr = std::unique_ptr<IconPack>;

struct IconPackResource {
	QString prefix;
	QString filePath;

	bool Serialize(Serialization::IArchive& ar);
};

class IconPack
{
public:
	IconPack(QString& basePath);
	~IconPack();

	static TIconPackPtr LoadIconPack(QString& packPath);


	bool Serialize(Serialization::IArchive& ar);


	void AddToSearchPath(QStringList& searhPaths);

	bool TryReplacePackPath(QString& path, const char* replaceWith);

	void UnloadResources();
	void LoadResources();
protected:

	bool CleanAndValidate();
	
	

	QString m_basePath;


	bool						  m_bIsIconFolder = false;
	std::vector<IconPackResource> m_iconPackResources;
};
