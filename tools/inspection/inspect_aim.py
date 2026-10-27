import sys, struct
from pathlib import Path
sys.path.insert(0, str(Path(__file__).parent / 'inspection-deps'))
import pefile
from capstone import Cs, CS_ARCH_X86, CS_MODE_32

path = Path(r'C:\Program Files (x86)\Steam\steamapps\common\Left 4 Dead 2\left4dead2\bin\client.dll')
pe = pefile.PE(str(path))
blob = pe.get_memory_mapped_image()
base = pe.OPTIONAL_HEADER.ImageBase
md = Cs(CS_ARCH_X86, CS_MODE_32)
def scan(pattern):
    values = [None if '?' in b else int(b,16) for b in pattern.split()]
    first = bytes([values[0]])
    pos = 0
    hits=[]
    while (pos := blob.find(first,pos)) >= 0:
        if all(v is None or blob[pos+i] == v for i,v in enumerate(values)):
            hits.append(pos)
        pos += 1
    return hits
def dump(rva,size):
    for i in md.disasm(blob[rva:rva+size],base+rva):
        print(f'{i.address-base:08x} {i.mnemonic:8} {i.op_str}')
for name,pattern in [
 ('ClientMode','89 04 B5 ? ? ? ? E8'),
 ('RijiN command callback','E8 ? ? ? ? 8B 7D FC 56'),
 ('Shared random','55 8B EC 83 EC ? A1 ? ? ? ? 53 56 57 8B 7D ? 8D 4D ? 51 89 7D ? 89 45 ? E8'),
 ('Sequence name','E8 ? ? ? ? 89 45 EC 8B 45 D0')]:
    hits=scan(pattern)
    print(name, [hex(h) for h in hits])
    if len(hits)==1:
        hit=hits[0]
        dump(hit,80)
        if blob[hit]==0xe8:
            target=hit+5+struct.unpack_from('<i',blob,hit+1)[0]
            print('CALL TARGET',hex(target))
            dump(target,180)
print('COMMAND CALL CONTEXT')
dump(0x134590,310)
print('SHARED RANDOM')
dump(0x1acdb0,185)
ep=pefile.PE(r'C:\Program Files (x86)\Steam\steamapps\common\Left 4 Dead 2\bin\engine.dll')
blob=ep.get_memory_mapped_image()
base=ep.OPTIONAL_HEADER.ImageBase
print('SENDPACKET LOCAL INITIALIZATION', [hex(h) for h in scan('C6 45 E3 01')])
for h in scan('C6 45 E3 01'):
    dump(h-18,85)
