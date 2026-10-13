"""Non-crediting, target-independent alignment of complete code operand inventories.

Records are (owner-relative offset, relocation kind, independently bound
physical destination). Do not use matching entries to infer codegen exactness:
register allocation/CSE can reorder or eliminate individual address loads.
"""

from difflib import SequenceMatcher


def align_fields(target, candidate):
    """Return all aligned field changes with intact source-relative offsets.

    This helper does not inspect a binary or supply a destination oracle.
    Callers must attest target identity and decode complete owner operands first.
    """
    for label, records in (("target", target), ("candidate", candidate)):
        previous = -1
        for record in records:
            if len(record) != 3:
                raise ValueError(label + ": expected offset/type/destination")
            offset, kind, destination = record
            if (not isinstance(offset, int) or offset <= previous or
                    kind not in ("REL32", "DIR32") or
                    not isinstance(destination, int) or destination < 0):
                raise ValueError(label + ": unreviewed field record/order")
            previous = offset

    matcher = SequenceMatcher(
        None, [(kind, dest) for _, kind, dest in target],
        [(kind, dest) for _, kind, dest in candidate],
        autojunk=False,
    )
    changes = []
    paired = 0
    for tag, t_first, t_last, c_first, c_last in matcher.get_opcodes():
        if tag == 'equal':
            paired += t_last - t_first
            continue
        changes.append({
            'kind': tag,
            'target': [tuple(row) for row in target[t_first:t_last]],
            'candidate': [tuple(row) for row in candidate[c_first:c_last]],
        })
    return {'paired_field_identities': paired, 'target_fields': len(target),
            'candidate_fields': len(candidate), 'changes': changes}
