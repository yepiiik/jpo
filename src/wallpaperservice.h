#ifndef WALLPAPERSERVICE_H
#define WALLPAPERSERVICE_H

#include <QString>

class WallpaperService
{
public:
    WallpaperService();
    bool setWallpaper(const QString &imagePath);
};

#endif // WALLPAPERSERVICE_H 