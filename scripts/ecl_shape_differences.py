"""Pure, non-crediting aligned opcode instruction diff for a verified ECL audit.

Inputs are complete decoded streams of (owner-relative byte offset, normalized
instruction bytes).  Relocation/branch normalization and target identity must
be checked by the caller; this helper neither pairs identities nor grants
source or exact-byte credit.
"""

from difflib import SequenceMatcher


def changed_instructions(candidate, target):
    """Return only differing aligned spans, keeping both original offsets."""
    matcher = SequenceMatcher(
        a=[data for _, data in candidate],
        b=[data for _, data in target],
        autojunk=False,
    )
    result = []
    for tag, c_start, c_end, t_start, t_end in matcher.get_opcodes():
        if tag == "equal":
            continue
        result.append({
            "change": tag,
            "candidate": [(offset, data.hex()) for offset, data in candidate[c_start:c_end]],
            "target": [(offset, data.hex()) for offset, data in target[t_start:t_end]],
        })
    return result
