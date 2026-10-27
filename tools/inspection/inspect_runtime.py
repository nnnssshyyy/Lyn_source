"""Read-only diagnostic of this project's loaded DLL; never writes to the game."""
import ctypes as c, struct, sys
from ctypes import wintypes as w
from pathlib import Path
k=c.WinDLL('kernel32',use_last_error=True); d=c.WinDLL('dbghelp',use_last_error=True)
k.GetCurrentProcess.restype=w.HANDLE
k.OpenProcess.argtypes=[w.DWORD,w.BOOL,w.DWORD];k.OpenProcess.restype=w.HANDLE
k.ReadProcessMemory.argtypes=[w.HANDLE,c.c_void_p,c.c_void_p,c.c_size_t,c.POINTER(c.c_size_t)]
k.CloseHandle.argtypes=[w.HANDLE]
class SI(c.Structure):
    _fields_=[('SizeOfStruct',w.ULONG),('TypeIndex',w.ULONG),('Reserved',c.c_ulonglong*2),('Index',w.ULONG),('Size',w.ULONG),('ModBase',c.c_ulonglong),('Flags',w.ULONG),('Value',c.c_ulonglong),('Address',c.c_ulonglong),('Register',w.ULONG),('Scope',w.ULONG),('Tag',w.ULONG),('NameLen',w.ULONG),('MaxNameLen',w.ULONG),('Name',c.c_char*1)]
CALLBACK=c.WINFUNCTYPE(w.BOOL,c.POINTER(SI),w.ULONG,c.c_void_p)
d.SymInitialize.argtypes=[w.HANDLE,c.c_char_p,w.BOOL]
d.SymLoadModuleEx.argtypes=[w.HANDLE,w.HANDLE,c.c_char_p,c.c_char_p,c.c_ulonglong,w.DWORD,c.c_void_p,w.DWORD];d.SymLoadModuleEx.restype=c.c_ulonglong
d.SymEnumSymbols.argtypes=[w.HANDLE,c.c_ulonglong,c.c_char_p,CALLBACK,c.c_void_p]
p=k.GetCurrentProcess();assert d.SymInitialize(p,None,False)
dll=str(Path(__file__).resolve().parents[2] / 'build/Release/lyn.dll').encode()
base=d.SymLoadModuleEx(p,None,dll,None,0x10000000,0,None,0);assert base,c.get_last_error()
remote=k.OpenProcess(0x1010,False,int(sys.argv[1]));assert remote,c.get_last_error()
loaded=int(sys.argv[2],0)
def read(address,size):
    buf=c.create_string_buffer(size);got=c.c_size_t()
    if not k.ReadProcessMemory(remote,address,buf,size,c.byref(got)):raise OSError(c.get_last_error(),hex(address))
    return buf.raw
symbols={}
@CALLBACK
def callback(ptr,size,context):
    s=ptr.contents;name=c.string_at(c.addressof(s)+SI.Name.offset,s.NameLen).decode()
    if any(key in name for key in ['published_status','published_overlay','features::api','features::pending','features::active','command_return','original_move']):
        symbols[name]=(loaded+s.Address-base,s.Size,s.TypeIndex)
    return True
assert d.SymEnumSymbols(p,base,b'*',callback,None),c.get_last_error()
d.SymGetTypeInfo.argtypes=[w.HANDLE,c.c_ulonglong,w.ULONG,c.c_int,c.c_void_p]
k.LocalFree.argtypes=[c.c_void_p]
def ti(t,which,typ=w.ULONG):
    value=typ()
    if not d.SymGetTypeInfo(p,base,t,which,c.byref(value)):return None
    return value.value
def children(t):
    n=ti(t,13) or 0
    arr=(w.ULONG*(n+2))();arr[0]=n
    if not n or not d.SymGetTypeInfo(p,base,t,7,arr):return []
    result=[]
    for child in arr[2:]:
        nameptr=ti(child,1,c.c_void_p)
        name=c.wstring_at(nameptr) if nameptr else '?'
        if nameptr:k.LocalFree(nameptr)
        offset=ti(child,10);typ=ti(child,4)
        if offset is not None:result.append((name,offset,typ))
    return result
def show_fields(address,t):
    for name,offset,typ in children(t):
        size=ti(typ,2,c.c_ulonglong) or 4
        if size<=8:print(name,offset,read(address+offset,size).hex())
    return {n:(address+o,t) for n,o,t in children(t)}
for name,(address,size,typ) in symbols.items():
    print(name,hex(address),size)
    data=read(address,size)
    if 'published_status' in name:
        print('flags',list(data[:10]),'counters',struct.unpack_from('<8I',data,12),'modes',list(data[44:48]),'target/weapon/ms',struct.unpack_from('<iif',data,48))
        print('messages',[data[i:i+128].split(b'\0')[0] for i in (60,188,316)])
    elif 'pending' in name:show_fields(address,typ)
    elif name=='features::api':
        members=show_fields(address,typ)
        print('API members',members)
        ba,bt=members['base']; interfaces=show_fields(ba,bt)
        a,t=members['classes']; print('CLASSES',read(a,40).hex());print('MAP MEMBERS',children(t))
        globals_ptr=struct.unpack('<I',read(members['globals'][0],4))[0]
        print('CLOCK',struct.unpack('<fifffii ff'.replace(' ',''),read(globals_ptr,36)))
        import sys
        sys.path.insert(0,str(Path(__file__).resolve().parent / 'inspection-deps'))
        from capstone import Cs,CS_ARCH_X86,CS_MODE_32
        md=Cs(CS_ARCH_X86,CS_MODE_32)
        for method in ['entity','local_player']:
            f=struct.unpack('<I',read(interfaces[method][0],4))[0]
            print(method,hex(f))
            for ins in md.disasm(read(f,80),f):print(ins.mnemonic,ins.op_str)

    elif size<32:print(data.hex())
k.CloseHandle(remote)
