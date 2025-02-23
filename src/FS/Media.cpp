/**
 * @file        Media.cpp
 * @author      Adam Łyskawa
 *
 * @brief       Provides C++ types and services for the file system media.
 *              Defines C bindings for the file system media services.
 * @remark      A part of the Woof Toolkit (WTK), File System API
 *
 * @copyright	(c)2025 CodeDog, All rights reserved.
 */

#include "OS/AppThread.hpp"
#include "Media.hpp"
#include "FileSystem.hpp"
#include "Log.hpp"

#if defined(USE_FILEX)
#include "fx_api.h"
#include "fx_stm32_sd_driver.h"
#elif defined(USE_FATFS)
#include "fatfs.h"
#endif

void FS::MediaServices::registerType(MediaType mediaType, const char* root, MediaDriver driver)
{
    auto configuration = const_cast<MediaConfiguration*>(getConfiguration(mediaType));
    if (!configuration) configuration = const_cast<MediaConfiguration*>(getConfiguration(MediaType::NONE));
    configuration->type = mediaType;
    configuration->root = root;
    configuration->driver = driver;
}

const FS::MediaConfiguration *FS::MediaServices::getConfiguration(MediaType mediaType)
{
    for (const auto& c : configurations) if (c.type == mediaType) return &c;
    return nullptr;
}

const FS::MediaConfiguration *FS::MediaServices::getConfiguration(const char *root)
{
    for (const auto& c : configurations) if (strcmp(root, c.root) == 0) return &c;
    return nullptr;
}

bool FS::MediaServices::format(const char* root, MediaFormat format, const char *label)
{
#if defined(USE_FATFS)
    BYTE opt = 0;
    switch (format)
    {
    case MediaFormat::ExFAT:
        opt = FM_EXFAT;
        break;
    case MediaFormat::FAT32:
        opt = FM_FAT32;
        break;
    case MediaFormat::FAT16:
    case MediaFormat::FAT12:
        opt = FM_FAT;
        break;
    default:
        opt = FM_ANY;
        break;
    }
    size_t bufferSize = 2 * _MAX_SS;
    char buffer[2 * _MAX_SS];
    return f_mkfs(root, opt, 0, buffer, bufferSize) == FR_OK;
#elif defined(USE_FILEX_) // Not implemented yet, code below doesn't work.
    auto entry = const_cast<FileSystem*>(FileSystemTable::find(root));
    if (!entry) return false;
    auto config = getConfiguration(root);
    if (!config) return false;
    auto media = entry->media();
//    media->fx_media_driver_request = FX_DRIVER_INIT;
    void(*driver)(FX_MEDIA* media_ptr) = (void(*)(FX_MEDIA*))config->driver;
//    driver(media);
//    if (media->fx_media_driver_status != FX_SUCCESS) return false;
    ULONG totalSectors = media->fx_media_total_sectors;
    UINT bytesPerSector = media->fx_media_bytes_per_sector;
    UINT sectorsPerCluster = media->fx_media_sectors_per_cluster;
    UINT directoryEntries = media->fx_media_root_directory_entries;
    UINT heads = media->fx_media_heads;
    UINT sectorsPerTrack = media->fx_media_sectors_per_track;
    UINT volumeSerialNumber = 42;
    UINT boundaryUnit = 128;
//    fx_media_close(media);
    constexpr size_t bufferSize = FX_MAX_SECTOR_CACHE;
    char buffer[bufferSize];
    switch (format)
    {
    case MediaFormat::DEFAULT:
    case MediaFormat::ExFAT:
        return fx_media_exFAT_format(
            media,
            driver,
            nullptr,
            (UCHAR*)buffer,
            bufferSize,
            (char*)label,
            FX_NUMBER_OF_FATS,
            FX_HIDDEN_SECTORS,
            totalSectors,
            bytesPerSector,
            sectorsPerCluster,
            volumeSerialNumber,
            boundaryUnit
        ) == FX_SUCCESS;
        break;
    default:
        return fx_media_format(
            media,
            driver,
            nullptr,
            (UCHAR*)buffer,
            bufferSize,
            (char*)label,
            FX_NUMBER_OF_FATS,
            directoryEntries, // directory entries
            FX_HIDDEN_SECTORS,
            totalSectors,
            bytesPerSector,
            sectorsPerCluster,
            heads,
            sectorsPerTrack
        ) == FX_SUCCESS;
        break;
    }
#endif
    return false; // Not implemented yet.
}

bool FS::MediaServices::mount(Media &media, const char *root)
{
    auto entry = const_cast<FileSystem*>(FileSystemTable::find(root));
    if (!entry) // New entry.
    {
        entry = const_cast<FileSystem*>(FileSystemTable::add(root, &media));
        if (!entry) return false; // FS table full.
    }
    else if (entry->m_media) // Existing entry with media set.
    {
        if (entry->m_media == &media) return true; // Already mounted to this media.
        return false;
    }
    else entry->m_media = &media; // Existing entry without media set.
    bool status = false;
#ifdef USE_FATFS
    status = f_mount(&media, root, 0) == FR_OK;
#else
    status = true;
#endif
    notifyChanged();
    return status;
}

bool FS::MediaServices::umount(const char *root)
{
    auto entry = const_cast<FileSystem*>(FileSystemTable::find(root));
    if (!entry) return false; // FS root not found.
    entry->clear();
    notifyChanged();
    return true;
}

bool FS::MediaServices::umount(Media &media)
{
    auto entry = const_cast<FileSystem*>(FileSystemTable::find(&media));
    if (!entry) return false; // Media not found.
    entry->clear();
    notifyChanged();
    return true;
}

void FS::MediaServices::notifyChanged()
{   // ISR!
    if (mountNotify) OS::AppThread::sync(mountNotify);
}

#include "fs_bindings.h"

EXTERN_C_BEGIN

void fs_register_type(FS_MediaType mediaType, const char* root, FS_MediaDriver driver)
{
    FS::MediaServices::registerType(static_cast<FS::MediaType>(mediaType), root, driver);
}

int fs_format(const char* root, FS_MediaFormat format, const char* label)
{
    return FS::MediaServices::format(root, static_cast<FS::MediaFormat>(format), label);
}

int fs_mount(FS_Media* media, const char* root)
{
    return FS::MediaServices::mount(*media, root);
}

int fs_umount(const char* root)
{
    return FS::MediaServices::umount(root);
}

int fs_umount_media(FS_Media* media)
{
    return FS::MediaServices::umount(*media);
}

EXTERN_C_END
