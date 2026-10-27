from pathlib import Path
import hashlib, json, re, shutil, subprocess
from datetime import datetime, timezone
import pefile

root = Path(__file__).resolve().parent.parent
out = root / 'build/server-hooks'
packed = root / 'build/compressed/lyn.dll'
manifest = packed.with_name('build-info.json')
old = json.loads(manifest.read_text(encoding='utf-8-sig'))
backup = root / 'build/before-server-hooks'
backup.mkdir(exist_ok=True)
if not (backup / 'build-info.json').exists():
    shutil.copy2(packed, backup / 'lyn-compressed.dll')
    shutil.copy2(root / 'build/vs3-license/lyn.dll', backup / 'lyn-original.dll')
    shutil.copy2(manifest, backup / 'build-info.json')
out.mkdir(exist_ok=True)
for name in ['lyn.dll', 'lyn.pdb']:
    shutil.copy2(root / 'build-current/Release' / name, out / name)
log = (root / 'build-current/Testing/Temporary/LastTest.log').read_text(encoding='utf-8', errors='replace')
counts = [int(x) for x in re.findall(r'^(\d+) (?:aim policy scenarios passed|.*checks.*0 fail(?:ed|ures))', log, re.M)]
assert len(counts) == 14 and sum(counts) == 551, counts
(out / 'test-results.log').write_text(log, encoding='utf-8')
upx = Path(r'C:\Users\User\.codex\tools\upx-5.2.1\upx-5.2.1-win64\upx.exe')
def run(args):
    result = subprocess.run([str(x) for x in args], check=True, capture_output=True)
    return result.stdout.decode('utf-8', errors='replace')
run([upx, '--best', '--lzma', '--strip-relocs=0', '--no-progress', '--force-overwrite', '-o', packed, out / 'lyn.dll'])
integrity = run([upx, '-t', packed])
native = run([root / 'build/compressed/validation/out/Release/check_load.exe', packed, root / 'assets'])
assert 'PASS: native load, relocation, and 11 resources (2862261 bytes)' in native, native
roundtrip = packed.with_name('lyn-validation-roundtrip.dll')
run([upx, '-d', '--force-overwrite', '-o', roundtrip, packed])
sections = ['.text', '.data', '.fptable', '.reloc']
with pefile.PE(str(out / 'lyn.dll')) as original, pefile.PE(str(roundtrip)) as decoded:
    for name in sections:
        section = lambda pe: next(s for s in pe.sections if s.Name.rstrip(b'\0').decode() == name).get_data()
        assert section(original) == section(decoded), name
roundtrip.unlink()
hash_file = lambda p: hashlib.sha256(Path(p).read_bytes()).hexdigest()
for folder in ['build/Release', 'build/loader']:
    shutil.copy2(packed, root / folder / 'lyn.dll')
    shutil.copy2(out / 'lyn.pdb', root / folder / 'lyn.pdb')
    assert hash_file(root / folder / 'lyn.dll') == hash_file(packed)
old['feature'] = 'VS 3.1: authenticated signed DLL delivery, server-generated signed hook patches, duplex ACK/retry, owned weapon metadata, automatic hitbox fallback, grounded bunnyhop retry'
old['original'] = dict(path=str(out / 'lyn.dll'), bytes=(out / 'lyn.dll').stat().st_size, sha256=hash_file(out / 'lyn.dll'))
old['compressed'] = dict(path=str(packed), bytes=packed.stat().st_size, sha256=hash_file(packed))
old['reduction_percent'] = round((1-packed.stat().st_size/(out / 'lyn.dll').stat().st_size)*100, 2)
for name in ['vendor/minhook/src/hook.c', 'vendor/minhook/include/MinHook.h', 'src/remote_hooks.h', 'src/remote_hooks.cpp', 'tests/remote_hooks_tests.cpp', 'src/license.h', 'src/license.cpp', 'src/dllmain.cpp', 'src/hooks.cpp', 'src/features.cpp', 'src/chat_spy.cpp', 'tests/license_tests.cpp', 'tests/chat_runtime_tests.cpp', 'CMakeLists.txt']:
    old['source_sha256'][name] = ''
for name in old['source_sha256']:
    old['source_sha256'][name] = hash_file(root / name)
old['validation'].update(checks_passed=sum(counts), aim_selection_checks_passed=66, aim_policy_checks_passed=28,
    runtime_api_checks_passed=75, shotgun_checks_passed=68, in_game_tested=False, dedicated_accuracy_verified=False)
old['aim_accuracy'] = dict(live_tick_preserved=True, historical_pose_timestamp='render clock minus validated interpolation; reject extrapolation',
    history_stage='FRAME_RENDER_END', all_modes_center_first=True, normal_motion_tracking=True,
    automatic_fire_hitbox_radius=True, all_hitscan_spread_refreshed=True, full_movement_prediction_added=False)
old['license'] = dict(protocol=2, signature='ECDSA P-256 / SHA-256 / raw r||s', lease_seconds=120, renewal_seconds=30, pid_creation_bound=True, admin_secrets_embedded=False)
old['remote_hooks'] = dict(local_pe_loader=True, local_trampolines=True, server_generated_patches=True, signed_plan_seconds=30, riJin_flat_block_manual_map=False)
old['built_at_utc'] = datetime.now(timezone.utc).isoformat()
old['test_log'] = str(out / 'test-results.log')
old['previous_delivery_backup'] = str(backup)
manifest.write_text(json.dumps(old, indent=2)+'\n', encoding='utf-8')
(out / 'build-info.json').write_text(manifest.read_text(encoding='utf-8'), encoding='utf-8')
validation = dict(checks=counts, total=sum(counts), upx=integrity.strip(), native=native.strip(), sections=sections, in_game_tested=False)
(out / 'package-validation.json').write_text(json.dumps(validation, indent=2)+'\n', encoding='utf-8')
print(json.dumps(dict(original=old['original'], compressed=old['compressed'], reduction=old['reduction_percent'], checks=sum(counts))))
