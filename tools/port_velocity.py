"""Extract the original UI into a build snapshot; preserve the adapted L4D2 sources."""
from pathlib import Path
import re, shutil

root = Path(__file__).resolve().parents[1]
ref = Path(r'C:\Users\henry\OneDrive\Desktop\velocity-main\velocity-main\cs2\velocity-cs2\project')
dest = root / 'build/velocity-original-import/velocity_ui'
dest.mkdir(parents=True, exist_ok=True)
def read(p): return (ref / p).read_text(encoding='utf-8-sig')
def write(p,s):
    p = dest / p
    p.parent.mkdir(parents=True,exist_ok=True)
    p.write_text(s,encoding='utf-8')
def section(s, begin, end): return s[s.index(begin):s.index(end)]
def replace_function(s, signature, new):
    pos=s.index(signature); opening=s.index('{',pos); depth=1; end=opening+1
    while depth:
        if s[end]=='{': depth+=1
        if s[end]=='}': depth-=1
        end+=1
    return s[:pos]+new+s[end:]

shutil.copytree(ref/'external/nlohmann',dest/'nlohmann',dirs_exist_ok=True)
shutil.copytree(ref/'external/lz4',dest/'lz4',dirs_exist_ok=True)
shutil.copytree(ref/'external/xdraw/dependencies/nanosvg',dest/'nanosvg',dirs_exist_ok=True)

common='''#pragma once
#include <windows.h>
#include <windowsx.h>
#include <wrl/client.h>
#include <d3d9.h>
#include <algorithm>
#include <array>
#include <chrono>
#include <cmath>
#include <cctype>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <cwchar>
#include <functional>
#include <iterator>
#include <limits>
#include <memory>
#include <numbers>
#include <optional>
#include <span>
#include <string>
#include <string_view>
#include <type_traits>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>
'''
write('common.hpp',common)
hdr=read('external/xdraw/xdraw.hpp').replace('#include <d3d11.h>','#include "common.hpp"')
hdr=hdr.replace('ID3D11ShaderResourceView','IDirect3DTexture9').replace('ID3D11Texture2D','IDirect3DTexture9').replace('D3D11_RECT','RECT')
hdr=hdr.replace('bool initialize( ID3D11Device* device, ID3D11DeviceContext* context );','bool initialize( IDirect3DDevice9* device );\n\tvoid invalidate_device_objects();\n\tvoid capture_blur();')
hdr=hdr.replace('ID3D11Device* device( )','IDirect3DDevice9* device( )')
write('xdraw.hpp',hdr)
hdr=read('external/xdraw/xui/xui.hpp').replace('"../xdraw.hpp"','"xdraw.hpp"').replace('bool rect::contains','bool contains')
hdr=hdr.replace('std::uintptr_t{ 14695981039346656037ull }','std::uintptr_t{ 2166136261u }').replace('1099511628211ull','16777619u')
write('xui.hpp',hdr)
xui=read('external/xdraw/xui/xui.cpp').replace('#define NOMINMAX\n','').replace('#include <pch/pch.hpp>','#include "common.hpp"')
xui=xui.replace('c.windows.clear( );','c.windows.clear( );\n\t\tc.windows.reserve(16);')
xui=xui.replace('1099511628211ull','16777619u')
write('xui.cpp',xui)

config=read('external/config.hpp').replace('#define NOMINMAX','#include "common.hpp"').replace('"xdraw/xui/xui.hpp"','"xui.hpp"')
config=config.replace('Software\\\\velokitty\\\\configs','Software\\\\FreeL4D2\\\\VelocityUI\\\\configs')
write('config.hpp',config)
settings=read('core/settings.hpp').replace('#include <utilities/math/math.hpp>','#include "common.hpp"').replace('#include <external/config.hpp>','#include "config.hpp"')
settings=re.sub(r'\n\s*(?:const )?weapon_group& get_group\( std::uint32_t weapon_type \)\s*(?:const\s*)?\{.*?\n\s*\}', '', settings, flags=re.S)
settings=settings.replace('this->flags[ f ]','this->flags.values[ static_cast<size_t>(f) ]')
write('settings.hpp',settings)

rh=read('core/rendering/rendering.hpp')
rh='#pragma once\n#include "xui.hpp"\nnamespace rendering {\n'+section(rh,'    class menu','\tclass widgets')+section(rh,'\tclass fonts','\tinline context g_context')+'inline menu g_menu{};\ninline fonts g_fonts{};\n}\n'
rh=rh.replace('ID3D11ShaderResourceView','IDirect3DTexture9').replace('void toggle( ) { this->m_open = !this->m_open; }','void toggle( ) { this->m_open = !this->m_open; }\n        void set_open(bool open) { this->m_open = open; }')
rh=rh.replace('float m_open_anim{ 1.0f };','float m_open_anim{ 0.0f };')
write('rendering.hpp',rh)

for page in ['core','ragebot','legitbot','player','world','misc','config','skins']:
    s=read(f'core/rendering/impl/menu/menu.{page}.cpp')
    s=re.sub(r'#include[^\n]*\n','',s)
    s='#include "common.hpp"\n#include "settings.hpp"\n#include "rendering.hpp"\n'+s
    if page=='core':
        s=s.replace('if ( !g_context.ui_assets_ready( ) )','if ( !this->m_intro_pack_textures_ready )').replace('const bool pack_ready = g_context.ui_assets_ready( );','const bool pack_ready = this->m_intro_pack_textures_ready;')
        s=replace_function(s,'void menu::finalize_textures_after_rpack( )','void menu::finalize_textures_after_rpack() { this->m_intro_pack_textures_ready = true; }')
        a=s.index('\t\tif ( this->m_last_open != this->m_open )'); b=s.index('\t\tif ( !this->m_intro_pack_textures_ready )',a)
        s=s[:a]+s[b:]
        s=replace_function(s,'void menu::shutdown( ) const','void menu::shutdown() const {}')
        s=replace_function(s,'void menu::apply_saved_cursor( )','void menu::apply_saved_cursor() {}')
        s=replace_function(s,'void menu::draw_menu_peek_decor( float wx, float wy, float ww, float wh, float reveal ) const','void menu::draw_menu_peek_decor(float, float, float, float, float) const {}')
        s=s.replace('this->m_textures.gif.update( dt );','')
    if page=='misc':
        s=s.replace('features::misc::impacts::list_custom_sounds( )','std::vector<std::string>{}')
        s=s.replace('features::misc::g_impacts.play_custom_sound( file_setting.value, preview_volume );','(void)preview_volume; // Interface only: no CS2 sound system.')
    if page=='skins':
        s='#include "catalog.hpp"\n'+s.replace('features::changer::','ui_catalog::')
    write(f'menu.{page}.cpp',s)

cat=read('core/features/changer/changer.hpp')
cat='#pragma once\n#include "common.hpp"\nnamespace ui_catalog {\n'+section(cat,'\tclass econ_item_system','\t\t[[nodiscard]] bool initialize( );')
cat=cat.replace('ID3D11ShaderResourceView','IDirect3DTexture9')
cat+='''
        const std::vector<paint_kit>& paint_kits() const { return m_paint_kits; }
        const std::vector<skin_entry>& skins() const { return m_skins; }
        const std::vector<const item_def*>& guns() const { return m_empty; }
        const std::vector<const item_def*>& knives() const { return m_empty; }
        const std::vector<const item_def*>& gloves() const { return m_empty; }
        const std::vector<const item_def*>& agents() const { return m_empty; }
        const item_def* find_def(std::int16_t) const { return nullptr; }
        const paint_kit* find_paint_kit(int) const { return nullptr; }
        const skin_image* get_skin_image(const std::string&) const { return nullptr; }
        const skin_image* get_skin_image(std::int16_t,int) const { return nullptr; }
        int combined_rarity(std::int16_t,int) const { return 0; }
    private:
        // This menu port has no CS2 schema, inventory, VPK parser or network fetch.
        std::vector<paint_kit> m_paint_kits;
        std::vector<skin_entry> m_skins;
        std::vector<const item_def*> m_empty;
    };
    inline econ_item_system g_econ_item_system;
}
'''
write('catalog.hpp',cat)

# Embed the exact fonts in the DLL. The original primary Inter is bundled locally.
assets='// Fonts from Velocity and its resource repository; kept in static storage.\nnamespace velocity_assets {\n'
for path,name,outname in [(ref/'external/xdraw/dependencies/fonts.hpp','inter','inter_medium'),(ref/'external/xdraw/dependencies/fonts.hpp','noto_math','noto_math'),(root/'vendor/velocity-resourcer/project/core/resources/fonts/inter.hpp','bold','inter_bold'),(root/'vendor/velocity-resourcer/project/core/resources/fonts/pixel7.hpp','smallest','pixel7')]:
    text=path.read_text(encoding='utf-8-sig')
    m=re.search(r'inline unsigned char '+name+r'\[\s*(\d+)\s*\]\s*\{(.*?)\};',text,re.S)
    if not m: raise RuntimeError(name)
    assets+=f'inline const unsigned char {outname}[{m[1]}]{{{m[2]}}};\n'
assets+='}\n'
write('font_assets.hpp',assets)

# Preserve the source's tessellation, antialiasing, text placement and clip commands.
xd=read('external/xdraw/xdraw.cpp')
primitives=section(xd,'\tvoid draw_list::clear( )','\tvoid gif_image::update( float dt )')
primitives=primitives.replace('ID3D11ShaderResourceView','IDirect3DTexture9').replace('D3D11_RECT','RECT')
write('draw_primitives.inl',primitives)
font=section(xd,'\tfont::~font( )','\tvoid draw_list::clear( )')
font=replace_function(font,'void font::flush_atlas( )','''void font::flush_atlas() {
    if (!atlas_dirty || !atlas_tex) return;
    D3DLOCKED_RECT lock{};
    if (FAILED(atlas_tex->LockRect(0,&lock,nullptr,0))) return;
    for (int y=0;y<atlas_h;++y)
        std::memcpy(static_cast<unsigned char*>(lock.pBits)+y*lock.Pitch,
                    atlas_bitmap.data()+static_cast<size_t>(y)*atlas_w*4,static_cast<size_t>(atlas_w)*4);
    atlas_tex->UnlockRect(0);
    atlas_dirty=false;
}''')
write('font.inl',font)
loadfont=section(xd,'\tfont* load_font( std::span<const std::byte> data','\tvoid push_font( font* f )')
a=loadfont.index('\t\tD3D11_TEXTURE2D_DESC td{};'); b=loadfont.index('\t\tf->atlas_dirty = false;',a)
loadfont=loadfont[:a]+'''        f->atlas_tex=create_srv_from_rgba(f->atlas_bitmap.data(),atlas_w,atlas_h);
        if (!f->atlas_tex) return nullptr;
        f->atlas_srv=f->atlas_tex;
'''+loadfont[b:]
write('load_font.inl',loadfont)
write('font_stack.inl',section(xd,'\tvoid push_font( font* f )','\tstd::pair<int, int> viewport_size( )'))
write('decode_utf8.inl',section(xd,'\t\tstatic char32_t decode_utf8','\t} // namespace detail'))
print('Imported UI sources and embedded fonts into',dest)
