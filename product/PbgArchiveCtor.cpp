#include "../src/PbgArchive.hpp"

// The global archive's constructor at 0x00433040 clears all four members.
PbgArchive::PbgArchive()
{
    entries = NULL;
    entryCount = 0;
    archiveFilename = NULL;
    file = NULL;
}
