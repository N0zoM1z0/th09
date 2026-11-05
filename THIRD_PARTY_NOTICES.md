# Cirno-Labs reference integration

This branch includes adaptations from [Cirno-Labs/th09](https://github.com/Cirno-Labs/th09),
revision `d7653152becabb8c86f2f43d49015ecc11bd0652`, licensed upstream under
**GPL-3.0-only**. Its license text is retained in
[LICENSES/GPL-3.0-only.txt](LICENSES/GPL-3.0-only.txt).

The combined reconstructed program on this branch is distributed under
GPL-3.0-only. The original portions remain available under their existing
MIT grant in [LICENSE](LICENSE); that grant does not relicense the GPL
adaptations. Original game files and assets are not included or licensed here.

| Adapted file | Upstream reference | Adaptation |
| --- | --- | --- |
| `src/TitleScreen.cpp` | `src/mainmenu.cpp`, `MainMenu::SelectEntryStep`, `SelectEntry2`, `SelectEntry3` | Three indexed character-cursor loops, retaining our independently reviewed types, methods and ABI |
| `src/SupervisorFrameQueueInsert.cpp` | `src/app.cpp`, `App::AddRecord` | Indexed native frame scan and shift loops, preserving the independently reviewed void ABI |
| `src/TitleScreenResultDraw.cpp` | `src/mainmenu.cpp`, `MainMenu::DrawState13` | Ranking/name-keyboard drawing with native score records and Ascii/GameManager identities |
| `src/TitleScreenOptions.cpp` | `src/mainmenu.cpp`, `MainMenu::UpdateState9` | Options state machine with native Supervisor/input/ANM/sound fields; uses our real scrolling-helper implementation |
| `src/ControllerInput.cpp` | `src/unsorted.cpp`, `ReadPad` | Native DirectInput/WinMM polling and axis expressions, retaining our shared input/configuration types |
| `src/SupervisorUpdate.cpp` | `src/app.cpp`, `App::OnUpdate` | Full native scene-transition callback, preserving all original switch tables and native callee identities |

| `src/FrontMessageRandomSetup.cpp` | `src/gui.cpp`, `StartRandom` | Native weighted message selection and indexed fallback loops |
| `src/FrontAddedCallback.cpp` | `src/gui.cpp`, `AddedCallback` | Native vertex and ANM initialization, using the reviewed AnmLoaded record |
| `src/TitleScreenReplaySaveDraw.cpp` | `src/mainmenu.cpp`, `DrawState15` | Native replay-slot and name-keyboard drawing |
| `src/ExAttackUpdateType3.cpp` | `src/pl02.cpp`, `UpdateExAttackType3` | Native history shifts, collision positions and VM state transitions |
| `src/SupervisorNetworkMessageHandler.cpp` | `src/network.cpp`, message handler | Full native packet dispatch, with target-backed unsigned 16-bit frame input ABI |
| `src/EclManager.cpp`, `src/ExAttackDrawType14Type22.inl` | `src/pl04.cpp`, `DrawExAttackTypes14And22` | Native fan rendering in the existing ECL carrier, retaining our real polar helper |

Modified files identify this provenance in their headers. Integration commits
also identify the upstream repository and revision. Original-byte agreement
is established separately with this repository's pinned TH09/VC7.1 Oracle;
attribution is not an exactness or original-source-authorship claim.
