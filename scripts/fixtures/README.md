# Historical compiler inputs

`type3-pre-cirno.cpp.txt` is the authored native source from commit
`944870a99a8124cffdd1228ba8980b601d64f964`, before the Cirno-Labs integration.
It retains the repository MIT grant. SHA-256:
`c57cf9b9277f967832c5ab85031a4610716a88cb03fe02e96efa6389cac030a2`.

The old word-order and native-context probes use this frozen root with live,
recursively hashed project headers. They reproduce historical hypotheses;
they do not test or grant exactness to the current exact Type3 owner. Reports
identify the frozen source and its commit. Current-source verification uses
`check-boss-frontiers.py --owner type3` and the canonical match unit.
