# Front factory reset is now a genuine member of the constructed owner

The original TH09 v1.50a `Front::Reset` at `0x415D40` is a 14-byte
function that zeroes exactly **0x1293C bytes**, the full allocation
constructed by `CreateGameSubsystem` (0x418840). Earlier source declared
a separate anonymous `Front` byte array solely to call `Reset()` through a
reinterpret cast. There was no second allocation in the original executable.

The source now defines `FrontFactoryConstructView::Reset()` directly on the
full native factory type, zeroing `sizeof(*this)`; the factory invokes that
real member. The remaining `FrontSupport.hpp` fake whole-object type and
unneeded Front Calc/Added includes have been removed. The original factory
continues to construct the object, register the `FrontInf` allocation,
reset all native fields, and build calc/draw callback chains in the same
order. This is target-specific memory behavior on a historically constructed
object; it is **not** a claim that `memset` of nontrivial members is portable
C++ on other compilers.

| Complete original owner | Bytes | Relocations | Result |
| --- | ---: | ---: | --- |
| Native `FrontFactoryConstructView::Reset` | 14/14 | 0 | exact |
| `CreateGameSubsystem` | 220/220 | 22 | exact |
| Front Added callback | 706/706 | 37 | exact |
| Front Calc callback | 1491/1491 | 107 | exact |
| Front Draw callback | 1474/1474 | 127 | exact |

The original PE factory `REL32` at relative offset +0x61 was independently
verified to resolve to **0x415D40** before replacing the source-linked C++
method symbol. The constructor-only VC7.1 `/O2 /Ob0` carrier also retains
five complete original-bound source-equivalent bodies.

The original-pinned VC7.1 `/O2 /Ob1` profiles reproduce every byte/field of
the five affected canonical owners; 45 targeted Front tests plus two new
native factory-owner tests pass. No new authored exact credit or broad cold
replay is claimed. Native Windows product/runtime equivalence is still open.
