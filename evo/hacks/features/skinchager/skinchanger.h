#pragma once
#include "../../sdk/classes/entity.h"

namespace skinchanger
{

    int get_id(uintptr_t weapons)
    {
        int id;
        _proc_manager.read_memory(weapons + Offset::EconEntity.AttributeManager + Offset::WeaponBaseData.Item + Offset::WeaponBaseData.ItemDefinitionIndex, id);

        return id;
    }

    void run(const CEntity& LocalPlayer, CGame Game) 
    {
        uintptr_t weapon_service;
        _proc_manager.read_memory(LocalPlayer.Pawn.Address + Offset::Pawn.WeaponServices, weapon_service);

        int weapon_size;
        _proc_manager.read_memory(weapon_service + Offset::WeaponBaseData.WeaponSize, weapon_size);

        auto weapon_handler = [&](int i) -> int 
            {
            int handle;
            _proc_manager.read_memory((weapon_service + Offset::WeaponBaseData.ActiveWeapon) + 0x4 * i, handle);

            return handle;
            };

       // int32_t WeaponPaintKit;
       // _proc_manager.read_memory<int32_t>(LocalPlayer.Controller.Address + Offset::EconEntity.FallbackPaintKit, WeaponPaintKit);
       // uintptr_t dwNetworkGameClient;
       // _proc_manager.read_memory<uintptr_t>(Game.get_engine_dll_address() + 0x512AC8, dwNetworkGameClient);
       // int32_t DeltaTick;
       // int delta = -1;
       // _proc_manager.read_memory<int32_t>(dwNetworkGameClient + 0x258, DeltaTick);
       // _proc_manager.write_memory<int>(dwNetworkGameClient + 0x258, DeltaTick);

        auto get_controler_from_handle = [&](uintptr_t entity_list, int c_handle) -> uintptr_t 
            {
            c_handle = c_handle & 0x7FFF;

            uintptr_t list;
            _proc_manager.read_memory(entity_list + 8 * (c_handle >> 9) + 16, list);

            auto controller = list + 120 * (c_handle & 0x1FF);

            uintptr_t r_controler;
            _proc_manager.read_memory(controller, r_controler);

            return r_controler;
            };

        uintptr_t entity_list_pass;
        _proc_manager.read_memory(Game.get_client_dll_address() + Offset::EntityList, entity_list_pass);

        auto get_game_scene = [&](uintptr_t c_handle) -> uintptr_t 
            {
            uintptr_t scene;
            _proc_manager.read_memory(c_handle + Offset::Pawn.GameSceneNode, scene);

            return scene;
            };

        auto set_mask_group = [&](uintptr_t scene) -> int 
            {
            int mesh;
            _proc_manager.read_memory(scene + 0x160 + Offset::WeaponBaseData.m_MeshGroupMask, mesh);

            return mesh;
            };

        auto get_view_handle = [&]() -> int 
            {
            uintptr_t view_model_service;
            _proc_manager.read_memory(LocalPlayer.Pawn.Address + Offset::Pawn.ViewModelServices, view_model_service);

            int h_viewmodel;
            _proc_manager.read_memory(view_model_service + Offset::Pawn.ViewModel, h_viewmodel);

            return h_viewmodel;
            };

        static bool wrote = false;

        auto set_weapon_skin = [&](int weapon_id, uintptr_t weapon,
            uintptr_t scene, int mesh, int item_id_high) -> void 
            {
                auto update = [&]() -> void 
                    {
                    int to_write = 3; //2?

                    auto view_handle = get_view_handle();
                    auto view_model = get_controler_from_handle(entity_list_pass, view_handle);

                    auto view_node = get_game_scene(view_model);
                    auto view_mask = set_mask_group(view_node);


                        _proc_manager.write_memory<int>(scene + 0x160 + Offset::WeaponBaseData.m_MeshGroupMask, to_write); //160 - m_modelState
                        _proc_manager.write_memory<int>(view_node + 0x160 + Offset::WeaponBaseData.m_MeshGroupMask, to_write);
                    };

                if (mesh != 3)
                {
                    update();
              
                }

                if (weapon_id == 9) 
                {
                    int paint = 38; // 361 abyss ssg, 36 fade
                    _proc_manager.write_memory<int>(weapon + Offset::EconEntity.FallbackPaintKit, paint); //paint
                }

               // if (WeaponPaintKit != 38 && DeltaTick != -1) {
               //    _proc_manager.write_memory<int>(dwNetworkGameClient + 0x258, delta);
               // }

                // global fallback
                int seed = 0;
                int stat = 1337;
                float wear = 0.00001f;
                std::string nametag = "angeldancy";
                int quality = 2;
                int high = -1;

                _proc_manager.write_memory<int>(weapon + Offset::EconEntity.FallbackSeed, seed); //seed
                _proc_manager.write_memory<int>(weapon + Offset::EconEntity.FallbackStatTrak, stat); //stat
                _proc_manager.write_memory<float>(weapon + Offset::EconEntity.FallbackWear, wear); //wear
                _proc_manager.write_memory<std::string>(weapon + Offset::EconEntity.AttributeManager + Offset::WeaponBaseData.Item + Offset::EconEntity.szCustomName, nametag);
                _proc_manager.write_memory<int>(weapon + Offset::EconEntity.AttributeManager + Offset::WeaponBaseData.Item + Offset::EconEntity.EntityQuality, quality);

                //Set item id high
                _proc_manager.write_memory<int>(weapon + Offset::EconEntity.AttributeManager + Offset::WeaponBaseData.Item + Offset::EconEntity.ItemIDHigh, high);
            };

        for (int i = 0; i < weapon_size; i++) 
        {
            auto weapon_handle = weapon_handler(i);
            if (!weapon_handle) {
                //print_error( "mem 1" );
                continue;
            }

            auto weapon = get_controler_from_handle(entity_list_pass, weapon_handle);
            if (!weapon) 
            {
                //print_error( "mem 2" );
                continue;
            }

            auto weapon_id = get_id(weapon);
            auto weapon_scene = get_game_scene(weapon);
            auto weapon_mesh = set_mask_group(weapon_scene);



            set_weapon_skin(weapon_id, weapon, weapon_scene, weapon_mesh, -1);
        }
    }
}
