#pragma once
#define _USE_MATH_DEFINES
#include <math.h>
#include <iostream>
#include "../../core/ctx/ctx.h"
#include "../../sdk/classes/entity.h"
#include "../../framework/cfg/cfg.hpp"
#include "../../sdk/classes/view.hpp"
#include "../../hacks/features/rcs/rcs.h"
#include "mouse.hpp"

extern "C" {
#include "../../sdk/classes/entity.h"
}

namespace aimbot_t
{
    inline int hotkey = VK_LBUTTON;
    inline float AimFov = 0.f;
    inline float Smooth = 2.0f;
    inline Vec2 RCSScale = { 1.f, 1.f };
    inline std::vector<int> hotkey_list{ VK_LBUTTON, VK_LMENU, VK_RBUTTON, VK_XBUTTON1, VK_XBUTTON2, VK_CAPITAL, VK_LSHIFT, VK_LCONTROL };

    inline void set_hotkey(int Index)
    {
        hotkey = hotkey_list.at(Index);
    }

    inline void switch_toggle()
    {
        angel::_settings->aimbot_always = !angel::_settings->aimbot_always;
    }

    inline float qfloor(float x)
    {
        if (x >= 0.0f)
            return (float)((int)x);
        return (float)((int)x - 1);
    }

    inline float qfmodf(float a, float b)
    {
        return (a - b * qfloor(a / b));
    }

    inline void vec_clamp(Vec3* v)
    {
        if (v->x > 89.0f && v->x <= 180.0f) {
            v->x = 89.0f;
        }
        if (v->x > 180.0f) {
            v->x = v->x - 360.0f;
        }
        if (v->x < -89.0f) {
            v->x = -89.0f;
        }
        v->y = qfmodf(v->y + 180, 360) - 180;
        v->z = 0;
    }

    inline void aimbot(const CEntity& local, Vec3 local_pos, Vec3 aim_pos)
    {
        if (angel::_settings->show_menu)
            return;

        if (aim_pos == Vec3(0, 0, 0))
            return;

        if (local.Pawn.Health <= 0)
            return;

        if (local.Pawn.weapon_type == cs_weapon_type::weapon_type_knife)
            return;
        if (local.Pawn.weapon_type == cs_weapon_type::weapon_type_grenade)
            return;
        if (local.Pawn.weapon_type == cs_weapon_type::weapon_type_c4)
            return;
        if (local.Pawn.weapon_type == cs_weapon_type::weapon_type_taser)
            return;

        float yaw, pitch, distance, norm, length;
        Vec3 opp_pos;
        Vec2 angles{};
        int screen_center_x = Gui.Window.Size.x / 2;
        int screen_center_y = Gui.Window.Size.y / 2;
        float target_x = 0.f;
        float target_y = 0.f;

        opp_pos = aim_pos - local_pos;

        distance = sqrt(pow(opp_pos.x, 2) + pow(opp_pos.y, 2));

        length = sqrt(distance * distance + opp_pos.z * opp_pos.z);

        if (angel::_settings->rcs)
        {
            rcs::update_angles(local, angles);
            float rad = angles.x * RCSScale.x / 180.f * M_PI;
            float si = sinf(rad);
            float co = cosf(rad);

            float z = opp_pos.z * co + distance * si;
            float d = (distance * co - opp_pos.z * si) / distance;

            rad = -angles.y * RCSScale.y / 180.f * M_PI;
            si = sinf(rad);
            co = cosf(rad);

            float x = (opp_pos.x * co - opp_pos.y * si) * d;
            float y = (opp_pos.x * si + opp_pos.y * co) * d;

            opp_pos = Vec3{ x, y, z };

            aim_pos = local_pos + opp_pos;

            if (angel::_settings->rcs && (local.Pawn.ShotsFired > rcs::rcs_bullet))
            {
                AimFov = angel::_settings->rcsfov;
            }
            else
            {
                AimFov = angel::_settings->aimbotfov;
            }
        }

        if (angel::_settings->rcs && local.Pawn.ShotsFired > 0)
        {
            if (local.Pawn.weapon_type == cs_weapon_type::weapon_type_sniper_rifle)
            {
                angel::_settings->rcs = false;
            }
            if (local.Pawn.weapon_type == cs_weapon_type::weapon_type_pistol)
            {
                if (local.Pawn.ShotsFired < 2)
                {
                    angel::_settings->rcs = false;
                }
                else
                {
                    angel::_settings->rcs = true;
                }
            }
        }
        else
        {
            angel::_settings->rcs = true;
        }

        if (angel::_settings->weapon_config)
        {
            if (local.Pawn.weapon_type == cs_weapon_type::weapon_type_sniper_rifle)
            {
                AimFov = angel::_settings->sniper_fov;
                Smooth = angel::_settings->sniper_smooth;
            }
            if (local.Pawn.weapon_type == cs_weapon_type::weapon_type_pistol)
            {
                AimFov = angel::_settings->pistol_fov;
                Smooth = angel::_settings->pistol_smooth;
            }
            else if (angel::_settings->rcs && (local.Pawn.ShotsFired > rcs::rcs_bullet))
            {
                AimFov = angel::_settings->rcsfov;
            }
            if (local.Pawn.weapon_type == cs_weapon_type::weapon_type_rifle)
            {
                AimFov = angel::_settings->rifle_fov;
                Smooth = angel::_settings->rifle_smooth;
            }
            else if (angel::_settings->rcs && (local.Pawn.ShotsFired > rcs::rcs_bullet))
            {
                AimFov = angel::_settings->rcsfov;
            }
            if (local.Pawn.weapon_type == cs_weapon_type::weapon_type_machinegun)
            {
                AimFov = angel::_settings->heavysmg_fov;
                Smooth = angel::_settings->heavysmg_smooth;
            }
            else if (angel::_settings->rcs && (local.Pawn.ShotsFired > rcs::rcs_bullet))
            {
                AimFov = angel::_settings->rcsfov;
            }
            if (local.Pawn.weapon_type == cs_weapon_type::weapon_type_submachinegun)
            {
                AimFov = angel::_settings->heavysmg_fov;
                Smooth = angel::_settings->heavysmg_smooth;
            }
            else if (angel::_settings->rcs && (local.Pawn.ShotsFired > rcs::rcs_bullet))
            {
                AimFov = angel::_settings->rcsfov;
            }
            if (local.Pawn.weapon_type == cs_weapon_type::weapon_type_shotgun)
            {
                AimFov = angel::_settings->heavysmg_fov;
                Smooth = angel::_settings->heavysmg_smooth;
            }
        }
        else if (!angel::_settings->rcs && (local.Pawn.ShotsFired < rcs::rcs_bullet))
        {
            AimFov = angel::_settings->aimbotfov;
            Smooth = aimbot_t::Smooth;
        }

        yaw = atan2f(opp_pos.y, opp_pos.x) * 57.295779513 - local.Pawn.ViewAngle.y;
        pitch = -atan(opp_pos.z / distance) * 57.295779513 - local.Pawn.ViewAngle.x;
        norm = sqrt(pow(yaw, 2) + pow(pitch, 2));

        Vec2 screen_pos;
        g_game.view.WorldToScreen(Vec3(aim_pos), screen_pos);

        if (norm < AimFov)
        {
            if (!Smooth)
            {
                _driver->MouseMove((DWORD)(target_x), (DWORD)(target_y));
                return;
            }

            // Dynamic AimSmooth based on distance
            float DistanceRatio = norm / AimFov;// Calculate the distance ratio
            float SpeedFactor = 1.0f + (1.0f - DistanceRatio); // Determine the speed factor based on the distance ratio
            target_x /= (Smooth * SpeedFactor);
            target_y /= (Smooth * SpeedFactor);
            
            if (screen_pos.x != screen_center_x)
            {
                target_x = (screen_pos.x > screen_center_x) ? -(screen_center_x - screen_pos.x) : screen_pos.x - screen_center_x;
                target_x /= Smooth != 0.0f ? Smooth : 1.5f;
                target_x = (target_x + screen_center_x > screen_center_x * 2 || target_x + screen_center_x < 0) ? 0 : target_x;
            }

            if (screen_pos.y != 0)
            {
                if (screen_pos.y != screen_center_y)
                {
                    target_y = (screen_pos.y > screen_center_y) ? -(screen_center_y - screen_pos.y) : screen_pos.y - screen_center_y;
                    target_y /= Smooth != 0.0f ? Smooth : 1.5f;
                    target_y = (target_y + screen_center_y > screen_center_y * 2 || target_y + screen_center_y < 0) ? 0 : target_y;
                }
            }
            _driver->MouseMove(target_x, target_y);
        }
    }
}