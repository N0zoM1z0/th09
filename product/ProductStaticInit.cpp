// Native C++ global object definitions and static-initialization ordering for
// the Windows i386 product build.
//
// The original image constructs its global objects through 25 compiler
// wrappers at 0x0048D7B0..0x0048DAD0 in strict link-input order. Keeping
// every statically constructed global object in this single translation unit
// guarantees identical construction and atexit() teardown order across both
// normal and reversed link layouts while owning all global storage in C++.
#include "AnmVmLifecycle.hpp"
#include "Chain.hpp"
#include "GameErrorContext.hpp"
#include "GameWindow.hpp"
#include "PbgArchive.hpp"
#include "ReplayInputState.hpp"
#include "ScoreFileView.hpp"
#include "SoundPlayer.hpp"
#include "Supervisor.hpp"
#include "ZunMemory.hpp"
#include <new>

typedef char GameErrorContextSizeIs2008[(sizeof(GameErrorContext) == 0x2008) ? 1 : -1];
typedef char ReplayInputStateSizeIs8ECheck[(sizeof(ReplayInputState) == 0x8E) ? 1 : -1];
typedef char SupervisorSizeIs7B0Check[(sizeof(Supervisor) == 0x7B0) ? 1 : -1];
typedef char AnmVmSizeIs2A4Check[(sizeof(AnmVm) == 0x2A4) ? 1 : -1];
typedef char SoundPlayerSizeIs6224Check[(sizeof(SoundPlayer) == 0x6224) ? 1 : -1];

struct AsciiGameManagerView;
struct GameManagerResetView { void ResetForSupervisorAdded(); };
struct TitleScoreRecordView { unsigned char storage[0x2C]; };
struct ScoreRecordCtorView { ScoreRecordCtorView(); };
struct NativeVmCompositeOwnerView { NativeVmCompositeOwnerView(); };
class AsciiManager { public: unsigned char storage[0xE0AC]; };

extern AsciiGameManagerView g_GameManager;
extern TitleScoreRecordView g_TitleScoreTable[16][5][5];
extern TitleScoreRecordView g_TitleScoreRecord;

// 1. GameManager static reset (retail 0x0048D7B0).
static struct GameManagerStaticInit
{
    GameManagerStaticInit()
    {
        reinterpret_cast<GameManagerResetView *>(&g_GameManager)->ResetForSupervisorAdded();
    }
} s_gameManagerStaticInit;

// 2-4. GameManager chains and ScoreFile header.
ChainElem g_GameManagerCalcChain;
ChainElem g_GameManagerDrawChain;
ScoreFileView g_ScoreFileHeader;

// 5-6. Score table and score record (storage owned by src/ScoreData.cpp).
static struct ScoreRecordsStaticInit
{
    ScoreRecordsStaticInit()
    {
        for (int i = 0; i < 16 * 5 * 5; ++i)
            new (&g_TitleScoreTable[0][0][0] + i) ScoreRecordCtorView;
        new (&g_TitleScoreRecord) ScoreRecordCtorView;
    }
} s_scoreRecordsStaticInit;

// 7-15. Core runtime singletons and arrays in exact retail order.
Chain g_Chain;
ZunMemory g_ZunMemory;
ReplayInputState g_ReplayInputStates[3];
PbgArchive g_PbgArchive;
GameErrorContext g_GameErrorContext;
GameWindow g_GameWindow;
Supervisor g_Supervisor;
AnmVm g_SupervisorLoadingVms[3];

// 16. Auxiliary archives (retail 0x0048D920 / 0x0048DA90).
static PbgArchive s_auxiliaryArchives[20];

// 17. AsciiManager (constructed through the exact composite constructor at 0x00435DC0).
AsciiManager g_AsciiManager;
static struct AsciiManagerStaticInit
{
    AsciiManagerStaticInit()
    {
        new (&g_AsciiManager) NativeVmCompositeOwnerView;
    }
} s_asciiManagerStaticInit;

// 18-21. AsciiManager chains and SoundPlayer.
ChainElem g_AsciiManagerCalcChain;
ChainElem g_AsciiManagerDrawChainLowPrio;
ChainElem g_AsciiManagerDrawChainHighPrio;
SoundPlayer g_SoundPlayer;
