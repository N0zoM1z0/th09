#include "FileSystem.hpp"
#include "SupervisorNetworkState.hpp"
#include "ZunMemory.hpp"

#include <stdlib.h>
#include <string.h>

static const char *ParseNetworkConfigValue(
    const char *cursor, int *remaining, const char *key, char *out)
{
    memset(out, 0, 0x100);

    if (strncmp(cursor, key, strlen(key)) == 0)
    {
        cursor += strlen(key);
        while (*cursor == ' ' || *cursor == '\t')
        {
            if (*remaining <= 0 || *cursor == '\n')
                return cursor;

            ++cursor;
            --*remaining;
        }

        if (*remaining > 0 && *cursor == '=')
        {
            ++cursor;
            --*remaining;

            while (*cursor == ' ' || *cursor == '\t')
            {
                if (*remaining <= 0)
                    return cursor;

                if (*cursor == '\n')
                {
                    *out = 0;
                    return cursor;
                }

                ++cursor;
                --*remaining;
            }

            if (*remaining > 0 && *cursor != ';')
            {
                int outIndex = 0;
                while (*cursor != ' ' && *cursor != '\t' &&
                       *cursor != '\n' && *cursor != '\r' &&
                       *cursor != ';' && *remaining > 0 &&
                       outIndex < 255)
                {
                    out[outIndex] = *cursor;
                    ++cursor;
                    --*remaining;
                    ++outIndex;
                }
            }
        }

        return cursor;
    }

    return 0;
}

void __fastcall PrepareTitleMode4Network(void *optionState)
{
    SupervisorNetworkState *state =
        reinterpret_cast<SupervisorNetworkState *>(optionState);
    char value[256];
    int fileSize;
    unsigned char *allocation;
    const char *cursor;
    const char *next;

    state->active = 0;
    if (!FileSystem::CheckIfFileAlreadyExists("IP.txt"))
        return;

    allocation = FileSystem::OpenFile("IP.txt", &fileSize, 1);
    memset(state->hostname, 0, sizeof(state->hostname));
    state->syncValue = 0;
    state->syncRate = 30;
    cursor = reinterpret_cast<const char *>(allocation);

    while (fileSize > 0)
    {
        char c = *cursor;
        if (c == ' ' || c == '\n' || c == '\t' || c == '\r')
            goto SKIP_CHARACTER;

        if (c == ';')
        {
        SKIP_COMMENT:
            if (fileSize <= 0)
                goto CLEANUP;

            ++cursor;
            --fileSize;
            if (*cursor != '\n')
                goto SKIP_COMMENT;
            continue;
        }

        next = ParseNetworkConfigValue(cursor, &fileSize, "IPAddress", value);
        if (next != 0)
        {
            cursor = next;
            strcpy(state->hostname, value);
        }
        else
        {
            next = ParseNetworkConfigValue(cursor, &fileSize, "Port", value);
            if (next != 0)
            {
                cursor = next;
                state->port = atol(value);
            }
            else
            {
                next = ParseNetworkConfigValue(cursor, &fileSize, "Sync", value);
                if (next != 0)
                {
                    cursor = next;
                    if (strcmp(value, "LAN") == 0)
                    {
                        state->syncValue = 1;
                        state->syncRate = 60;
                    }
                    else if (strcmp(value, "High") == 0)
                    {
                        state->syncValue = 0;
                        state->syncRate = 60;
                    }
                    else if (strcmp(value, "Low") == 0)
                    {
                        state->syncValue = 0;
                        state->syncRate = 30;
                    }
                    else if (strcmp(value, "Bad") == 0)
                    {
                        state->syncValue = 0;
                        state->syncRate = 20;
                    }
                }
                else
                {
                    next = ParseNetworkConfigValue(
                        cursor, &fileSize, "Side", value);
                    if (next == 0)
                        goto SKIP_CHARACTER;

                    cursor = next;
                    if (strcmp(value, "Auto") == 0)
                        state->joinMode = 2;
                    else if (strcmp(value, "1P") == 0)
                        state->joinMode = 0;
                    else if (strcmp(value, "2P") == 0)
                        state->joinMode = 1;
                }
            }
        }
        continue;

    SKIP_CHARACTER:
        ++cursor;
        --fileSize;
    }

CLEANUP:
    g_ZunMemory.Free(allocation);
}
