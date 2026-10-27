"""Package the capture performance module after the full native regression run."""
from pathlib import Path
from datetime import datetime, timezone
import hashlib
import json
import shutil
import struct
import subprocess
import argparse
import re

root = Path(__file__).resolve().parent.parent
parser = argparse.ArgumentParser()
parser.add_argument('--version', default='3.1.18')
parser.add_argument('--out')
parser.add_argument('--feature', default='Explicit Steam game-capture route without a private swap chain; GPU-aware, empty-frame-free display privacy; no privacy blur copies')
parser.add_argument('--source', nargs='+')
args = parser.parse_args()
assert re.fullmatch(r'\d{1,3}\.\d{1,3}\.\d{1,3}', args.version)
out = (root / (args.out or f'build/privacy-{args.version}')).resolve()
assert out.is_relative_to(root / 'build')
upx = Path(r'C:\Users\User\.codex\tools\upx-5.2.1\upx-5.2.1-win64\upx.exe')

def run(args):
    return subprocess.run([str(arg) for arg in args], capture_output=True, check=True).stdout.decode('utf-8', errors='replace')

def sha(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()

def sections(path):
    data = path.read_bytes()
    assert data[:2] == b'MZ'
    pe = struct.unpack_from('<I', data, 0x3c)[0]
    assert data[pe:pe+4] == b'PE\0\0'
    assert struct.unpack_from('<H', data, pe+4)[0] == 0x14c
    first = pe + 24 + struct.unpack_from('<H', data, pe+20)[0]
    result = {}
    for index in range(struct.unpack_from('<H', data, pe+6)[0]):
        offset = first + index * 40
        name = data[offset:offset+8].rstrip(b'\0').decode('ascii')
        size, start = struct.unpack_from('<II', data, offset+16)
        result[name] = data[start:start+size]
    return result

test_log = (root / 'build/Testing/Temporary/LastTest.log').read_text(encoding='utf-8', errors='replace')
assert test_log.count('Test Passed.') == 21 and 'Test Failed.' not in test_log
assert '44 capture overlay checks, 0 failed' in test_log
assert not out.exists(), 'Inspect existing artifacts before packaging again.'
out.mkdir()
previous = json.loads((root / 'build/compressed/build-info.json').read_text(encoding='utf-8-sig'))
assert sha(root / 'build/compressed/lyn.dll') == previous['compressed']['sha256']
backup = out / ('previous-' + previous['module_version'])
backup.mkdir()
for name in ['lyn.dll', 'build-info.json']:
    shutil.copy2(root / 'build/compressed' / name, backup / name)
raw, packed = out / 'lyn-original.dll', out / 'lyn.dll'
shutil.copy2(root / 'build/Release/lyn.dll', raw)
shutil.copy2(root / 'build/Release/lyn.pdb', out / 'lyn.pdb')
(out / 'test-results.log').write_text(test_log, encoding='utf-8')
run([upx, '--best', '--lzma', '--strip-relocs=0', '--no-progress', '-o', packed, raw])
integrity = run([upx, '-t', packed])
native = run([root / 'build/compressed/validation/out/Release/check_load.exe', packed, root / 'assets'])
assert 'PASS: native load, relocation, and 11 resources' in native
decoded = out / 'lyn-roundtrip.dll'
run([upx, '-d', '-o', decoded, packed])
original, roundtrip = sections(raw), sections(decoded)
preserved = ['.text', '.data', '.fptable', '.reloc']
for name in preserved:
    assert original[name] == roundtrip[name], name
decoded.unlink()
sources = args.source or ['src/hooks.cpp', 'src/capture_overlay.cpp', 'src/capture_overlay.h', 'src/overlay_callsite.h',
           'src/features_fields.inc', 'src/menu.cpp', 'src/velocity_ui/menu.features.cpp',
           'tests/capture_overlay_tests.cpp', 'tests/settings_persistence_tests.cpp']
info = {
    'module_version': args.version,
    'feature': args.feature,
    'built_at_utc': datetime.now(timezone.utc).isoformat(),
    'original': {'path': str(raw), 'bytes': raw.stat().st_size, 'sha256': sha(raw)},
    'compressed': {'path': str(packed), 'bytes': packed.stat().st_size, 'sha256': sha(packed)},
    'source_sha256': {name: sha(root / name) for name in sources},
    'validation': {'ctest_suites_passed': 21, 'ctest_suites_failed': 0, 'capture_overlay_checks': 44,
                   'upx_test': True, 'native_load_relocation_resources': True,
                   'roundtrip_sections_identical': preserved, 'in_game_tested': False,
                   'obs_recording_tested': False, 'in_game_fps_measured': False, 'remote_published': False},
    'previous_delivery_backup': str(backup),
}
for name, pattern in [('rapid_fire_checks', r'(\d+) rapid fire checks, 0 failed'),
                      ('runtime_api_checks', r'(\d+) runtime API regression checks, 0 failed')]:
    match = re.search(pattern, test_log)
    if match:
        info['validation'][name] = int(match[1])
serialized = json.dumps(info, indent=2) + '\n'
(out / 'build-info.json').write_text(serialized, encoding='utf-8')
(out / 'package-validation.json').write_text(json.dumps({'upx': integrity.strip(), 'native': native.strip()}, indent=2)+'\n', encoding='utf-8')
for folder in ['build/Release', 'build/loader', 'build/compressed']:
    shutil.copy2(packed, root / folder / 'lyn.dll')
    assert sha(root / folder / 'lyn.dll') == sha(packed)
shutil.copy2(out / 'lyn.pdb', root / 'build/loader/lyn.pdb')
(root / 'build/compressed/build-info.json').write_text(serialized, encoding='utf-8')
print(json.dumps({'module_version': info['module_version'], 'compressed': info['compressed'], 'validation': info['validation']}))
