/**
 * @file        AdapterNull.hpp
 * @author      Adam Łyskawa
 *
 * @brief       An empty file system adapter for no file system backend defined.
 * @remark      A part of the Woof Toolkit (WTK), File System API.
 *
 * @remarks     Used as a template for concrete file system adapters or as a template for a new file system adapter.
 *              Simulates Linux /dev/null device.
 *
 * @copyright	(c)2024 CodeDog, All rights reserved.
 */

#pragma once

#include "target.h"

#if !defined(USE_FILEX) && !defined(USE_FATFS)

#include "IAdapterMethods.hpp"

namespace FS
{

/// @brief A dummy adapter that can write anything just discarding the data it gets.
/// @remarks Obviously, all functions that involve reading fill always fail.
/// @remarks Can be used as a template for implementing non trivial adapters for different file system middlewares.
class AdapterNull final : public IAdapterMethods
{

public:

    /// @brief Finds the directory entry that matches the path.
    /// @param media Media structure reference.
    /// @param path File or directory path.
    /// @param entry Directory entry reference.
    /// @returns Status.
    Status find(Media& media, const char* path, DirectoryEntry& entry) const override;

    /// @brief Gets the file or directory creation time.
    /// @param media Media structure reference.
    /// @param path File or directory path.
    /// @param dateTime `DateTime` structure reference.
    /// @returns Status.
    Status created(Media& media, const char* path, DateTime& dateTime) const override;

    /// @brief Gets the file or directory last modification time.
    /// @param media Media structure reference.
    /// @param path File or directory path.
    /// @param dateTime `DateTime` structure reference.
    /// @returns Status.
    Status modified(Media& media, const char* path, DateTime& dateTime) const override;

    /// @brief Creates a file.
    /// @param media Media structure reference.
    /// @param path File path.
    /// @returns Status.
    Status fileCreate(Media& media, const char* path) const override;

    /// @brief Tests if a file exist on the media.
    /// @param media Media structure reference.
    /// @param path File path.
    /// @returns True if the file exists, false otherwise.
    Status fileExists(Media& media, const char* path) const override;

    /// @brief Opens a file.
    /// @param media Media structure reference.
    /// @param file File handle reference.
    /// @param path A path to the file relative to the file system root.
    /// @param mode File opening mode. Default opens existing file for reading.
    /// @returns Status.
    Status fileOpen(Media& media, FileControlBlock& file, const char* path, FileMode mode = FileMode::read) const override;

    /// @brief Moves the file pointer to the specified offset.
    /// @param file File handle reference.
    /// @param offset Position within the file.
    /// @returns Status.
    Status fileSeek(FileControlBlock& file, FileOffset offset) const override;

    /// @brief Reads data from a file.
    /// @param file File handle reference.
    /// @param buffer Buffer pointer.
    /// @param size Buffer size.
    /// @param bytesRead Number of bytes read variable reference.
    /// @returns Status.
    Status fileRead(FileControlBlock& file, void* buffer, size_t size, size_t& bytesRead) const override;

    /// @brief Writes data to a file.
    /// @param file File handle reference.
    /// @param buffer Buffer pointer.
    /// @param size Buffer size.
    /// @param bytesWritten Number of bytes written variable reference.
    /// @returns Status.
    Status fileWrite(FileControlBlock& file, const void* buffer, size_t size) const override;

    /// @brief Closes a file.
    /// @param file File handle reference.
    /// @returns Status.
    Status fileClose(FileControlBlock& file) const override;

    /// @brief Renames a file.
    /// @param media Media structure reference.
    /// @param oldName Old file name.
    /// @param newName New file name.
    /// @returns Status.
    Status fileRename(Media& media, const char* oldName, const char* newName) const override;

    /// @brief Deletes a file.
    /// @param media Media structure reference.
    /// @param path File name.
    /// @returns Status.
    Status fileDelete(Media& media, const char* path) const override;

    /// @brief Creates a directory on the media.
    /// @param media Media structure reference.
    /// @param path Directory name.
    /// @returns Status.
    Status directoryCreate(Media& media, const char* path) const override;

    /// @brief Tests if a directory exists on the media.
    /// @param media Media structure reference.
    /// @param path Directory name.
    /// @returns Status.
    Status directoryExists(Media& media, const char* path) const override;

    /// @brief Renames a directory on the media.
    /// @param media Media structure reference.
    /// @param oldName Old directory name.
    /// @param newName New directory name.
    /// @returns Status.
    Status directoryRename(Media& media, const char* oldName, const char* newName) const override;

    /// @brief Deletes a directory from the media.
    /// @param media Media structure reference.
    /// @param path Directory name.
    /// @returns Status.
    Status directoryDelete(Media& media, const char* path) const override;

};

}

#endif
