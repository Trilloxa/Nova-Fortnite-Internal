#pragma once
#include <Windows.h>
#include <cstdint>
#include <locale>
#include <String>
#include <Psapi.h>
#include <vector>
#include <d3d11.h>
#include <iostream>
#include <fstream>
#include <algorithm>

#include "crt.h"

#include "xorstr.h"
#define _(s) xorstr(s)

#include "spoof/spoof_call.h"
#include "spoof/syscall.h"

#include <d3d9.h>
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#include <dwmapi.h>

#pragma comment(lib, "d3d9.lib")

wchar_t* s2wc(const char* c)
{
	const size_t cSize = strlen(c) + 1;
	wchar_t* wc = new wchar_t[cSize];
	mbstowcs(wc, c, cSize);

	return wc;
}

#include "SDK/Structs/Structs.h"

namespace Settings {

	PVOID Module = 0;
	PVOID DiscordModule = 0;
	DWORD ImageSize = 0;

	int X, Y;
	float draw_y;
	bool Render = false;

	bool customtime = false;
	float dilation = 0.1f;

	namespace Bones
	{
		FName head,
			root,
			neck,
			Pelvis,
			Upperarm_l,
			Upperarm_r,
			Lowerarm_l,
			Lowerarm_r,
			Hand_l,
			Hand_r,
			Thigh_l,
			Thigh_r,
			Calf_l,
			Calf_r,
			Foot_l,
			Foot_r,
			Toe_l,
			Toe_r;
	}

	namespace Offsets {
		/* Pointers */
		std::uintptr_t UWorld = 0;
		std::uintptr_t gObjects = 0;


		/* Render */
		PVOID Font = 0;
		PVOID K2_DrawText = 0;
		PVOID K2_DrawLine = 0;
		PVOID K2_DrawBox = 0;
		PVOID KismetMathLibrary = 0;

		PVOID GetSocketBoneName = 0;
		PVOID GetSocketNames = 0;

		/* Functions */
		PVOID GetPlayerName = 0;
		PVOID SetLocation = 0;

		std::uintptr_t PC = 0;
		std::uintptr_t MP = 0;
		std::uintptr_t Mesh = 0;
		std::uintptr_t Target = 0;
	}

	namespace Aimbot
	{
		std::uintptr_t Aimbot_pawn;
		std::uintptr_t PC;
		std::uintptr_t UCanvas;
		FVector Loc;
		FRotator Rot;
		float FOV;
	}

	typedef struct settings {

		struct {
			bool Aimbot;

			bool Memory;
			bool Silent;
			float Smoothing = 0;

			bool target_teamates;
			bool visible;
			bool draw;

			bool draw_line;

			float FieldOV;

			bool baim;
			bool bodyaim;

			int aimbone;
			int aimkey;
		} Aimbot;

		struct {
			bool Skeleton;
			bool Boxesp;
			bool snaplines;
			bool headcircle;
			bool health;
			bool player_chams;
			bool name;
			bool distance;
			bool weapon;
			bool ammoo_count;
			bool tribe;
			bool level;
			bool weight;
			bool draw_corpses;
			bool show_armor;
			bool draw_self;

			bool supply_crate;
			bool dropped_item;
			bool dropped_by;
			bool item_cache;

			bool c4_charge;
			bool sleeping_stations;

			bool turret;
			bool turret_ammo_count;

			bool generators;

			bool laser;

			bool transmitter;
			bool dummy_nigga;
			bool cryofridge;

			bool storage;
			bool storage_item_count;
			bool storage_chams;

			bool world_chams;

			int world_distance;
			int dino_distance;
			int player_distance;

			bool draw_wild;
			bool draw_tamed;
			bool draw_dead;
			bool draw_level;
			bool draw_alpha;
			bool draw_health;
			bool draw_tribe;
			bool torpor;
			bool dino_chams;
			int minimum_level;

			bool force_wireframe;
			bool tridbox;

		} Visuals;
		struct {

			bool nosway;
			bool nospread;
			bool norecoil;
			bool instareload;

			bool rapidfire;
			bool freecam;
			bool infiniteoribt;
			bool long_arms;
			bool transmitter_exploit;

			bool fly;

			bool popcorn;
			bool drop_in_bag;

			bool chat_spammer;
			int msg_time;

			bool anti_net;

			bool infinite_element;
			bool no_overheat_rifle;

			bool no_tek_gloves_cooldown;
			bool auto_loot_bodies;
			bool chest_tek_;
			float tek_z_chest;

			bool unlock_dlc;
			float free_cam_speed;

			bool projectiletp;

			bool self_upload;

			bool tek_punch;
			bool tek_jump_boost;

			float rapid_fire_time;

			bool air_stuck;
			bool speed_hack;
			float speed_hack_time;

			bool free_tek_energy;
			bool no_engram;

			bool auto_heal;
			bool auto_drink;
			float heal_percentage;

			bool no_limit_rotation;
			bool ghost_mode;

			bool autoflak;
			bool legit_autoflak;
			bool rage_autoflak;
			int autoflak_key;

			bool infinite_weapon_range;

			int key_forward;
			int key_backward;
			int key_right;
			int key_left;

			bool fovchanger;
			float FOV;
			bool infinitestamina;
			bool nofall;
			bool insta_turn;

			bool crosshair;

			bool debug;
		} Exploits;
		struct {
			bool radar;
			bool fov;
			bool draw_player;
			bool draw_dino;
			bool draw_item;

			bool serverinfo;
			bool uploadtimer;
			bool serverfps;
			bool storages;

			int x;
			int y;
			int size;

		}Radar;
	}SETTINGS;

	SETTINGS Setings = { 0 };
}

#include "Utils/Utils.h"
#include "SDK/Structs/Objects.h"
#include "SDK/Functions.h"
#include "SDK/Gui.h"
