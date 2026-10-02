"""Non-crediting instruction-correspondence and complete direct-CFG checks.

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


def direct_control_flow(instructions, immediate_type, call_identity=None):
    """Physical-order block graph, with per-block calls and return cleanup.

    Caller must separately establish complete decoding and bind relocations.
    This covers direct edges, not predicates, data flow, behavior or bytes.
    Indirect calls fail closed unless a caller-provided identity function
    validates them and records their actual call operand (for example an IAT
    cell, not an assumed runtime callee). Indirect/external jumps and unsupported
    exits always fail closed. The default remains strict direct-call coverage.
    No correspondence is guessed for a missing destination.
    """
    if not instructions:
        raise ValueError('empty instruction stream')
    addresses = [i.address for i in instructions]
    if any(a >= b for a, b in zip(addresses, addresses[1:])):
        raise ValueError('instruction addresses are not strictly increasing')
    positions = {address: n for n, address in enumerate(addresses)}
    leaders = {0}
    destinations = {}
    call_identities = {}
    for n, instruction in enumerate(instructions):
        mnemonic = instruction.mnemonic
        if mnemonic in ('loop', 'loope', 'loopne', 'retf', 'iret', 'iretd',
                        'int', 'int3', 'sysenter', 'sysexit', 'ud2'):
            raise ValueError('unsupported control transfer')
        if mnemonic == 'call' and call_identity is not None:
            call_identities[n] = call_identity(instruction)
            if call_identities[n] is None:
                raise ValueError('missing call identity')
        elif mnemonic.startswith('j') or mnemonic == 'call':
            if len(instruction.operands) != 1 or instruction.operands[0].type != immediate_type:
                raise ValueError('non-direct control transfer')
            if mnemonic.startswith('j'):
                destination = instruction.operands[0].imm
                if destination not in positions:
                    raise ValueError('jump outside decoded instructions')
                destinations[n] = positions[destination]
                leaders.add(positions[destination])
            else:
                call_identities[n] = instruction.operands[0].imm
        if mnemonic.startswith('j') or mnemonic == 'ret':
            if n + 1 < len(instructions):
                leaders.add(n + 1)
    starts = sorted(leaders)
    spans = list(zip(starts, starts[1:] + [len(instructions)]))
    owners = {n: block for block, (a, z) in enumerate(spans) for n in range(a, z)}
    graph = []
    for a, z in spans:
        last = instructions[z - 1]
        kind = last.mnemonic
        successors = []
        cleanup = ()
        if kind.startswith('j'):
            successors.append(owners[destinations[z - 1]])
            if kind != 'jmp':
                if z == len(instructions):
                    raise ValueError('missing conditional fallthrough')
                successors.append(owners[z])
        elif kind == 'ret':
            if any(op.type != immediate_type for op in last.operands):
                raise ValueError('unreviewed return operand')
            cleanup = tuple(op.imm for op in last.operands)
        else:
            kind = 'fallthrough'
            if z == len(instructions):
                raise ValueError('missing terminal transfer')
            successors.append(owners[z])
        calls = tuple(call_identities[n] for n in range(a, z) if n in call_identities)
        graph.append((kind, tuple(successors), calls, cleanup))
    return graph
