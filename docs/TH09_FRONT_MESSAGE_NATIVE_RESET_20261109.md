# Front message Reset uses the actual constructed runtime

The original 23-byte `Reset` at 0x415C90 clears the complete 0x1D70-byte
Front message runtime, then sets its native +0x08 instruction index to -1.
Its previous standalone `FrontMessageRuntimeResetView` duplicated that full
allocation and concealed all members behind a byte buffer. The same operation
now belongs to the constructed `FrontMessageRuntimeView` used by Setup,
Update, Draw, Load, Release and the Front factory.

The three real callers—`LoadForSideCount`, `ReleaseForSideCount` and
`FrontAddedCallback`—now use the authentic member function, without casts
to an unimplemented source-only receiver. All existing original-call
addresses, calling conventions and memory operations are unchanged.

The pinned VC7.1 PE/COFF verifier freshly compiled **13 affected exact
functions / 9,775 physical bytes / 599 relocation fields** and found no
full-owner differences, including the **2,968-byte Update interpreter** and
the 706-byte FrontAdded callback. The real construction-only `/Ob0`
carrier still reproduces five complete original bodies. The 30 private
`$L` labels of Update were rebound only after proving their same-section
COFF values and original immutable target destinations.

The focused FrontAdded guard and 31 FrontMessage tests also pass. This
source-equivalent owner consolidation adds no new distinct authored exact
function and required no full cold replay.
