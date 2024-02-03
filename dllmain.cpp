#include "stdafx.h"

void(*Postrender)(void*, void*) = nullptr;

float(*GetFOVangle)(void*) = nullptr;

float hkGetFOVAngle(void* this_)
{
	if (Settings::Setings.Exploits.fovchanger)
		return Settings::Setings.Exploits.FOV;

	return GetFOVangle(this_);
}

#define ViewYawMin 0x245C
#define ViewYawMax 0x2460
#define ViewPitchMin 0x2454
#define ViewPitchMax 0x2458

void hkPostRender(void* Viewport, void* Canvas) {

	float closestDistance = FLT_MAX;
	uintptr_t closestPawn = 0;
	int NearActors = 0;

	GUI::SetupCanvas(Canvas);

	Settings::Aimbot::UCanvas = (uintptr_t)Canvas;

	if ((nt::GetKey(VK_PRIOR) & 1) && Settings::Setings.Aimbot.aimbone < 2)
		Settings::Setings.Aimbot.aimbone++;
	if ((nt::GetKey(VK_NEXT) & 1) && Settings::Setings.Aimbot.aimbone > 0)
		Settings::Setings.Aimbot.aimbone--;

	do {
		if (Canvas) {

			uintptr_t GWorld = Utils::read<uintptr_t>(Settings::Offsets::UWorld);
			if (!GWorld) break;

			uintptr_t PersLevel = Utils::read<uintptr_t>(GWorld + 0x30);
			if (!PersLevel) break;

			uintptr_t Gameinstance = Utils::read<uintptr_t>(GWorld + 0x160);
			if (!Gameinstance) break;

			uintptr_t LocalPlayers = Utils::read<uintptr_t>(Gameinstance + 0x38);
			if (!LocalPlayers) break;

			uintptr_t LocalPlayer = Utils::read<uintptr_t>(LocalPlayers);
			if (!LocalPlayer) break;

			if (Settings::Setings.Exploits.debug) {
				std::string boz = xorstr("bone: ");
				boz += Engine::Aimbot::g[Settings::Setings.Aimbot.aimbone].first;
				draw_text(boz.data(), { (float)Settings::X / 2, Settings::draw_y }, Colors::White, true);
				Settings::draw_y += 18.f;
			}

			uintptr_t PC = Utils::read<uintptr_t>(LocalPlayer + 0x30);
			Settings::Aimbot::PC = PC;
			if (PC)
			{
				Engine::Getscreensize(&Settings::X, &Settings::Y);
				//GPVP::Hook_viewpoint(PC);
			}
			if (!PC) break;

			uintptr_t PCM = Utils::read<uintptr_t>(PC + 0x3D0);
			if (!PCM) break;
			Utils::InstallVtableHook((void*)PCM, hkGetFOVAngle, 0xD3, (void**)&GetFOVangle);

			uintptr_t MyPawn = Utils::read<uintptr_t>(PC + 0x3B8);
			Settings::Offsets::MP = MyPawn;
			//if( !MyPawn ) break;
			if (MyPawn)
			{
				draw_text(Utils::GetObjectNameByIndex(*(int*)(MyPawn + 0x18)).c_str(), { (float)Settings::X / 2, (float)Settings::draw_y }, Colors::Yellow, true);
			}

			Settings::Aimbot::FOV = hkGetFOVAngle((void*)PCM);
			Settings::Aimbot::Loc = Engine::GetCameraLocation(PCM);
			Settings::Aimbot::Rot = Engine::GetCameraRotation(PCM);
			Settings::draw_y = Settings::Y / 6;

			static uintptr_t Player_class = 0;
			if (!Player_class) Player_class = (uintptr_t)GObjects->FindObject(xorstr("Class FortniteGame.FortPlayerPawnAthena"));

			static uintptr_t RangedWep_class = 0;
			if (!RangedWep_class) RangedWep_class = (uintptr_t)GObjects->FindObject(xorstr("Class FortniteGame.FortWeaponRanged"));

			static uintptr_t Projectile_class = 0;
			if (!Projectile_class) Projectile_class = (uintptr_t)GObjects->FindObject(xorstr("Class FortniteGame.FortProjectileBase"));

			auto Levels = *(TArray<uintptr_t>*)(GWorld + 0x138);
			for (int level = 0; level < Levels.Num(); ++level) {
				if (!Levels.IsValidIndex(level)) continue;

				auto current_level = Levels[level];
				if (!current_level) continue;

				auto Actors = *(TArray<uintptr_t>*)(current_level + 0x98);
				if (!Actors.Num()) break;

				for (int index = 0; index < Actors.Num(); ++index) {
					if (!Actors.IsValidIndex(index)) continue;

					uintptr_t current_actor = Actors[index];
					if (!current_actor) continue;

					if (!Settings::Setings.Visuals.draw_self) {
						if (current_actor == MyPawn) continue;
					}

					if (Engine::IsA(current_actor, Player_class))
					{ // 4444 was here
						uintptr_t mesh = Utils::read<uintptr_t>(current_actor + 0x390);
						if (!mesh) continue;

						Settings::Offsets::Mesh = Utils::read<uintptr_t>(mesh);

						uintptr_t CharacterMovement = Utils::read<uintptr_t>(current_actor + 0x398);
						if (!CharacterMovement) continue; 

						uintptr_t root = Utils::read<uintptr_t>(current_actor + 0x158);
						if (!root) continue;

						uintptr_t state = Utils::read<uintptr_t>(current_actor + 0x350);
						if (!state) continue;

						uintptr_t skeletal_mesh = Utils::read<uintptr_t>(mesh + 0x5A0);
						if (!skeletal_mesh) continue;

						FVector location = Engine::GetLocation(current_actor);
						auto world_to_screen_nig = Engine::WorldToScreen(location);

						auto fafaf = Engine::GetSocketLocation(mesh, Settings::Bones::head);
						if (!fafaf.valid()) continue;

						auto aimbotBone = Engine::GetSocketLocation(mesh, Engine::Aimbot::GetBone());
						if (!aimbotBone.valid()) continue;

						auto aimbotBonew2s = Engine::WorldToScreen(aimbotBone);
						if (!aimbotBonew2s.valid()) continue;

						auto head = Engine::WorldToScreen(fafaf);
						if (!head.valid()) continue;

						FLinearColor box_color = Colors::Red;
						FLinearColor snapline_color = Colors::White;

						FName top[] =
						{
							Settings::Bones::Hand_l, // Hand_l 38
							Settings::Bones::head, // head
							Settings::Bones::Hand_r // Hand_r 51
						};

						FName bottom[] =
						{
								Settings::Bones::Foot_l, // foot_l
								Settings::Bones::Foot_r // foot_r
						};

						FVector2D downright;
						FVector2D topleft;

						topleft.X = 9999999;
						topleft.Y = 9999999;
						downright.X = -9999999;
						downright.Y = -9999999;

						CalculateBox(mesh, top, 2, topleft, downright);
						CalculateBox(mesh, bottom, 1, topleft, downright);

						FVector2D PIXEL(downright.X - topleft.X, downright.Y - topleft.Y);
						topleft -= PIXEL * 0.10;
						downright += PIXEL * 0.10;

						auto h = downright.Y - topleft.Y;
						auto w = downright.X - topleft.X;

						auto downleft = FVector2D{ topleft.X, downright.Y };
						auto topright = FVector2D{ downright.X, topleft.Y };

						float centerx = topleft.X + ((topright.X - topleft.X) / 2);
						float y = topright.Y - 18;

						int distance = Engine::get_distance(MyPawn, current_actor);

						if (distance < 50 && current_actor != MyPawn)
						{
							snapline_color = Colors::Red;
							NearActors++;
						}

						if (Settings::Setings.Visuals.Skeleton) {
							FLinearColor SkeletonColor = Colors::Yellow;
							float SkeletonThickness = 1.f;

							FVector2D Neck = Engine::WorldToScreen(Engine::GetSocketLocation(mesh, Settings::Bones::neck));
							FVector2D Upperarm_r = Engine::WorldToScreen(Engine::GetSocketLocation(mesh, Settings::Bones::Upperarm_r));
							FVector2D Lowerarm_r = Engine::WorldToScreen(Engine::GetSocketLocation(mesh, Settings::Bones::Lowerarm_r));
							FVector2D Hand_r = Engine::WorldToScreen(Engine::GetSocketLocation(mesh, Settings::Bones::Hand_r));
							FVector2D Hand_l = Engine::WorldToScreen(Engine::GetSocketLocation(mesh, Settings::Bones::Hand_l));
							FVector2D Lowerarm_l = Engine::WorldToScreen(Engine::GetSocketLocation(mesh, Settings::Bones::Lowerarm_l));
							FVector2D Upperarm_l = Engine::WorldToScreen(Engine::GetSocketLocation(mesh, Settings::Bones::Upperarm_l));
							FVector2D Pelvis = Engine::WorldToScreen(Engine::GetSocketLocation(mesh, Settings::Bones::Pelvis));
							FVector2D Thigh_l = Engine::WorldToScreen(Engine::GetSocketLocation(mesh, Settings::Bones::Thigh_l));
							FVector2D Calf_l = Engine::WorldToScreen(Engine::GetSocketLocation(mesh, Settings::Bones::Calf_l));
							FVector2D Foot_l = Engine::WorldToScreen(Engine::GetSocketLocation(mesh, Settings::Bones::Foot_l));
							FVector2D Foot_r = Engine::WorldToScreen(Engine::GetSocketLocation(mesh, Settings::Bones::Foot_r));
							FVector2D Calf_r = Engine::WorldToScreen(Engine::GetSocketLocation(mesh, Settings::Bones::Calf_r));
							FVector2D Thigh_r = Engine::WorldToScreen(Engine::GetSocketLocation(mesh, Settings::Bones::Thigh_r));
#define SKEL_DRAW(a, b) draw_line(a, b, SkeletonColor, SkeletonThickness);

							SKEL_DRAW(Neck, head);
							SKEL_DRAW(Neck, Upperarm_l);
							SKEL_DRAW(Upperarm_l, Lowerarm_l);
							SKEL_DRAW(Lowerarm_l, Hand_l);
							SKEL_DRAW(Neck, Upperarm_r);
							SKEL_DRAW(Upperarm_r, Lowerarm_r);
							SKEL_DRAW(Lowerarm_r, Hand_r);
							SKEL_DRAW(Neck, Pelvis);
							SKEL_DRAW(Pelvis, Thigh_l);
							SKEL_DRAW(Thigh_l, Calf_l);
							SKEL_DRAW(Calf_l, Foot_l);
							SKEL_DRAW(Pelvis, Thigh_r);
							SKEL_DRAW(Thigh_r, Calf_r);
							SKEL_DRAW(Calf_r, Foot_r);
						}

						if (Settings::Setings.Visuals.headcircle) {
							GUI::DrawCircle({ head.X, head.Y }, 3.f, 60, Colors::White);
						}

						if (Settings::Setings.Visuals.Boxesp)
						{
							float thickness = 2.f;
							float scale__ = 0.3f;

							draw_line(topleft, { topleft.X, topleft.Y + h * scale__ }, box_color, thickness);
							draw_line(topleft, { topleft.X + w * scale__, topleft.Y }, box_color, thickness);

							draw_line(downright, { downright.X, downright.Y - h * scale__ }, box_color, thickness);
							draw_line(downright, { downright.X - w * scale__, downright.Y }, box_color, thickness);

							draw_line(downleft, { downleft.X, downleft.Y - h * scale__ }, box_color, thickness);
							draw_line(downleft, { downleft.X + w * scale__, downright.Y }, box_color, thickness);

							draw_line(topright, { topright.X, topright.Y + h * scale__ }, box_color, thickness);
							draw_line(topright, { topright.X - w * scale__, topright.Y }, box_color, thickness);

						}

						if (Settings::Setings.Visuals.snaplines)
						{
							draw_line(FVector2D(Settings::X / 2, Settings::Y), { centerx, downleft.Y }, snapline_color, 1.0f);
						}

						if (Settings::Setings.Visuals.name)
						{
							std::string name = Engine::GetPlayerName(state).ToString().c_str();
							if (Settings::Setings.Visuals.distance) {
								name += xorstr(" [distance ") + std::to_string(distance) + xorstr("m]");
							}

							draw_text(name.c_str(), { centerx, y }, Colors::White, true);
							y -= 18;
						}

						if (!Settings::Setings.Visuals.name && Settings::Setings.Visuals.distance)
						{
							std::string name = xorstr("[distance ") + std::to_string(distance) + xorstr("m]");
							draw_text(name.c_str(), { centerx, y }, Colors::White, true);
							y -= 18;
						}


						if (Settings::Setings.Visuals.weapon)
						{
							std::string wep = "";
							bool f = false;
							uintptr_t weaponComp = Utils::read<uintptr_t>(current_actor + 0x820);
							if (weaponComp) {
								uintptr_t weaponData = Utils::read<uintptr_t>(weaponComp + 0x390);
								if (weaponData)
								{
									FText DisplayName = Utils::read<FText>(weaponData + 0x80);
									if (DisplayName.Get()) {

										auto str_ = DisplayName.Get();

										wdraw_text(str_, { centerx, y }, Engine::get_weapon_color(weaponData), true);
										y -= 18;

										f = true;
									}
								}
							}
							if (!f) {
								wep += xorstr("no weapon");

								draw_text(wep.c_str(), { centerx, y }, Colors::White, true);
								y -= 18;
							}
						}

						if (aimbotBonew2s.valid()) {

							aimbotBonew2s.X -= Settings::X / 2;
							aimbotBonew2s.Y -= Settings::Y / 2;

							auto dis = crt::sqrtf(aimbotBonew2s.X * aimbotBonew2s.X + aimbotBonew2s.Y * aimbotBonew2s.Y);
							if (dis < closestDistance && dis < Settings::Setings.Aimbot.FieldOV)
							{
								closestDistance = dis;
								closestPawn = current_actor;
							}
						}

					}

					if (Engine::IsA(current_actor, Projectile_class) && closestPawn && MyPawn && Settings::Setings.Exploits.projectiletp)
					{
						uintptr_t MovementComp = Utils::read<uintptr_t>(current_actor + 0x668);
						uintptr_t EnemyMesh = Utils::read<uintptr_t>(closestPawn + 0x390);

						if (MovementComp && EnemyMesh)
						{
							FVector EnemyHead = Engine::GetSocketLocation(EnemyMesh, Settings::Bones::head);
							FRotator EnemyRot = Engine::CalcAngle(Settings::Aimbot::Loc, EnemyHead);
							if (!EnemyHead.valid()) continue;

							Engine::MoveInterpolationTarget(MovementComp, EnemyHead, EnemyRot);
							Engine::StopMovementImmediately(MovementComp);
						}
					}
				}


			}

			if (MyPawn && PC)
			{
				uintptr_t AFortWeapon = Utils::read<uintptr_t>(Settings::Offsets::MP + 0x820);
				if (AFortWeapon && Engine::IsA(AFortWeapon, RangedWep_class))
				{
					if (Settings::Setings.Exploits.nosway)
					{
						*(bool*)(AFortWeapon + 0x971) = true;
					}
					else
					{
						*(bool*)(AFortWeapon + 0x971) = false;
					}

					auto WeaponStats = Engine::GetWeaponStats(AFortWeapon);

					if (WeaponStats) {
						static float Spread = 0.0f;//*(float*)(Row + 0x130);
						static float RecoilV = 0.0f; // *(float*)(Row + 0x1AC);
						static float RecoilH = 0.0f; // *(float*)(Row + 0x1AC);
						static float FireRate = 0.0f;
						static float ReloadTime = 0.0f;
						static const char* wep_name = "";
						static bool fag = false;
						if (fag == false)
						{
							wep_name = Utils::GetObjectNameByIndex(*(int*)(AFortWeapon + 0x18)).c_str();
							fag = true;
						}
						if (Utils::GetObjectNameByIndex(*(int*)(AFortWeapon + 0x18)).c_str() != wep_name) // Weapon change, hardcore 
						{
							fag = false;
							Spread = 0.0f, RecoilH = 0.0f;
							RecoilV = 0.0f;
							ReloadTime = 0.f;
						}

						if (RecoilH == 0.0f)
							RecoilH = *(float*)(WeaponStats + 0x1BC);
						if (ReloadTime == 0.0f)
							ReloadTime = *(float*)(WeaponStats + 0xF0);
						if (RecoilV == 0.0f)
							RecoilV = *(float*)(WeaponStats + 0x1AC);
						if (Spread == 0.0f)
							Spread = *(float*)(WeaponStats + 0x130);

						if (Settings::Setings.Exploits.norecoil)
						{
							*(float*)(WeaponStats + 0x1AC) = 0.0f;
							*(float*)(WeaponStats + 0x1BC) = 0.0f;
						}
						else
						{
							*(float*)(WeaponStats + 0x1AC) = RecoilV;
							*(float*)(WeaponStats + 0x1BC) = RecoilH;
						}

						if (Settings::Setings.Exploits.nospread)
						{
							*(float*)(WeaponStats + 0x130) = 0.0f;
							*(float*)(WeaponStats + 0x134) = 0.0f;
							*(float*)(WeaponStats + 0x138) = 0.0f;
							*(float*)(WeaponStats + 0x13C) = 0.0f;
							*(float*)(WeaponStats + 0x140) = 0.0f;
							*(float*)(WeaponStats + 0x144) = 0.0f;
							*(int32*)(WeaponStats + 0x10C) = 0.0f;
							*(int32*)(WeaponStats + 0x110) = 0.0f;

						}
						else {
							*(float*)(WeaponStats + 0x130) = Spread;
						}

						if (Settings::Setings.Exploits.rapidfire)
							*(float*)(WeaponStats + 0x19C) = Settings::Setings.Exploits.rapid_fire_time;

						if (Settings::Setings.Exploits.instareload)
						{
							*(float*)(WeaponStats + 0xF0) = 0.0f;
							*(float*)(WeaponStats + 0xF4) = 0.0f;
						}
						else
						{
							*(float*)(WeaponStats + 0xF0) = ReloadTime;
						}
					}

				}
			}

			if (NearActors > 0 && Settings::Setings.Exploits.debug) {
				std::string faggot1 = xorstr("Enemies near: ");
				faggot1 += std::to_string(NearActors);
				draw_text(faggot1.c_str(), { (float)Settings::X / 2, Settings::draw_y }, Colors::Red, true);
				Settings::draw_y += 18.f;
			}

			if (GetAsyncKeyState(Settings::Setings.Aimbot.aimkey) && Settings::Setings.Aimbot.Memory && Settings::Setings.Aimbot.Aimbot && closestPawn && Settings::Aimbot::Loc.valid())
			{
				uintptr_t mesh = (*(uintptr_t*)(closestPawn + 0x390));

				FName bone_ = Settings::Bones::head;
				switch (Settings::Setings.Aimbot.aimbone)
				{
				case 0:
					bone_ = Settings::Bones::Calf_l;
				case 2:
					bone_ = Settings::Bones::neck;
				}

				FVector bone = Engine::GetSocketLocation(mesh, bone_);
				if (!bone.valid()) break;

				FVector OriginalLocation = Engine::GetCameraLocation(PCM);

				FRotator Angles = Engine::STATIC_FindLookAtRotation(Settings::Aimbot::Loc, bone);

				Engine::SetControlRotation(Angles);
			}

		}
	} while (0);

	if (Settings::Setings.Aimbot.draw && Settings::X != 0 && Settings::Y != 0)
		GUI::DrawCircle({ (float)Settings::X / 2, (float)Settings::Y / 2 }, Settings::Setings.Aimbot.FieldOV, 64, Colors::White);

	if (Settings::Setings.Exploits.crosshair && Settings::X != 0 && Settings::Y != 0) {
		draw_line({ (float)Settings::X / 2 - 5.f, (float)Settings::Y / 2 }, { (float)Settings::X / 2 + 5.f, (float)Settings::Y / 2 }, Colors::White, 1.0f);
		draw_line({ (float)Settings::X / 2, (float)Settings::Y / 2 - 5.f }, { (float)Settings::X / 2, (float)Settings::Y / 2 + 5.f }, Colors::White, 1.0f);

	}

	Tick();

	Postrender(Viewport, Canvas);
}

void DllStart() {
    if (!Settings::Module || !Settings::ImageSize) {
        Settings::Module = *(PVOID*)(__readgsqword(0x60) + 0x10);

        GObjects = decltype(GObjects)((uintptr_t)Settings::Module + 0x05B918B8); // GObjects

        if (!Settings::Offsets::UWorld) {
            Settings::Offsets::UWorld = (std::uintptr_t)(Settings::Module) +0x5C80950; // GWorld
        }

		Settings::Offsets::KismetMathLibrary = GObjects->FindObject("Class Engine.KismetMathLibrary");
		kill();
        uintptr_t GWorld = Utils::read<uintptr_t>(Settings::Offsets::UWorld);
        uintptr_t Gameinstance = Utils::read<uintptr_t>(GWorld + 0x160);
        uintptr_t LocalPlayer = Utils::read<uintptr_t>(Utils::read<uintptr_t>(Gameinstance + 0x38));
        uintptr_t ViewportClient = Utils::read<uintptr_t>(LocalPlayer + 0x70);

        Utils::dbg_print("ViewportClient: %x\n", ViewportClient);

        uintptr_t gEngine = Utils::read<uintptr_t>(Gameinstance + 0x20); // OuterClass
        if (gEngine) {
            Settings::Offsets::Font = *(PVOID*)(gEngine + 0x70);
            if (Settings::Offsets::Font) {
                Utils::dbg_print(xorstr("Font: %s\n"), Utils::GetObjectName((uintptr_t)Settings::Offsets::Font).c_str());
            }

        }

		Settings::Bones::head = Engine::ConvStringToName(xorstr(L"head"));
		Settings::Bones::root = Engine::ConvStringToName(xorstr(L"Root"));
		Settings::Bones::neck = Engine::ConvStringToName(xorstr(L"neck_01"));
		Settings::Bones::Pelvis = Engine::ConvStringToName(xorstr(L"pelvis"));
		Settings::Bones::Upperarm_l = Engine::ConvStringToName(xorstr(L"upperarm_l"));
		Settings::Bones::Upperarm_r = Engine::ConvStringToName(xorstr(L"upperarm_r"));
		Settings::Bones::Lowerarm_l = Engine::ConvStringToName(xorstr(L"lowerarm_l"));
		Settings::Bones::Lowerarm_r = Engine::ConvStringToName(xorstr(L"lowerarm_r"));
		Settings::Bones::Hand_l = Engine::ConvStringToName(xorstr(L"Hand_l"));
		Settings::Bones::Hand_r = Engine::ConvStringToName(xorstr(L"hand_r"));
		Settings::Bones::Thigh_l = Engine::ConvStringToName(xorstr(L"thigh_l"));
		Settings::Bones::Thigh_r = Engine::ConvStringToName(xorstr(L"thigh_r"));
		Settings::Bones::Calf_l = Engine::ConvStringToName(xorstr(L"calf_l"));
		Settings::Bones::Calf_r = Engine::ConvStringToName(xorstr(L"calf_r"));
		Settings::Bones::Foot_l = Engine::ConvStringToName(xorstr(L"foot_l"));
		Settings::Bones::Foot_r = Engine::ConvStringToName(xorstr(L"foot_r"));
		Settings::Bones::Toe_l = Engine::ConvStringToName(xorstr(L"ball_l"));
		Settings::Bones::Toe_r = Engine::ConvStringToName(xorstr(L"ball_r"));

        Utils::InstallVtableHook((void*)ViewportClient, hkPostRender, 92, (void**)&Postrender);

    }

} // 4444 was here

BOOL WINAPI DllMain(HMODULE hModule, DWORD dwReason, LPVOID lpReserved) {
    if (dwReason == 1) {
		AllocConsole();
		FILE* fileptr;
		freopen_s(&fileptr, "CONOUT$", "w", stdout);
		freopen_s(&fileptr, "CONOUT$", "w", stderr);
		freopen_s(&fileptr, "CONIN$", "r", stdin);
        DllStart();
    }
    return true;
}
