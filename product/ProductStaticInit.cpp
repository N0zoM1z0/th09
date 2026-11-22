// Product-closure glue for the trial whole-image link (not original source).
//
// The original image constructs its global objects through 25 compiler
// wrappers at 0x0048D7B0..0x0048DAD0, each emitted by the translation unit
// that defines the object. Until those definitions return to their own
// units, the objects' storage comes from the labelled data closure that
// scripts/product-data.py generates. This unit runs the same constructor
// calls in the original wrapper order and registers the same destructors.
//
// Only names, class keys and special-member signatures are declared here, so
// each call binds to the existing exact or product-only definitions without
// depending on any view layout beyond the three array element sizes. Not constructed: the ScreenEffect at
// 0x004AC888 and the sixteen PbgArchive entries at 0x004B42E0, which no
// reconstructed code references yet, and the three vertex arrays whose
// element constructor (0x004343D0) only returns its object.
#include <new>
#include <stdlib.h>

struct AsciiGameManagerView;
struct GameManagerResetView { void ResetForSupervisorAdded(); };
class ChainElem { public: ChainElem() throw(); ~ChainElem(); };
class Chain { public: Chain(); ~Chain(); };
struct ScoreFileView { ScoreFileView(); };
struct TitleScoreRecordView { unsigned char storage[0x2C]; };
struct ScoreRecordCtorView { ScoreRecordCtorView(); };
class ZunMemory { public: ZunMemory(); ~ZunMemory(); };
struct ReplayInputState { ReplayInputState(); unsigned char storage[0x8E]; };
class PbgArchive { public: PbgArchive(); ~PbgArchive(); };
class GameErrorContext { public: GameErrorContext(); };
struct GameWindow { GameWindow(); };
class Supervisor { public: Supervisor(); };
struct AnmVm { AnmVm(); unsigned char storage[0x2A4]; };
class AsciiManager;
struct NativeVmCompositeOwnerView { NativeVmCompositeOwnerView(); };
class SoundPlayer { public: SoundPlayer(); };

extern AsciiGameManagerView g_GameManager;
extern ChainElem g_GameManagerCalcChain;
extern ChainElem g_GameManagerDrawChain;
extern ScoreFileView g_ScoreFileHeader;
extern TitleScoreRecordView g_TitleScoreTable[16][5][5];
extern TitleScoreRecordView g_TitleScoreRecord;
extern Chain g_Chain;
extern ZunMemory g_ZunMemory;
extern ReplayInputState g_ReplayInputStates[3];
extern PbgArchive g_PbgArchive;
extern GameErrorContext g_GameErrorContext;
extern GameWindow g_GameWindow;
extern Supervisor g_Supervisor;
extern AnmVm g_SupervisorLoadingVms[3];
extern AsciiManager g_AsciiManager;
extern ChainElem g_AsciiManagerCalcChain;
extern ChainElem g_AsciiManagerDrawChainLowPrio;
extern ChainElem g_AsciiManagerDrawChainHighPrio;
extern SoundPlayer g_SoundPlayer;

static void __cdecl DestroyGameManagerCalcChain() { g_GameManagerCalcChain.~ChainElem(); }
static void __cdecl DestroyGameManagerDrawChain() { g_GameManagerDrawChain.~ChainElem(); }
static void __cdecl DestroyChain() { g_Chain.~Chain(); }
static void __cdecl DestroyZunMemory() { g_ZunMemory.~ZunMemory(); }
static void __cdecl DestroyPbgArchive() { g_PbgArchive.~PbgArchive(); }
static void __cdecl DestroyAsciiManagerCalcChain() { g_AsciiManagerCalcChain.~ChainElem(); }
static void __cdecl DestroyAsciiManagerDrawChainLowPrio() { g_AsciiManagerDrawChainLowPrio.~ChainElem(); }
static void __cdecl DestroyAsciiManagerDrawChainHighPrio() { g_AsciiManagerDrawChainHighPrio.~ChainElem(); }

static struct ProductStaticInit
{
    ProductStaticInit()
    {
        reinterpret_cast<GameManagerResetView *>(&g_GameManager)->ResetForSupervisorAdded();
        new (&g_GameManagerCalcChain) ChainElem;
        atexit(DestroyGameManagerCalcChain);
        new (&g_GameManagerDrawChain) ChainElem;
        atexit(DestroyGameManagerDrawChain);
        new (&g_ScoreFileHeader) ScoreFileView;
        for (int i = 0; i < 16 * 5 * 5; ++i)
            new (&g_TitleScoreTable[0][0][0] + i) ScoreRecordCtorView;
        new (&g_TitleScoreRecord) ScoreRecordCtorView;
        new (&g_Chain) Chain;
        atexit(DestroyChain);
        new (&g_ZunMemory) ZunMemory;
        atexit(DestroyZunMemory);
        for (int i = 0; i < 3; ++i)
            new (&g_ReplayInputStates[i]) ReplayInputState;
        new (&g_PbgArchive) PbgArchive;
        atexit(DestroyPbgArchive);
        new (&g_GameErrorContext) GameErrorContext;
        new (&g_GameWindow) GameWindow;
        new (&g_Supervisor) Supervisor;
        for (int i = 0; i < 3; ++i)
            new (&g_SupervisorLoadingVms[i]) AnmVm;
        new (&g_AsciiManager) NativeVmCompositeOwnerView;
        new (&g_AsciiManagerCalcChain) ChainElem;
        atexit(DestroyAsciiManagerCalcChain);
        new (&g_AsciiManagerDrawChainLowPrio) ChainElem;
        atexit(DestroyAsciiManagerDrawChainLowPrio);
        new (&g_AsciiManagerDrawChainHighPrio) ChainElem;
        atexit(DestroyAsciiManagerDrawChainHighPrio);
        new (&g_SoundPlayer) SoundPlayer;
    }
} s_productStaticInit;
