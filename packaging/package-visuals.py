"""Package a tested module, preserving the previous signed distribution.

Requires a successful Release build and the complete native CTest run. The
loader executable stays unchanged; only the authenticated module is replaced.
"""
from pathlib import Path
from datetime import datetime, timezone
import hashlib
import json
import shutil
import struct
import subprocess
import argparse
import re

parser = argparse.ArgumentParser()
parser.add_argument('--version', default='3.1.12')
parser.add_argument('--previous', default='3.1.11')
parser.add_argument('--label', default='visuals', choices=['visuals', 'aim-movement', 'target-life', 'privacy'])
parser.add_argument('--feature', default='Confirmed damage indicators, offscreen arrows, boss HUD, estimated throwable paths, legible ESP and profile-persisted HUD editing')
args = parser.parse_args()
assert all(part.isdigit() for part in args.version.split('.')) and args.version.count('.') == 2
assert all(part.isdigit() for part in args.previous.split('.')) and args.previous.count('.') == 2

root = Path(__file__).resolve().parent.parent
out = root / f'build/{args.label}-{args.version}'
backup = out / f'previous-{args.previous}'
raw = out / 'lyn-original.dll'
packed = out / 'lyn.dll'
upx = Path(r'C:\Users\User\.codex\tools\upx-5.2.1\upx-5.2.1-win64\upx.exe')

def sha(path):
    return hashlib.sha256(Path(path).read_bytes()).hexdigest()

def run(args):
    result = subprocess.run([str(arg) for arg in args], capture_output=True, check=True)
    return result.stdout.decode('utf-8', errors='replace')

def sections(path):
    data = path.read_bytes()
    pe = struct.unpack_from('<I', data, 0x3c)[0]
    assert data[pe:pe+4] == b'PE\0\0'
    assert struct.unpack_from('<H', data, pe+4)[0] == 0x14c
    assert struct.unpack_from('<H', data, pe+22)[0] & 0x2000
    assert struct.unpack_from('<H', data, pe+24)[0] == 0x10b
    count = struct.unpack_from('<H', data, pe+6)[0]
    first = pe+24+struct.unpack_from('<H', data, pe+20)[0]
    result = {}
    for index in range(count):
        offset = first+index*40
        name = data[offset:offset+8].rstrip(b'\0').decode('ascii')
        size, start = struct.unpack_from('<II', data, offset+16)
        result[name] = data[start:start+size]
    return result

out.mkdir(exist_ok=True)
assert not raw.exists() and not packed.exists(), 'Inspect existing version artifacts before overwriting.'
info = json.loads((root / 'build/compressed/build-info.json').read_text(encoding='utf-8-sig'))
assert info['module_version'] == args.previous, 'A newer module exists; reconcile before publication.'
assert sha(root / 'build/compressed/lyn.dll') == info['compressed']['sha256']
backup.mkdir(exist_ok=True)
for name in ['lyn.dll', 'build-info.json']:
    shutil.copy2(root / 'build/compressed' / name, backup / name)
test_log = (root / 'build/Testing/Temporary/LastTest.log').read_text(encoding='utf-8', errors='replace')
suite_count=21 if args.label=='privacy' else 18
assert test_log.count('Test Passed.') == suite_count
assert '26 visual logic checks, 0 failed' in test_log
assert '9 visual event ABI checks, 0 failed' in test_log
assert '34 visual rendering checks, 0 failed' in test_log
if args.label == 'aim-movement':
    assert '76 aim selection / camera checks, 0 failed' in test_log
    assert '121 air movement checks, 0 failed' in test_log
if args.label == 'target-life':
    assert '130 aim selection / camera checks, 0 failed' in test_log
    assert '40 aim policy scenarios passed' in test_log
    assert '97 runtime API regression checks, 0 failed' in test_log
if args.label=='privacy':
    removal_checks=int(re.search(r'(\d+) visual removal checks, 0 failed',test_log).group(1))
    assert removal_checks>=20
    capture_checks=int(re.search(r'(\d+) capture overlay checks, 0 failed',test_log).group(1))
    assert capture_checks>=12
    charger_checks=int(re.search(r'(\d+) charger turn checks, 0 failed',test_log).group(1))
    assert charger_checks>=19
    assert '101 runtime API regression checks, 0 failed' in test_log
(out / 'test-results.log').write_text(test_log, encoding='utf-8')
loader_hash = sha(root / 'build/loader/Lyn.Loader.exe')
shutil.copy2(root / 'build/Release/lyn.dll', raw)
shutil.copy2(root / 'build/Release/lyn.pdb', out / 'lyn.pdb')
run([upx, '--best', '--lzma', '--strip-relocs=0', '--no-progress', '-o', packed, raw])
integrity = run([upx, '-t', packed])
assert packed.stat().st_size <= 4 * 1024 * 1024, 'Private publisher module size limit exceeded.'
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
assert sha(root / 'build/loader/Lyn.Loader.exe') == loader_hash
info.pop('publication', None)
info.update(module_version=args.version,
    feature=args.feature,
    ui={'aim-movement':'Aimbot / activation; Movimiento / bunny hop',
        'target-life':'Aimbot / normal, silent client, silent server / target validity',
        'privacy':'World / no fog, no vomit, OBS / Discord privacy (F8)'}.get(args.label,'Visuales / HUD / efectos; Jugadores / ESP'),
    original={'path': str(raw), 'bytes': raw.stat().st_size, 'sha256': sha(raw)},
    compressed={'path': str(root / 'build/compressed/lyn.dll'), 'bytes': packed.stat().st_size, 'sha256': sha(packed)},
    reduction_percent=round(100*(1-packed.stat().st_size/raw.stat().st_size), 2),
    built_at_utc=datetime.now(timezone.utc).isoformat(), previous_delivery_backup=str(backup),
    test_log=str(out / 'test-results.log'))
sources = ['CMakeLists.txt', 'src/features.cpp', 'src/features.h', 'src/aimbot.cpp', 'src/aimbot.h', 'src/aim_logic.h',
    'src/game.cpp', 'src/game.h', 'src/air_movement.h', 'src/features_fields.inc', 'src/visuals.cpp', 'src/visuals.h',
    'src/capture_overlay.cpp', 'src/capture_overlay.h', 'src/overlay_budget.h', 'src/visual_removals.h', 'src/visual_removals.cpp', 'src/hooks.cpp', 'src/menu_input.h',
    'src/charger_turn.h', 'src/charger_turn.cpp', 'tests/charger_turn_tests.cpp', 'src/velocity_ui/features_ui.hpp',
    'src/visual_events.h', 'src/visuals_logic.h', 'src/esp_style.h', 'src/menu.cpp',
    'src/velocity_ui/hud_widgets.hpp', 'src/velocity_ui/visuals_window.cpp', 'src/velocity_ui/visuals_window.hpp',
    'src/velocity_ui/radar_window.cpp', 'src/velocity_ui/chat_window.cpp', 'src/velocity_ui/spectators_window.cpp',
    'src/velocity_ui/menu.features.cpp', 'src/velocity_ui/weapons_page.cpp', 'src/velocity_ui/menu.core.cpp', 'src/velocity_ui/rendering.hpp',
    'tests/visual_events_tests.cpp', 'tests/visuals_logic_tests.cpp', 'tests/visuals_render_tests.cpp', 'tests/settings_persistence_tests.cpp',
    'tests/aim_selection_tests.cpp', 'tests/aim_logic_tests.cpp', 'tests/runtime_api_tests.cpp', 'tests/air_movement_tests.cpp',
    'tests/visual_removals_tests.cpp', 'tests/capture_overlay_tests.cpp']
for name in sorted(set(info.get('source_sha256', {})) | set(sources)):
    info.setdefault('source_sha256', {})[name] = sha(root / name)
info['validation'] = {'ctest_suites_passed': suite_count, 'ctest_suites_failed': 0, 'visual_logic_checks': 26,
    'visual_event_abi_checks': 9, 'visual_render_checks': 34, 'upx_test': True,
    'x86_LoadLibraryW': True, 'relocated_load': True, 'resource_payloads_identical': 11,
    'resource_payload_bytes': 2862261, 'roundtrip_sections_identical': preserved,
    'loader_executable_modified': False, 'remote_published': False, 'in_game_tested': False}
info['visuals_limits'] = {'trajectory': 'Estimated flight only; <=129 trace calls per 100ms update including release clearance',
    'threat_capture': 'At most 20Hz; 96 threats, 24 non-overlapping direction sectors, 8 bosses / 4 displayed',
    'confirmed_damage': 'Only received player_hurt / infected_hurt events; non-networked events cannot be fabricated',
    'boss_health': 'HP -- when the server does not replicate health; max uses native field or observed peak',
    'render_thread': 'POD snapshots only, no entity reads or engine traces'}
if args.label == 'aim-movement':
    info.setdefault('aim_diagnostics', {}).update(always_activation_respects_effective_auto_shoot_off=False,
        always_activation_initiates_automatic_fire=True, normal_automatic_fire_waits_for_alignment=True)
    info.setdefault('air_movement', {}).update(walking_infected_ghost_bunnyhop=True,
        ghost_combat_paused=True, noclip_ladders_observer_swimming_assistance=False)
    info['validation'].update(aim_selection_camera_checks=76, air_movement_checks=121)
if args.label == 'target-life':
    info['target_life']={'death_sequence_names':True,'case_insensitive':True,'replicated_client_ragdoll':True,
        'dead_history_discarded':True,'stable_sequence_lookup_cached':True,'sequence_lookup_failure_retried':True,
        'all_aim_modes':True,'known_health_life_solidity_checks_preserved':True,'missing_npc_health_invented':False}
    for key,pattern in [('aim_selection_camera_checks',r'(\d+) aim selection / camera checks, 0 failed'),
                        ('aim_policy_checks',r'(\d+) aim policy scenarios passed'),
                        ('runtime_api_checks',r'(\d+) runtime API regression checks, 0 failed')]:
        info['validation'][key]=int(re.search(pattern,test_log).group(1))
if args.label=='privacy':
    info['validation'].update(visual_removal_checks=removal_checks,capture_overlay_checks=capture_checks,charger_turn_checks=charger_checks,runtime_api_checks=101,
        native_private_backbuffer_isolated=True,native_capture_affinity_verified=True)
    info['capture_privacy']={'separate_window':True,'separate_swapchain':True,'game_backbuffer_modified':False,
        'windows_affinity': 'WDA_EXCLUDEFROMCAPTURE', 'requires_windowed_or_borderless':True,
        'native_glow_chams_engine_chat_paused':True,'unsupported_route':'private draw suppressed; F8 disables',
        'obs_discord_live_tested':False,'all_capture_methods_guaranteed':False}
    info['visual_removals']={'no_fog':'ClientMode ShouldDrawFog vtable entry 18; shared code untouched',
        'no_vomit':'native bile shader-enable query and particle drawing routes','server_effects_modified':False,
        'entity_timestamps_modified':False,'fog_convar_writes':False,'label_overlap_option_removed':True,'menu_help_removed':True}
    info['capture_privacy'].update(private_frame_rate='display refresh clamped to 60-90 Hz',
        unchanged_private_frames_reused=True,geometry_poll_ms=100,menu_blur_refresh_hz=25,menu_blur_tail_ms=200)
    info['charger_turn']={'signature_unique_in_installed_client':True,'absolute_rva_used':False,'branch_destination_validated':True,
        'live_controlled_charger_only':True,'restored_on_disable_class_change_level_clear':True,'external_changes_preserved':True,
        'dedicated_server_tested':False}
serialized = json.dumps(info, indent=2)+'\n'
(out / 'build-info.json').write_text(serialized, encoding='utf-8')
(root / 'build/compressed/build-info.json').write_text(serialized, encoding='utf-8')
(out / 'package-validation.json').write_text(json.dumps({'upx': integrity.strip(), 'native': native.strip(),
    'sections': preserved, 'validation': info['validation']}, indent=2)+'\n', encoding='utf-8')
print(json.dumps({'module_version': info['module_version'], 'compressed': info['compressed'], 'validation': info['validation']}))
