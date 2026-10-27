"""Package the locally built Raze rapidfire integration after its focused tests."""
from pathlib import Path
from datetime import datetime, timezone
import hashlib
import json
import shutil
import struct
import subprocess

root = Path(__file__).resolve().parent.parent
out = root / 'build/raze-rapid-integration'
backup = out / 'previous-3.1.7'
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
    assert struct.unpack_from('<H', data, pe+22)[0] & 0x2000
    assert struct.unpack_from('<H', data, pe+24)[0] == 0x10b
    count = struct.unpack_from('<H', data, pe+6)[0]
    first = pe + 24 + struct.unpack_from('<H', data, pe+20)[0]
    result = {}
    for index in range(count):
        offset = first + index * 40
        name = data[offset:offset+8].rstrip(b'\0').decode('ascii')
        size, start = struct.unpack_from('<II', data, offset+16)
        result[name] = data[start:start+size]
    return result


out.mkdir(exist_ok=True)
assert backup.is_dir(), 'Preserve the previous DLL and metadata before packaging.'
assert not raw.exists() and not packed.exists(), 'Inspect existing artifacts before overwriting.'
info = json.loads((backup / 'build-info.json').read_text(encoding='utf-8-sig'))
assert sha(backup / 'lyn.dll') == info['compressed']['sha256']
test_log = (root / 'build/Testing/Temporary/LastTest.log').read_text(encoding='utf-8', errors='replace')
assert test_log.count('Test Passed.') == 4
assert '69 rapid fire checks, 0 failed' in test_log
assert '85 runtime API regression checks, 0 failed' in test_log
(out / 'test-results.log').write_text(test_log, encoding='utf-8')
shutil.copy2(root / 'build/Release/lyn.dll', raw)
shutil.copy2(root / 'build/Release/lyn.pdb', out / 'lyn.pdb')
run([upx, '--best', '--lzma', '--strip-relocs=0', '--no-progress', '-o', packed, raw])
integrity = run([upx, '-t', packed])
native = run([root / 'build/compressed/validation/out/Release/check_load.exe', packed, root / 'assets'])
assert 'PASS: native load, relocation, and 11 resources (2862261 bytes)' in native
decoded = out / 'lyn-roundtrip.dll'
run([upx, '-d', '-o', decoded, packed])
original_sections, decoded_sections = sections(raw), sections(decoded)
preserved = ['.text', '.data', '.fptable', '.reloc']
for name in preserved:
    assert original_sections[name] == decoded_sections[name], name
decoded.unlink()

for folder in ['build/Release', 'build/loader', 'build/compressed']:
    shutil.copy2(packed, root / folder / 'lyn.dll')
    assert sha(root / folder / 'lyn.dll') == sha(packed)
shutil.copy2(out / 'lyn.pdb', root / 'build/loader/lyn.pdb')
info.pop('current_rollback', None)
info.pop('rapidfire_revert', None)
info.update(module_version='3.1.9',
    feature='Raze rapidfire: arm before Prediction::Update, native CL_Move arguments, correction history on release',
    original={'path': str(raw), 'bytes': raw.stat().st_size, 'sha256': sha(raw)},
    compressed={'path': str(root / 'build/compressed/lyn.dll'), 'bytes': packed.stat().st_size, 'sha256': sha(packed)},
    reduction_percent=round(100 * (1-packed.stat().st_size/raw.stat().st_size), 2),
    built_at_utc=datetime.now(timezone.utc).isoformat(),
    previous_delivery_backup=str(backup), test_log=str(out / 'test-results.log'))
for name in info['source_sha256']:
    info['source_sha256'][name] = sha(root / name)
info['validation'] = {'ctest_suites_passed': 4, 'ctest_suites_failed': 0,
    'rapid_fire_checks_passed': 69, 'runtime_api_checks_passed': 85,
    'upx_test': True, 'x86_LoadLibraryW': True, 'relocated_load': True,
    'resource_payloads_identical': 11, 'resource_payload_bytes': 2862261,
    'roundtrip_sections_identical': preserved, 'remote_published': False, 'in_game_tested': False}
info['rapidfire_integration'] = {'reference_sha256': 'dfa8632e85c89c318577ef619f20bae2f4db5136042d680ee4dae4109234691a',
    'evidence': 'docs/re/rapidfire/recheck-20261019', 'instructions_rechecked': 1432,
    'sustained_hold': True, 'speed_maximum': 200, 'ack_pacing': False,
    'prediction_update_after_arm': True, 'native_move_arguments_preserved': True,
    'pending_corrections_preserved_on_release': True, 'key_resampled_per_channel_application': True,
    'opaque_local_plus_0x10_ported': False, 'loader_executable_modified': False}
serialized = json.dumps(info, indent=2) + '\n'
(out / 'build-info.json').write_text(serialized, encoding='utf-8')
(root / 'build/compressed/build-info.json').write_text(serialized, encoding='utf-8')
(out / 'package-validation.json').write_text(json.dumps({'upx': integrity.strip(), 'native': native.strip(),
    'sections': preserved, 'validation': info['validation']}, indent=2)+'\n', encoding='utf-8')
print(json.dumps({'module_version': info['module_version'], 'original': info['original'],
    'compressed': info['compressed'], 'validation': info['validation']}))
