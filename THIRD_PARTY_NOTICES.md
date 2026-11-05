# Third-party notices

Portions are adapted from [Cirno-Labs/th09](https://github.com/Cirno-Labs/th09)
at `d7653152becabb8c86f2f43d49015ecc11bd0652` under
[GPL-3.0-only](LICENSES/GPL-3.0-only.txt). Original portions retain their
[MIT grant](LICENSE); the combined reconstructed program is GPL-3.0-only.
Original game files and assets are outside these licenses.

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
| `src/TitleScreenReplayMenuDraw.cpp` | `src/mainmenu.cpp`, `DrawState11` | Native SDK vector interpolation, shared formatting buffer and direct conditional arguments; retains the nonexact frontier |
| `src/TitleScreenReplaySaveDraw.cpp` | `src/mainmenu.cpp`, `DrawState15` | Native replay-slot and name-keyboard drawing |
| `src/ExAttackUpdateType3.cpp` | `src/pl02.cpp`, `UpdateExAttackType3` | Native history shifts, collision positions and VM state transitions |
| `src/PlayerChargeAttackUpdate.cpp` | `src/player.cpp`, `Player::UpdateShotInput` | Structured charge/shot branches and direct native timers, preserving the complete nonexact owner and reviewed ABI |
| `src/SupervisorNetworkMessageHandler.cpp` | `src/network.cpp`, message handler | Full native packet dispatch, with target-backed unsigned 16-bit frame input ABI |
| `src/EclManager.cpp`, `src/ExAttackDrawType14Type22.inl` | `src/pl04.cpp`, `DrawExAttackTypes14And22` | Native fan rendering in the existing ECL carrier, retaining our real polar helper |
| `src/EffectDrawType29.inl` | `src/pl09.cpp`, Type29 draw | Complete main/satellite fan source shape with native vertices and SDK local centers; reuses the existing polar primitive and remains nonexact |
| `src/EtamaControllerDraw.cpp` | `src/bullet.cpp`, `BulletManager::OnDraw` | Laser and bullet drawing with native VM/side fields and the SDK vector conversion, preserving both exact same-file siblings |

Source headers retain the upstream attribution and revision.
