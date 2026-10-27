#pragma once
#include <array>
#include <string_view>

namespace weapons {
struct Definition {
    const char* key;
    const char* name;
    const char* entity;
    int group; // Existing hitscan policy: pistol, SMG, rifle, shotgun, sniper, M60.
    bool semi;
    int image{};
    const char* model{};
};
inline constexpr auto Catalog=std::to_array<Definition>({
    {"pistol","Pistol","CPistol",0,true},
    {"pistol_dual","Dual pistols","CPistol",0,true},
    {"magnum","Magnum","CMagnumPistol",0,true},
    {"smg","Uzi","CSubMachinegun",1,false,306},
    {"smg_silenced","Silenced SMG","CSMG_Silenced",1,false,305},
    {"smg_mp5","MP5","CSMG_MP5",1,false},
    {"pumpshotgun","Pump shotgun","CPumpShotgun",3,true,304},
    {"shotgun_chrome","Chrome shotgun","CShotgun_Chrome",3,true,303},
    {"autoshotgun","Auto shotgun","CAutoShotgun",3,true,302},
    {"shotgun_spas","SPAS-12","CShotgun_SPAS",3,true},
    {"rifle","M16","CAssaultRifle",2,false},
    {"rifle_ak47","AK-47","CRifle_AK47",2,false,301},
    {"rifle_desert","Desert rifle","CRifle_Desert",2,false},
    {"rifle_sg552","SG 552","CRifle_SG552",2,false},
    {"hunting_rifle","Hunting rifle","CSniperRifle",4,true,309},
    {"sniper_military","Military sniper","CSniper_Military",4,true,307},
    {"sniper_scout","Scout","CSniper_Scout",4,true},
    {"sniper_awp","AWP","CSniper_AWP",4,true},
    {"rifle_m60","M60","CRifle_M60",5,false},
    {"grenade_launcher","Grenade launcher","CGrenadeLauncher",-1,false},
    {"chainsaw","Chainsaw","CChainsaw",-1,false},
    {"baseball_bat","Baseball bat","CTerrorMeleeWeapon",-1,false,0,"baseball_bat"},
    {"cricket_bat","Cricket bat","CTerrorMeleeWeapon",-1,false,0,"cricket_bat"},
    {"crowbar","Crowbar","CTerrorMeleeWeapon",-1,false,0,"crowbar"},
    {"electric_guitar","Electric guitar","CTerrorMeleeWeapon",-1,false,0,"electric_guitar"},
    {"fireaxe","Fire axe","CTerrorMeleeWeapon",-1,false,0,"fireaxe"},
    {"frying_pan","Frying pan","CTerrorMeleeWeapon",-1,false,0,"frying_pan"},
    {"golfclub","Golf club","CTerrorMeleeWeapon",-1,false,0,"golfclub"},
    {"katana","Katana","CTerrorMeleeWeapon",-1,false,0,"katana"},
    {"knife","Knife","CTerrorMeleeWeapon",-1,false,0,"knife"},
    {"machete","Machete","CTerrorMeleeWeapon",-1,false,0,"machete"},
    {"tonfa","Tonfa","CTerrorMeleeWeapon",-1,false,0,"tonfa"},
    {"pitchfork","Pitchfork","CTerrorMeleeWeapon",-1,false,0,"pitchfork"},
    {"shovel","Shovel","CTerrorMeleeWeapon",-1,false,0,"shovel"},
    {"melee","Other / custom melee","CTerrorMeleeWeapon",-1,false},
    {"molotov","Molotov","CMolotov",-1,false},
    {"pipe_bomb","Pipe bomb","CPipeBomb",-1,false},
    {"vomitjar","Bile jar","CItem_VomitJar",-1,false},
    {"first_aid_kit","First aid kit","CFirstAidKit",-1,false},
    {"defibrillator","Defibrillator","CItemDefibrillator",-1,false},
    {"pain_pills","Pain pills","CPainPills",-1,false},
    {"adrenaline","Adrenaline","CItem_Adrenaline",-1,false},
    {"upgrade_explosive","Explosive ammo pack","CItemUpgradePackExplosive",-1,false},
    {"upgrade_incendiary","Incendiary ammo pack","CItemUpgradePackIncendiary",-1,false},
    {"gascan","Gas can","CGasCan",-1,false},
    {"propanetank","Propane tank","CPropaneTank",-1,false},
    {"oxygentank","Oxygen tank","COxygenTank",-1,false},
    {"fireworkcrate","Fireworks","CFireworkCrate",-1,false},
    {"cola","Cola bottles","CColaBottles",-1,false},
    {"gnome","Gnome","CGnome",-1,false}
});
inline constexpr size_t Count=Catalog.size();
inline bool IsShotgun(int index) noexcept {
    return index>=0 && static_cast<size_t>(index)<Count && Catalog[index].group==3;
}
inline int Find(std::string_view entity,std::string_view model={},bool dual=false) noexcept {
    const auto slash=model.find_last_of("/\\");
    if(slash!=std::string_view::npos) model.remove_prefix(slash+1);
    if(model.starts_with("w_") || model.starts_with("v_")) model.remove_prefix(2);
    if(model.ends_with(".mdl")) model.remove_suffix(4);
    if(model=="bat") model="baseball_bat";
    if(model=="knife_t") model="knife";
    int fallback=-1;
    for(size_t i=0;i<Count;++i) {
        const auto& w=Catalog[i];
        if(entity!=w.entity) continue;
        if(entity=="CPistol") return dual?1:0;
        if(!w.model) fallback=static_cast<int>(i);
        else if(model==w.model) return static_cast<int>(i);
    }
    return fallback;
}
}
