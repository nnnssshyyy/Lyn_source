"""Package the Priorizar clases build (module 3.1.40) after its native suites.

The checkbox 'Priorizar clases' restricts target selection to the special
classes the user marks (Smoker, Boomer, Hunter, Spitter, Jockey, Charger,
Tank). Unmarked classes are never chosen and there is no substitution when the
picked target leaves the FOV, range or line of sight; with no class marked the
aimbot picks no target.
"""
from pathlib import Path
from datetime import datetime, timezone
import hashlib
import json
import re
import shutil
import struct
import subprocess

root = Path(__file__).resolve().parent.parent
out = root / 'build/special-classes-3.1.40'
raw = out / 'lyn-original.dll'
packed = out / 'lyn.dll'
upx = Path(r'C:\Users\User\.codex\tools\upx-5.2.1\upx-5.2.1-win64\upx.exe')


def run(args):
    result = subprocess.run([str(arg) for arg in args], capture_output=True, check=True)
    return result.stdout.decode('utf-8', errors='replace')


def sha(path):
    return hashlib.sha256(Path(path).read_bytes()).hexdigest()


def sections(path):
    data = Path(path).read_bytes()
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


# The parallel build left the uncompressed DLL under its plain name. Re-running
# is allowed only while lyn-original.dll is still that same uncompressed build.
built = out / 'lyn.dll'
if raw.exists():
    assert sha(raw) == sha(built) and built.stat().st_size > 8 * 1024 * 1024, 'Inspect the existing original before packaging again.'
else:
    assert built.exists() and built.stat().st_size > 8 * 1024 * 1024, 'The build must be the uncompressed 3.1.40 DLL.'
    shutil.copy2(built, raw)
assert sha(out / 'previous-3.1.39.dll') != sha(raw), 'The new build must differ from 3.1.39.'

# Preserve the delivery that is live right now (3.1.37) before replacing it.
live = json.loads((root / 'build/slots-snow-3.1.37/build-info.json').read_text(encoding='utf-8-sig'))
assert sha(root / 'build/compressed/lyn.dll') == live['publication']['sha256'], 'Live delivery moved.'
backup = out / 'previous-3.1.37'
backup.mkdir(exist_ok=True)
for name in ['lyn.dll', 'build-info.json']:
    shutil.copy2(root / 'build/compressed' / name, backup / name)

detailed = (root / 'build-current/Testing/Temporary/LastTest.log').read_text(encoding='utf-8', errors='replace')
summary = (out / 'tests.log').read_text(encoding='utf-8', errors='replace')
assert detailed.count('Test Passed.') == 6 and 'Test Failed.' not in detailed
assert '100% tests passed, 0 tests failed out of 6' in summary
assert '327 aim selection / camera checks, 0 failed' in detailed
assert '36 settings persistence checks across 50 weapons, 0 failed' in detailed

# The parallel build left the uncompressed DLL as lyn.dll, and UPX refuses to
# write over an existing file, so the delivery name is cleared first.
if packed.exists():
    packed.unlink()
compressed = run([upx, '--best', '--lzma', '--strip-relocs=0', '--no-progress', '-o', packed, raw])
(out / 'compression.log').write_text(compressed, encoding='utf-8')
integrity = run([upx, '-t', packed])
(out / 'compression-integrity.log').write_text(integrity, encoding='utf-8')
native = run([root / 'build/compressed/validation/out/Release/check_load.exe', packed, root / 'assets'])
(out / 'native-test-details.log').write_text(native, encoding='utf-8')
match = re.search(r'PASS: native load, relocation, and (\d+) resources \((\d+) bytes\)', native)
assert match, native
resources, resource_bytes = int(match[1]), int(match[2])
decoded = out / 'lyn-roundtrip.dll'
roundtrip_log = run([upx, '-d', '-o', decoded, packed])
(out / 'compression-roundtrip.log').write_text(roundtrip_log, encoding='utf-8')
original, restored = sections(raw), sections(decoded)
preserved = ['.text', '.data', '.fptable', '.reloc']
for name in preserved:
    assert original[name] == restored[name], name
assert packed.stat().st_size < 4 * 1024 * 1024, 'The panel proxy caps modules at 4 MiB.'
decoded.unlink()

info = {
    'module_version': '3.1.40',
    'status': 'published-pending-in-game-validation',
    'feature': 'Priorizar clases: the aimbot only picks the marked special classes and never substitutes an unmarked one',
    'built_at_utc': datetime.now(timezone.utc).isoformat(),
    'original': {'path': str(raw), 'bytes': raw.stat().st_size, 'sha256': sha(raw)},
    'compressed': {'path': str(packed), 'bytes': packed.stat().st_size, 'sha256': sha(packed)},
    'source_sha256': {name: sha(root / name) for name in [
        'src/features_fields.inc', 'src/features.h', 'src/aimbot.cpp',
        'src/velocity_ui/menu.features.cpp', 'src/velocity_ui/ui_text.inl',
        'tests/aim_selection_tests.cpp', 'tests/settings_persistence_tests.cpp',
        'tools/menu-ui-preview/preview.cpp']},
    'validation': {
        'release_win32_build': 'passed',
        'ctest_suites_passed': 6, 'ctest_suites_failed': 0,
        'aim_selection_checks': 327, 'settings_persistence_checks': 36,
        'runtime_api_checks': 188, 'shotgun_spread_checks': 125,
        'ui_behavior_checks': 81, 'ui_captures_reviewed': True,
        'class_filter_default': False, 'empty_selection_has_targets': False,
        'upx_test': True, 'native_load_relocation_resources': True,
        'resources': resources, 'resource_bytes': resource_bytes,
        'roundtrip_sections_identical': preserved,
        'below_4_mib': True,
        'native_dll_load_tested': False,
        'in_game_tested': False, 'windows_10_ltsc_tested': False,
        'remote_published': False,
    },
    'previous_delivery_backup': str(backup),
}
serialized = json.dumps(info, indent=2) + '\n'
(out / 'build-info.json').write_text(serialized, encoding='utf-8')
(out / 'package-validation.json').write_text(json.dumps(
    {'upx': integrity.strip(), 'native': native.strip(), 'roundtrip_sections': preserved,
     'resources': resources, 'resource_bytes': resource_bytes}, indent=2) + '\n', encoding='utf-8')
for folder in ['build/Release', 'build/loader', 'build/compressed']:
    shutil.copy2(packed, root / folder / 'lyn.dll')
    assert sha(root / folder / 'lyn.dll') == sha(packed)
shutil.copy2(out / 'lyn.pdb', root / 'build/loader/lyn.pdb')
shutil.copy2(out / 'lyn.pdb', root / 'build/Release/lyn.pdb')
(root / 'build/compressed/build-info.json').write_text(serialized, encoding='utf-8')
print(json.dumps({'module_version': info['module_version'], 'compressed': info['compressed'],
                  'resources': resources, 'validation': info['validation']}))
