"""Non-crediting physical-order CFG with one independently attested switch.

The caller binds every relocation and supplies target-reviewed jump-table
entries and the exact expected indirect dispatch operand. Unknown indirect
transfers and targets outside the decoded code fail closed.
"""


def switch_graph(instructions, immediate_type, switch_address, switch_operand, targets):
    if not instructions or not targets:
        raise ValueError('empty instructions or reviewed switch table')
    addresses = [ins.address for ins in instructions]
    if addresses != sorted(set(addresses)):
        raise ValueError('duplicate/out-of-order instructions')
    positions = {va: i for i, va in enumerate(addresses)}
    if any(va not in positions for va in targets):
        raise ValueError('table points outside decoded instructions')
    roots = {0}
    destinations = {}
    calls = {}
    switch_count = 0
    for index, ins in enumerate(instructions):
        name = ins.mnemonic
        if name in ('retf', 'iret', 'iretd', 'loop', 'loope', 'loopne',
                    'int', 'int3', 'sysenter', 'sysexit', 'ud2'):
            raise ValueError('unreviewed control transfer: ' + name)
        if name == 'call':
            if len(ins.operands) != 1 or ins.operands[0].type != immediate_type:
                raise ValueError('indirect call without identity')
            calls[index] = ins.operands[0].imm
        elif name.startswith('j'):
            if len(ins.operands) != 1:
                raise ValueError('ambiguous jump operands')
            operand = ins.operands[0]
            if operand.type == immediate_type:
                if operand.imm not in positions:
                    raise ValueError('direct branch outside decoded extent')
                destinations[index] = (positions[operand.imm],)
            elif (ins.address == switch_address and name == 'jmp' and
                  ins.op_str == switch_operand):
                destinations[index] = tuple(positions[va] for va in targets)
                switch_count += 1
            else:
                raise ValueError('unexpected indirect jump')
            roots.update(destinations[index])
        if (name.startswith('j') or name == 'ret') and index + 1 < len(instructions):
            roots.add(index + 1)
    if switch_count != 1:
        raise ValueError('reviewed switch dispatch missing or duplicated')
    starts = sorted(roots)
    spans = list(zip(starts, starts[1:] + [len(instructions)]))
    owner = {index: n for n, (start, stop) in enumerate(spans)
             for index in range(start, stop)}
    graph = []
    for start, stop in spans:
        last = instructions[stop - 1]
        kind = last.mnemonic
        successors = []
        cleanup = ()
        if kind.startswith('j'):
            successors.extend(owner[index] for index in destinations[stop - 1])
            if kind != 'jmp':
                if stop == len(instructions):
                    raise ValueError('missing conditional fallthrough')
                successors.append(owner[stop])
        elif kind == 'ret':
            if any(op.type != immediate_type for op in last.operands):
                raise ValueError('unreviewed return cleanup')
            cleanup = tuple(op.imm for op in last.operands)
        else:
            kind = 'fallthrough'
            if stop == len(instructions):
                raise ValueError('missing terminal transfer')
            successors.append(owner[stop])
        graph.append((kind, tuple(successors),
                      tuple(calls[index] for index in range(start, stop) if index in calls),
                      cleanup))
    return graph
