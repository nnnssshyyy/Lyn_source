"""Export selected Raze decompilations from the local IDA MCP session."""
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[1]))
from ida_mcp_client import call, data

out = Path(__file__).resolve().parents[2] / 'docs/re/raze/health'
out.mkdir(parents=True, exist_ok=True)
for address in ('0x1002EA20', '0x10036160', '0x10156830', '0x101669E0', '0x1016A520'):
    offset = 0
    pieces = []
    while True:
        page = data(call('decompile', {'database': 'raze-health', 'addr': address,
                                       'offset': offset, 'max_lines': 5000,
                                       'include_addresses': True}))
        pieces.append(page['code'])
        cursor = page.get('cursor', {})
        if cursor.get('done'):
            break
        next_offset = cursor.get('next')
        if not isinstance(next_offset, int) or next_offset <= offset:
            raise RuntimeError(f'{address}: stalled cursor {cursor}')
        offset = next_offset
    code = '\n'.join(pieces)
    target = out / f'{address.lower()}.c'
    target.write_text(code, encoding='utf-8')
    print(f'{address}: {len(code.splitlines())} lines -> {target}')
