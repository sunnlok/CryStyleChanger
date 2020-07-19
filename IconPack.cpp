#include "StdAfx.h"
#include "IconPack.h"
#include <Serialization/Qt.h>
#include <QDir>
#include <CrySerialization/IArchiveHost.h>
#include "QResource"
#include <QFileInfo>



TIconPackPtr IconPack::LoadIconPack(QString& packPath)
{
	auto filePath = QDir::cleanPath(packPath);

	auto pack = std::make_unique<IconPack>(filePath);

	if (!Serialization::LoadJsonFile(*pack, filePath.toLocal8Bit().constData()))
		return nullptr;


	if (!pack->CleanAndValidate())
		return nullptr;


	pack->LoadResources();

	return pack;
}

IconPack::IconPack(QString& basePath)
	: m_basePath(QFileInfo(basePath).absolutePath())
{

}

IconPack::~IconPack()
{
	UnloadResources();
}

bool IconPack::CleanAndValidate()
{
	QDir basePath(m_basePath);
	for (auto iter = m_iconPackResources.begin(); iter != m_iconPackResources.end();)
	{
		QString relativePath = QDir::cleanPath(iter->filePath);

		QFileInfo testFile(basePath.absoluteFilePath(relativePath));
		if (testFile.exists()) {
			iter->filePath = testFile.absoluteFilePath();
			++iter;
			continue;
		}
			

		CryLogAlways("Could not open resource file: %s", iter->filePath.toStdString().c_str());

		iter = m_iconPackResources.erase(iter);
	}

	return (!m_iconPackResources.empty() || m_bIsIconFolder);
}


void IconPack::LoadResources()
{
	for (auto& pack : m_iconPackResources) {
		QResource::registerResource(pack.filePath);
	}
		
}

void IconPack::UnloadResources()
{
	for (auto& pack : m_iconPackResources) {

		QResource::unregisterResource(pack.filePath);
	}
		
}

bool IconPack::Serialize(Serialization::IArchive& ar)
{
	ar(m_bIsIconFolder, "isIconFolder", "IsIconFolder");
	ar(m_iconPackResources, "resources", "Resources");
	return true;
}

void IconPack::AddToSearchPath(QStringList& searhPaths)
{
	if (m_bIsIconFolder)
		searhPaths.append(m_basePath);

	for (auto &pack : m_iconPackResources)
		searhPaths.append(QString(':').append(pack.prefix));
}

bool IconPack::TryReplacePackPath(QString& path, const char* replaceWith)
{
	if (path.startsWith(":/"))
	{
		for (auto& pack : m_iconPackResources)
		{
			auto pathPrefix = QString(":").append(pack.prefix);

			if(!path.startsWith(pathPrefix))
				continue;

			path.replace(0, pathPrefix.length() + 1, replaceWith);
			return true;

		}
	}
	else if (m_bIsIconFolder && path.startsWith(m_basePath))
	{
		path.replace(0, m_basePath.length() + 1, replaceWith);

		return true;
	}

	return false;
}


bool IconPackResource::Serialize(Serialization::IArchive& ar)
{
	ar(prefix, "prefix", "Prefix");
	ar(filePath, "file", "File");

	return true;
}

