#pragma once

// The ANM slot shared by Etama startup/shutdown, its exact AddedCallback,
// and Effect resource loading. Kept in a narrow header to avoid disturbing
// unrelated VC7.1 optimizer-local table labels in the global ANM source view.
enum EtamaAnmResourceSlot
{
    ANM_FILE_SLOT_ETAMA = 8
};
