/**
 * @file        Adapter.cpp
 * @author      Adam Łyskawa
 *
 * @brief       Contains the instance of the appropriate file system adapter depending on the target configuration.
 * @remark      A part of the Woof Toolkit (WTK), File System API.
 *
 * @copyright   (c)2026 CodeDog, All rights reserved.
 *
 *                / \__
 *               (    @\___
 *               /         O
 *              /   (_____/
 *              /_____/   U
 */

#include "Adapter.hpp"

namespace FS
{

#if defined(USE_FILEX)
AdapterFILEX adapter;
#elif defined(USE_FATFS)
AdapterFATFS adapter;
#else
AdapterNull adapter;
#endif

}
