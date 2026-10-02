"""Non-crediting branch checks over an instruction similarity correspondence.

No target reads, byte rewriting, acceptance verdicts or Capstone dependency.
Unpaired branches/destinations remain outside the diagnostic's coverage.
"""


def aligned_branch_conflicts(target, candidate, blocks, immediate_type):
    correspondence = {}
    paired = []
    for block in blocks:
        for left, right in zip(
            target[block.a:block.a + block.size],
            candidate[block.b:block.b + block.size],
        ):
            correspondence[right.address] = left.address
            paired.append((left, right))
    conflicts = []
    unpaired_destinations = 0
    for left, right in paired:
        if not left.mnemonic.startswith('j'):
            continue
        if left.operands[0].type != immediate_type or right.operands[0].type != immediate_type:
            continue
        destination = correspondence.get(right.operands[0].imm)
        if destination is None:
            unpaired_destinations += 1
        elif destination != left.operands[0].imm:
            conflicts.append((left.address, left.operands[0].imm, destination))
    return conflicts, unpaired_destinations
