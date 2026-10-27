"""Build the menu's local Hunter asset from an installed copy of L4D2.

Binary layouts follow Valve's public studio.h / optimize.h (Source SDK).
Only reads the game archives; output is private to this project.
"""
import argparse
import struct
import math
from pathlib import Path


def unpack(data, offset, fmt):
    return struct.unpack_from('<' + fmt, data, offset)


def cstring(data, offset):
    return data[offset:data.index(b'\0', offset)].decode('ascii')


class Vpk:
    def __init__(self, path):
        self.path = Path(path)
        data = self.path.read_bytes()
        magic, version, tree_size = unpack(data, 0, 'III')
        assert magic == 0x55aa1234 and version in (1, 2)
        pos = 12 if version == 1 else 28
        self.inline = pos + tree_size
        self.entries = {}
        def string():
            nonlocal pos
            value = cstring(data, pos)
            pos += len(value) + 1
            return value
        while ext := string():
            while directory := string():
                while name := string():
                    crc, preload, archive, offset, size, terminator = unpack(data, pos, 'IHHIIH')
                    assert terminator == 0xffff
                    pos += 18
                    key = f'{directory}/{name}.{ext}' if directory != ' ' else f'{name}.{ext}'
                    self.entries[key.lower()] = (archive, offset, size, data[pos:pos+preload])
                    pos += preload

    def read(self, name):
        archive, offset, size, preload = self.entries[name.lower()]
        path = self.path if archive == 0x7fff else self.path.with_name(self.path.name.replace('_dir', f'_{archive:03}'))
        with path.open('rb') as stream:
            stream.seek(offset + (self.inline if archive == 0x7fff else 0))
            data = stream.read(size)
        assert len(data) == size
        return preload + data


def bones_from_mdl(data):
    count, base = unpack(data, 156, 'ii')
    bones = []
    for i in range(count):
        p = base + i * 216
        name = cstring(data, p + unpack(data, p, 'i')[0])
        bones.append(dict(name=name, parent=unpack(data,p+4,'i')[0],
                          pos=unpack(data,p+32,'3f'), quat=unpack(data,p+44,'4f'),
                          inverse=unpack(data,p+96,'12f')))
    return bones


def quat48(data, p):
    a,b,c = unpack(data,p,'3H')
    x,y,z = (a-32768)/32768, (b-32768)/32768, ((c&32767)-16384)/16384
    w = math.sqrt(max(0,1-x*x-y*y-z*z)) * (-1 if c&32768 else 1)
    return x,y,z,w


def animation(mdl, ani, bones, wanted):
    count, base = unpack(mdl,180,'ii')
    for i in range(count):
        p = base+100*i
        if cstring(mdl,p+unpack(mdl,p+4,'i')[0]) == wanted:
            break
    else:
        raise ValueError(f'Animation missing: {wanted}')
    fps, flags, frames = unpack(mdl,p+8,'fii')
    block, offset = unpack(mdl,p+52,'ii')
    assert flags&64 and not flags&4 and unpack(mdl,p+84,'i')[0] == 0
    block_table = unpack(mdl,356,'i')[0]
    start,end = unpack(mdl,block_table+8*block,'ii')
    data = ani[start+offset:end]
    constant, frame, stride = unpack(data,0,'iii')
    bone_flags = data[24:24+len(bones)]
    defaults = [[*b['pos'],*b['quat']] for b in bones]
    cursor=constant
    for b, f in enumerate(bone_flags):
        assert not f&0xc0  # This Hunter uses the original quaternion packing.
        if f&2:
            defaults[b][3:]=quat48(data,cursor); cursor+=6
        if f&1:
            defaults[b][:3]=unpack(data,cursor,'3e'); cursor+=6
        if f&32:
            defaults[b][:3]=unpack(data,cursor,'3f'); cursor+=12
    poses=[]
    for n in range(frames):
        pose=[b.copy() for b in defaults]
        cursor=frame+n*stride
        for b,f in enumerate(bone_flags):
            if f&8:
                pose[b][3:]=quat48(data,cursor); cursor+=6
            if f&4:
                pose[b][:3]=unpack(data,cursor,'3e'); cursor+=6
            if f&16:
                pose[b][:3]=unpack(data,cursor,'3f'); cursor+=12
        assert cursor == frame+(n+1)*stride
        poses.extend(pose)
    print(wanted, frames, 'frames', fps, 'fps')
    return fps,frames,poses


def build(vpk, output):
    from PIL import Image
    mdl=vpk.read('models/infected/hunter.mdl')
    vvd=vpk.read('models/infected/hunter.vvd')
    vtx=vpk.read('models/infected/hunter.dx90.vtx')
    assert unpack(mdl,4,'i')[0]==49 and unpack(vtx,0,'i')[0]==7
    assert unpack(mdl,8,'i')[0]==unpack(vvd,8,'i')[0]==unpack(vtx,16,'i')[0]
    bones=bones_from_mdl(mdl)
    # Assign mesh regions from the actual bone hierarchy/hitgroups.
    def group(name):
        if 'Head' in name or 'Neck' in name: return 0
        if '_L_' in name: return 5 if any(x in name for x in ('Thigh','Calf','Foot','Toe')) else 3
        if '_R_' in name: return 6 if any(x in name for x in ('Thigh','Calf','Foot','Toe')) else 4
        return 2 if name.endswith(('Pelvis','Spine','Spine1')) else 1
    groups=[group(b['name']) for b in bones]
    nv=unpack(vvd,16,'i')[0]
    fix_count,fix_base,vert_base=unpack(vvd,48,'iii')
    order=[]
    for i in range(fix_count):
        lod,first,count=unpack(vvd,fix_base+12*i,'iii')
        order.extend(range(first,first+count))
    if not order: order=list(range(nv))
    assert len(order)==nv
    vertices=[]
    for i in order:
        p=vert_base+i*48
        weights=unpack(vvd,p,'3f'); ids=unpack(vvd,p+12,'3B'); num=vvd[p+15]
        assert 0<num<=3 and all(b<len(bones) for b in ids)
        vertices.append((*unpack(vvd,p+16,'8f'),*weights,*ids))
    # Only body model zero, top LOD; split triangles by anatomical group.
    body_count,body_base=unpack(mdl,232,'ii')
    vbody_count,vbody_base=unpack(vtx,28,'ii')
    assert body_count==vbody_count==1
    model=body_base+unpack(mdl,body_base+12,'i')[0]
    meshes,mesh_base=unpack(mdl,model+72,'ii'); mesh_base+=model
    model_vertex=unpack(mdl,model+84,'i')[0]//48
    vm=vbody_base+unpack(vtx,vbody_base+4,'i')[0]
    lod=vm+unpack(vtx,vm+4,'i')[0]
    mesh_count,vm_base=unpack(vtx,lod,'ii'); vm_base+=lod
    assert meshes==mesh_count
    triangles=[[] for _ in range(7)]
    for m in range(meshes):
        mp=mesh_base+116*m
        assert unpack(mdl,mp,'i')[0]==0
        vbase=model_vertex+unpack(mdl,mp+12,'i')[0]
        vp=vm_base+9*m
        count,base=unpack(vtx,vp,'ii'); base+=vp
        for g in range(count):
            p=base+25*g
            numv,vo,ni,io,ns,so=unpack(vtx,p,'6i')
            mapping=[vbase+unpack(vtx,p+vo+9*i+4,'H')[0] for i in range(numv)]
            indices=unpack(vtx,p+io,f'{ni}H')
            for s in range(ns):
                sp=p+so+s*27
                sn,si=unpack(vtx,sp,'ii')
                assert vtx[sp+18]&1  # Trilist
                for k in range(si,si+sn,3):
                    tri=[mapping[indices[k+j]] for j in range(3)]
                    assert all(i<nv for i in tri)
                    totals=[0.]*7
                    for i in tri:
                        for weight,bone in zip(vertices[i][8:11],vertices[i][11:]):
                            totals[groups[bone]]+=weight
                    triangles[max(range(7),key=lambda g:totals[g])].extend(tri)
    am=vpk.read('models/infected/anim_hunter.mdl'); ani=vpk.read('models/infected/anim_hunter.ani')
    ab=bones_from_mdl(am)
    assert [b['name'] for b in ab]==[b['name'] for b in bones]
    clips=[animation(am,ani,ab,name) for name in ('@Idle_Standing_01','a_Walk_Upper_KnifeN','a_RunN')]
    # A small validated binary avoids parsing engine assets on the game thread.
    output.mkdir(parents=True,exist_ok=True)
    with (output/'hunter-model.bin').open('wb') as f:
        f.write(struct.pack('<4s4I',b'HPR1',len(bones),len(vertices),sum(map(len,triangles)),len(clips)))
        for b in bones:
            skeleton=not ('Finger' in b['name'])
            f.write(struct.pack('<iI12f',b['parent'],int(skeleton),*b['inverse']))
        for v in vertices: f.write(struct.pack('<11f3I',*v))
        for indices in triangles:
            f.write(struct.pack('<I',len(indices)))
            f.write(struct.pack(f'<{len(indices)}I',*indices))
        for fps,frames,poses in clips:
            f.write(struct.pack('<fI',fps,frames))
            for pose in poses: f.write(struct.pack('<7f',*pose))
    # Read a 512px mip of the original DXT1 texture. Pillow decodes BC1 blocks.
    tex=vpk.read('materials/models/infected/hunter/hunter_01.vtf')
    assert unpack(tex,4,'II')==(7,4) and unpack(tex,52,'I')[0]==13
    width,height=unpack(tex,16,'HH'); mip_count=tex[56]
    resources=unpack(tex,68,'I')[0]
    image_offset=None
    for i in range(resources):
        tag,where=unpack(tex,80+8*i,'II')
        if tag==0x30: image_offset=where
    assert image_offset is not None
    for mip in range(mip_count-1,-1,-1):
        w,h=max(1,width>>mip),max(1,height>>mip)
        size=max(1,(w+3)//4)*max(1,(h+3)//4)*8
        if w==512:
            image=Image.frombytes('RGBA',(w,h),tex[image_offset:image_offset+size],'bcn',(1,'DXT1'))
            image.convert('RGB').save(output/'hunter-diffuse.png')
            break
        image_offset+=size
    else: raise ValueError('512px mip unavailable')
    print('Hunter packed:',len(vertices),'vertices,',sum(map(len,triangles))//3,'triangles,',len(bones),'bones')


if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('game', type=Path)
    parser.add_argument('--output', type=Path, default=Path('assets'))
    args = parser.parse_args()
    vpk = Vpk(args.game / 'left4dead2/pak01_dir.vpk')
    build(vpk,args.output)
