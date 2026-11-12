#include "Ending.hpp"

#include "AnmManager.hpp"
#include "FileSystem.hpp"
#include "GameErrorContext.hpp"
#include "ScreenEffect.hpp"
#include "Supervisor.hpp"
#include "ZunMemory.hpp"

#include <new>
#include <stdlib.h>
#include <string.h>

extern Chain g_Chain;
extern ZunMemory g_ZunMemory;
extern GameErrorContext g_GameErrorContext;

struct AsciiInputView
{
    unsigned short IsHeld(unsigned short buttons);
    unsigned short WasPressed(unsigned short buttons);
};
extern AsciiInputView g_AsciiInput;

struct EndingAnmManagerView
{
    int LoadSurface(int surfaceIndex, const char *filename);
};

struct EndingAnmVmScriptView
{
    unsigned char unknown000[0x21A];
    short scriptIndex;
};
typedef char EndingAnmVmScriptIndexAt21A[
    (offsetof(EndingAnmVmScriptView, scriptIndex) == 0x21A) ? 1 : -1];

// Neutral reconstruction aliases for target globals whose original owners remain unknown.
extern int g_EndingLifecycleState;
extern unsigned int g_EndingLifecycleFlags;
extern int g_EndingActive;

// The original file-backed script cursor advances through native i386
// modulo-32 byte addresses, independently of C++ pointer-array bounds.
#define TH09_ENDING_NATIVE_ADVANCE_CURSOR(owner, amount) \
    (owner)->scriptCursor = reinterpret_cast<char *>( \
        reinterpret_cast<unsigned int>((owner)->scriptCursor) + \
        static_cast<unsigned int>(amount))

int Ending::ReadScriptParameter()
{
    int parameter = atol(this->scriptCursor);

    while (*this->scriptCursor != '\0')
        TH09_ENDING_NATIVE_ADVANCE_CURSOR(this, 1);
    while (*this->scriptCursor == '\0')
        TH09_ENDING_NATIVE_ADVANCE_CURSOR(this, 1);

    return parameter;
}

void Ending::DrawFade()
{
    ScreenEffectRect rect;
    rect.left = 0.0f;
    rect.top = 0.0f;
    rect.right = 640.0f;
    rect.bottom = 480.0f;

    if ((this->fadeColor & 0xFF000000) != 0)
        ScreenEffect::DrawSquare(&rect, this->fadeColor);
}

void Ending::UpdateFade()
{
    int alpha;

    switch (this->fadeMode)
    {
    case ENDING_FADE_IN_BLACK:
        if (this->fadeTimer >= this->fadeDuration)
        {
            this->fadeMode = ENDING_FADE_NONE;
            this->fadeColor = 0;
        }
        else
        {
            alpha = 255 - this->fadeTimer * 255 / this->fadeDuration;
            this->fadeColor = (unsigned int)alpha << 24;
            this->fadeTimer++;
        }
        break;

    case ENDING_FADE_OUT_BLACK:
        if (this->fadeTimer >= this->fadeDuration)
        {
            this->fadeColor = 0xFF000000;
        }
        else
        {
            alpha = this->fadeTimer * 255 / this->fadeDuration;
            this->fadeColor = (unsigned int)alpha << 24;
            this->fadeTimer++;
        }
        break;

    case ENDING_FADE_IN_WHITE:
        if (this->fadeTimer >= this->fadeDuration)
        {
            this->fadeMode = ENDING_FADE_NONE;
            this->fadeColor = 0;
        }
        else
        {
            alpha = this->fadeTimer * 255 / this->fadeDuration;
            this->fadeColor = (0xFFFFFFFFu - ((unsigned int)alpha << 24)) | 0x00FFFFFFu;
            this->fadeTimer++;
        }
        break;

    case ENDING_FADE_OUT_WHITE:
        if (this->fadeTimer >= this->fadeDuration)
        {
            this->fadeColor = 0xFFFFFFFFu;
        }
        else
        {
            alpha = this->fadeTimer * 255 / this->fadeDuration;
            this->fadeColor = ((unsigned int)alpha << 24) | 0x00FFFFFFu;
            this->fadeTimer++;
        }
        break;

    case ENDING_FADE_NONE:
        this->fadeColor = 0;
        break;
    }
}

// Original ending script VM selectors address one actual 0x2A4-byte AnmVm
// array. Script text bytes are written through the real 68-byte stack buffer.
// Preserve independent live wire reads and i386 modulo-32 address arithmetic.
#define TH09_ENDING_NATIVE_VM(owner, index) \
    (*reinterpret_cast<AnmVm *>( \
        reinterpret_cast<unsigned int>(&(owner)->endingVms[0]) + \
        static_cast<unsigned int>(index) * sizeof((owner)->endingVms[0])))
#define TH09_ENDING_NATIVE_TEXT(buffer, index) \
    (*reinterpret_cast<char *>( \
        reinterpret_cast<unsigned int>(&(buffer)[0]) + \
        static_cast<unsigned int>(index)))

int Ending::RunEndingScript()
{
    char textBuffer[68];
    float fadeTime;
    int temporary;
    int textIndex;

    memset(textBuffer, 0, sizeof(textBuffer));
    textIndex = 0;

    this->UpdateFade();

    if (this->pageWaitTimer > 0)
    {
        this->pageWaitTimer--;
        if (this->pageSkipLockFrames != 0)
        {
            this->pageSkipLockFrames--;
        }
        else if (g_AsciiInput.WasPressed(0x1001) ||
                 (this->hasSeenEnding && g_AsciiInput.IsHeld(0x100)))
        {
            this->pageWaitTimer = 0;
        }

        if (this->pageWaitTimer <= 0)
        {
            for (int i = 0; i < 15; i++)
                TH09_ENDING_NATIVE_VM(this, i).SetInterrupt(2);
            this->nextTextVmIndex = 0;
        }
        else
        {
            goto end_of_parse;
        }
    }

    if (this->lineWaitTimer > 0)
    {
        this->lineWaitTimer--;
        if (this->lineSkipLockFrames != 0)
        {
            this->lineSkipLockFrames--;
        }
        else if (g_AsciiInput.WasPressed(0x1001) ||
                 (this->hasSeenEnding && g_AsciiInput.IsHeld(0x100)))
        {
            this->lineWaitTimer = 0;
        }
        goto end_of_parse;
    }

    while (true)
    {
        switch (*this->scriptCursor)
        {
        case '@':
            TH09_ENDING_NATIVE_ADVANCE_CURSOR(this, 1);
            switch (*this->scriptCursor)
            {
            case 'b':
                if (reinterpret_cast<EndingAnmManagerView *>(g_AnmManager)->LoadSurface(
                        0, this->scriptCursor + 1) != 0)
                    return -1;
                break;

            case 'a':
            {
                TH09_ENDING_NATIVE_ADVANCE_CURSOR(this, 1);
                int vmIndex = this->ReadScriptParameter();
                int scriptIndex = this->ReadScriptParameter();
                int spriteIndex = this->ReadScriptParameter();
                this->endingAnm->ExecuteAnmIdx(&TH09_ENDING_NATIVE_VM(this, vmIndex), scriptIndex);
                this->endingAnm->SetSprite(&TH09_ENDING_NATIVE_VM(this, vmIndex), spriteIndex);
                break;
            }

            case 'V':
            {
                TH09_ENDING_NATIVE_ADVANCE_CURSOR(this, 1);
                int numerator = this->ReadScriptParameter();
                int denominator = this->ReadScriptParameter();
                this->backgroundScrollSpeed = (float)numerator / (float)denominator;
                break;
            }

            case 'v':
                TH09_ENDING_NATIVE_ADVANCE_CURSOR(this, 1);
                this->backgroundPosY = (float)this->ReadScriptParameter();
                break;

            case 'F':
                g_AnmManager->ReleaseAnm(19);
                this->endingAnm = g_AnmManager->LoadAnm(19, "staff01.anm");
                if (this->LoadEndingScript(this->scriptCursor + 1) != 0)
                    return -1;
                textIndex = 0;
                // The target shares the reset tail with @R.
            case 'R':
                for (int i = 0; i < 16; i++)
                {
                    reinterpret_cast<EndingAnmVmScriptView *>(
                        &TH09_ENDING_NATIVE_VM(this, i))->scriptIndex = 0;
                }
                break;

            case 'm':
                g_Supervisor.StopAudio();
                g_Supervisor.LoadMusic(*(this->scriptCursor + 1));
                g_Supervisor.PlayMusic(*(this->scriptCursor + 1), 0);
                break;

            case 'M':
                TH09_ENDING_NATIVE_ADVANCE_CURSOR(this, 1);
                fadeTime = (float)this->ReadScriptParameter();
                g_Supervisor.FadeOutMusic(fadeTime);
                break;

            case 's':
                TH09_ENDING_NATIVE_ADVANCE_CURSOR(this, 1);
                this->defaultLineWaitFrames = this->ReadScriptParameter();
                this->minimumLineWaitFrames = this->ReadScriptParameter();
                break;

            case 'c':
                TH09_ENDING_NATIVE_ADVANCE_CURSOR(this, 1);
                this->textColor = this->ReadScriptParameter();
                break;

            case '0':
                this->fadeMode = ENDING_FADE_IN_BLACK;
                goto set_fade;
            case '1':
                this->fadeMode = ENDING_FADE_OUT_BLACK;
                goto set_fade;
            case '2':
                this->fadeMode = ENDING_FADE_IN_WHITE;
                goto set_fade;
            case '3':
                this->fadeMode = ENDING_FADE_OUT_WHITE;
            set_fade:
                this->fadeTimer = 0;
                TH09_ENDING_NATIVE_ADVANCE_CURSOR(this, 1);
                this->fadeDuration = this->ReadScriptParameter() / 2;
                this->UpdateFade();
                break;

            case 'r':
                TH09_ENDING_NATIVE_ADVANCE_CURSOR(this, 1);
                this->pageWaitTimer = this->ReadScriptParameter();
                this->pageSkipLockFrames = this->ReadScriptParameter();
                this->lineWaitTimer = 0;
                this->lineSkipLockFrames = 0;
                while (*this->scriptCursor != '\n' && *this->scriptCursor != '\r')
                    TH09_ENDING_NATIVE_ADVANCE_CURSOR(this, 1);
                while (*this->scriptCursor == '\n' || *this->scriptCursor == '\r')
                    TH09_ENDING_NATIVE_ADVANCE_CURSOR(this, 1);
                goto end_of_parse;

            case 'w':
                TH09_ENDING_NATIVE_ADVANCE_CURSOR(this, 1);
                this->lineWaitTimer = this->ReadScriptParameter();
                this->lineSkipLockFrames = this->ReadScriptParameter();
                while (*this->scriptCursor != '\n' && *this->scriptCursor != '\r')
                    TH09_ENDING_NATIVE_ADVANCE_CURSOR(this, 1);
                while (*this->scriptCursor == '\n' || *this->scriptCursor == '\r')
                    TH09_ENDING_NATIVE_ADVANCE_CURSOR(this, 1);
                goto end_of_parse;

            case 'z':
                return -1;
            }

            while (*this->scriptCursor != '\n' && *this->scriptCursor != '\r')
                TH09_ENDING_NATIVE_ADVANCE_CURSOR(this, 1);
            while (*this->scriptCursor == '\n' || *this->scriptCursor == '\r')
                TH09_ENDING_NATIVE_ADVANCE_CURSOR(this, 1);
            break;

        case '\0':
        case '\n':
        case '\r':
            if (textIndex != 0)
            {
                g_AnmManager->DrawTextLeft(
                    &TH09_ENDING_NATIVE_VM(this, this->nextTextVmIndex), this->textColor, 0,
                    textBuffer);
                TH09_ENDING_NATIVE_VM(this, this->nextTextVmIndex).SetInterrupt(1);
            }
            while (*this->scriptCursor == '\n' || *this->scriptCursor == '\0' ||
                   *this->scriptCursor == '\r')
            {
                TH09_ENDING_NATIVE_ADVANCE_CURSOR(this, 1);
            }
            if (g_AsciiInput.IsHeld(0x1001))
            {
                this->lineWaitTimer = this->minimumLineWaitFrames;
                this->lineSkipLockFrames = this->minimumLineWaitFrames;
                goto lineWaitConfigured;
            }

            this->lineWaitTimer = this->defaultLineWaitFrames;
            this->lineSkipLockFrames = this->minimumLineWaitFrames;

lineWaitConfigured:
            this->nextTextVmIndex++;
            goto end_of_parse;

        default:
            temporary = textIndex;
            TH09_ENDING_NATIVE_TEXT(textBuffer, textIndex) = *this->scriptCursor;
            TH09_ENDING_NATIVE_TEXT(textBuffer, static_cast<unsigned int>(temporary) + 1u) = *(this->scriptCursor + 1);
            textIndex = temporary + 2;
            TH09_ENDING_NATIVE_ADVANCE_CURSOR(this, 2);
            break;
        }
    }

end_of_parse:
    this->elapsedTimer++;
    this->backgroundPosY -= this->backgroundScrollSpeed;
    if (this->backgroundPosY <= 0.0f)
    {
        this->backgroundPosY = 0.0f;
        this->backgroundScrollSpeed = 0.0f;
    }
    return 0;
}

int Ending::LoadEndingScript(const char *path)
{
    unsigned char *previousScript = this->scriptData;
    this->scriptData = FileSystem::OpenFile(path, NULL, 0);

    if (this->scriptData == NULL)
    {
        this->scriptData = previousScript;
        g_GameErrorContext.Log("error : \x83\x47\x83\x93\x83\x66\x83\x42\x83\x93\x83\x4F\x83\x74\x83\x40\x83\x43\x83\x8B\x82\xAA\x93\xC7\x82\xDD\x8D\x9E\x82\xDF\x82\xC8\x82\xA2\x81\x41\x83\x74\x83\x40\x83\x43\x83\x8B\x82\xAA\x94\x6A\x89\xF3\x82\xB3\x82\xEA\x82\xC4\x82\xA2\x82\xDC\x82\xB7\r\n");
        return -1;
    }

    this->scriptCursor = reinterpret_cast<char *>(this->scriptData);
    this->defaultLineWaitFrames = 8;
    this->lineWaitTimer = 0;
    this->elapsedTimer = 0;

    if (previousScript != NULL)
        g_ZunMemory.Free(previousScript);
    return 0;
}

Ending::Ending()
{
    memset(this, 0, sizeof(Ending));
    this->defaultLineWaitFrames = 8;
    this->lineWaitTimer = 0;
    this->elapsedTimer = 0;
    this->backgroundPosX = 0.0f;
    this->backgroundPosY = 0.0f;
    this->backgroundScrollSpeed = 0.0f;
}

int Ending::OnUpdate(Ending *ending)
{
    int frameSkip = 0;

loop:
    if (ending->RunEndingScript() != 0)
        return 0;

    for (int i = 0; i < 15; i++)
        g_AnmManager->ExecuteScript(&TH09_ENDING_NATIVE_VM(ending, i));

    if (ending->hasSeenEnding && g_AsciiInput.IsHeld(0x100) && frameSkip < 8)
    {
        frameSkip++;
        goto loop;
    }
    return 1;
}

int Ending::OnDraw(Ending *ending)
{
    g_AnmManager->CopySurfaceToBackbuffer2(
        0, 0, 0, (int)ending->backgroundPosX, (int)ending->backgroundPosY, 640, 480);
    for (int i = 0; i < 15; i++)
        g_AnmManager->Draw2D(&TH09_ENDING_NATIVE_VM(ending, i));
    ending->DrawFade();
    return 1;
}

int Ending::DeletedCallback(Ending *ending)
{
    g_AnmManager->ReleaseAnm(19);
    g_EndingLifecycleState = 6;
    g_AnmManager->ReleaseSurface(0);
    g_ZunMemory.Free(ending->scriptData);
    g_Chain.Cut(ending->drawChain);
    ending->drawChain = NULL;
    operator delete(ending);
    g_EndingLifecycleFlags &= ~0x400u;
    g_EndingActive = 0;
    return 0;
}

int Ending::RegisterChain()
{
    Ending *ending = static_cast<Ending *>(
        g_ZunMemory.AddToRegistry(new Ending, sizeof(Ending), "EndingInf"));

    ending->calcChain = g_Chain.CreateElem((ChainCallback)Ending::OnUpdate);
    ending->calcChain->arg = ending;
    ending->calcChain->addedCallback = (ChainLifetimeCallback)Ending::AddedCallback;
    ending->calcChain->deletedCallback = (ChainLifetimeCallback)Ending::DeletedCallback;
    if (g_Chain.AddToCalcChain(ending->calcChain, 5) != 0)
        return -1;

    ending->drawChain = g_Chain.CreateElem((ChainCallback)Ending::OnDraw);
    ending->drawChain->arg = ending;
    g_Chain.AddToDrawChain(ending->drawChain, 4);
    return 0;
}

#undef TH09_ENDING_NATIVE_VM
#undef TH09_ENDING_NATIVE_TEXT
