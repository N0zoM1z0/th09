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

Modified files identify this provenance in their headers. Integration commits
also identify the upstream repository and revision. Original-byte agreement
is established separately with this repository's pinned TH09/VC7.1 Oracle;
attribution is not an exactness or original-source-authorship claim.
