// Reconstruction for review. Not compiled or integrated into the project.
// Original: RZL4D2_[unknowncheats.me]_.dll
// SHA256: dfa8632e85c89c318577ef619f20bae2f4db5136042d680ee4dae4109234691a
// Addresses use the preferred image base 0x10000000.
// Native interfaces and calling conventions below are symbolic.

// 0x10026BB0; exact table read from 0x10185140 via IDA MCP.
bool NoSpread_ShouldRun(Player* player, Weapon* weapon, UserCmd* cmd)
{
    if ((cmd->buttons & 0x21) != 1) return false; // attack; no use
    if (player->hanging_from_ledge || player->hanging_from_tongue) return false;
    switch (weapon->GetWeaponID()) { // vtable + 0x5FC
    case 1: case 2: case 5: case 6: case 7: case 9: case 10:
    case 26: case 32: case 33: case 34: case 35: case 36: case 37:
        return true;
    default:
        return false; // Includes ALL shotguns: 3, 4, 8, 11.
    }
}

// 0x10026D60. Presentation flags at the end are summarized, not expanded.
void NoSpread_ApplyShotAngles(UserCmd* cmd)
{
    auto* player = GetLocalPlayer();
    auto* weapon = GetActiveWeapon();
    if (!player || !weapon || menu_open) return;
    if (!NoSpread_ShouldRun(player, weapon, cmd)) return;
    if (!Weapon_CanFireFromTickbase(weapon, player)) return;

    auto angle = cmd->angles;
    if (NoSpread_Enabled) { // 0x103F4BF2, configuration key bNoSpread
        const float saved = weapon->current_spread; // +0xD0C
        NativeUpdateSpread(weapon);
        const float spread = weapon->current_spread;
        angle.pitch -= SharedRandomFloat(
            "CTerrorGun::FireBullet HorizSpread", -spread, spread, 0);
        angle.yaw -= SharedRandomFloat(
            "CTerrorGun::FireBullet VertSpread", -spread, spread, 0);
        weapon->current_spread = saved;
        angle -= player->PunchAngles(); // vtable +0x4DC
    }
    angle.pitch = clamp(normalize360(angle.pitch), -89.f, 89.f);
    angle.yaw = normalize360(angle.yaw);
    angle.roll = 0.f;
    FixMovement(cmd, angle);
    cmd->angles = angle;
    UpdateAnglePresentationFlags(); // no additional pellet correction
}

// Order verified in CreateMove (0x1016FF00):
void RelevantCreateMoveOrder(UserCmd* cmd)
{
    PredictionBegin(cmd); // 0x10170117
    // Global seed = MD5_PseudoRandom(cmd->number) & 0x7FFFFFFF;
    // predicted time/frame time; SetupMove/ProcessMovement/FinishMove.
    // Other features execute here too.
    NoSpread_ApplyShotAngles(cmd); // 0x101701CB
    PredictionEnd(); // 0x101701D6: restore clock/current-command; seed=-1.
}
