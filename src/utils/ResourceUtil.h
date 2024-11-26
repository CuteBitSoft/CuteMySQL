#pragma once
#include <wx/string.h>

class ResourceUtil
{
public:
	static wxString getResourceDir();
	static std::string getStdResourceDir();

	static wxString getResourcePath(const char * fileName);

	static wxString getProductBinDir();
	static std::string getStdProductBinDir();

	static wxString getProductImagesDir();
	static std::string getStdProductImagesDir();

	static wxString getProductUploadDir();
	static std::string getStdProductUploadDir();

	static wxString getProductTmpDir();
	static std::string getStdProductTmpDir();

	static wxString getDesktopDir();
};

