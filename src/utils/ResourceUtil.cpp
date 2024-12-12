#include "ResourceUtil.h"
#include <wx/stdpaths.h>

wxString ResourceUtil::getResourceDir()
{
	wxStandardPaths& path = wxStandardPaths::Get();
	path.UseAppInfo(wxStandardPaths::AppInfo_AppName);

	wxString result = wxPathOnly(path.GetExecutablePath());
	result.Append("/res");
	return result;
}

std::string ResourceUtil::getStdResourceDir()
{
	return ResourceUtil::getResourceDir().ToStdString();
}

wxString ResourceUtil::getResourcePath(const char* fileName)
{
	wxString path = getResourceDir();
	path.Append("/").Append(fileName);
	return path;
}

wxString ResourceUtil::getProductBinDir()
{
	wxStandardPaths& path = wxStandardPaths::Get();
	path.UseAppInfo(wxStandardPaths::AppInfo_AppName);

	return wxPathOnly(path.GetExecutablePath());
}

std::string ResourceUtil::getStdProductBinDir()
{
	return ResourceUtil::getProductBinDir().ToStdString();
}

wxString ResourceUtil::getProductImagesDir()
{
	wxString path = getResourceDir();
	path.Append("/images");
	return path;
}

std::string ResourceUtil::getStdProductImagesDir()
{
	return ResourceUtil::getProductImagesDir().ToStdString();
}

wxString ResourceUtil::getProductUploadDir()
{
	wxString path = getResourceDir();
	path.Append("/upload");
	return path;
}

std::string ResourceUtil::getStdProductUploadDir()
{
	return ResourceUtil::getProductUploadDir().ToStdString();
}

wxString ResourceUtil::getProductTmpDir()
{
	wxString path = getResourceDir();
	path.Append("/tmp");
	if (_access(path.c_str(), 0) != 0) {
		_mkdir(path.c_str());
	}
	return path;
}

std::string ResourceUtil::getStdProductTmpDir()
{
	return ResourceUtil::getProductTmpDir().ToStdString();
}
