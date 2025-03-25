#include "wallpaperservice.h"
#include <windows.h>
#include <QFile>

WallpaperService::WallpaperService()
{
}

bool WallpaperService::setWallpaper(const QString &imagePath)
{
    if (!QFile::exists(imagePath)) {
        return false;
    }

    // Convert QString to wchar_t* for Windows API
    std::wstring wPath = imagePath.toStdWString();
    
    // Set wallpaper using Windows API
    int result = SystemParametersInfoW(
        SPI_SETDESKWALLPAPER,
        0,
        (void*)wPath.c_str(),
        SPIF_UPDATEINIFILE | SPIF_SENDCHANGE
    );

    return result != 0;
} 