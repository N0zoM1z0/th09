# Exact 996-byte Player damage calculation belongs to the native Player

TH09 v1.50a's complete 996-byte `CalcDamageToEnemy` at `0x41FCD0`
uses the actual Player's +0x303C8 timer, +0xC11C 128-element shot array,
+0xB100 active collision regions, header +0x98 animation resource,
side +0x0C Effect manager and side index +0x08. Previously its C++ body
was placed on a separate `PlayerDamagePlayerView` facade with duplicated
storage and declaration-only `HasTicked`, ANM and Effect methods.

The body now belongs to the real constructed `PlayerLifecycleView`, with
ordinary native timer, `AnmLoaded::SetAndExecuteScriptIdx` and
`EffectManager::SpawnEffectInFixedSlot` methods. The Enemy 3,900-byte
OnUpdate boss calls it directly twice rather than casting to a separate
Player object. The original callback ABI remains `thiscall` with two
`PlayerDamageFloat3 *` and three accumulator pointers. The shot and region
inner records retain separately reviewed compatibility projections in this
checkpoint; these can be consolidated without changing the Player receiver.

The original PE bytes were checked for all three changed internal call
symbols (timer +0x17, ANM +0x15A, Effect +0x1BF), and both Enemy boss
REL32 calls (+0x8BF/+0x8FF). No target destination changes. Pinned VC7.1
compilation reproduces the **complete 996/996 bytes, 17/17 relocated
fields** of this owner. The 3,900-byte Enemy boss still has exactly
**97 bound relocations and 13 unlike nonfield bytes**. All **85 exact
PlayerDamage/EclManager owners, 30,907 physical bytes and 1,410 bound
fields**, remain complete-original-exact. The ECL TU's 20 compiler-private
label changes were independently checked against the original PE targets.

Scoped Enemy/Player tests and native-owner tests pass. No new exact ledger
credit, full cold replay, native Windows link or runtime closure is claimed.
