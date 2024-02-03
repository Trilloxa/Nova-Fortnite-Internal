#pragma once
#include "../stdafx.h"

#define resolve_rva(addr, size) ((std::uintptr_t)(addr + *(DWORD*)(addr + ((size) - 4)) + size))

namespace Engine {

	void ProcessEvent(void* object, void* UFunction, void* Params)
	{
		auto vtable = *reinterpret_cast<void***>(object);
		auto fn = reinterpret_cast<void(*)(void*, void*, void*)>(vtable[0x41]);
		spoof(fn, object, UFunction, Params);
	}

	namespace Aimbot
	{
		std::vector<std::pair<std::string, int>> g = {
			{xorstr("Legs"), 48}, // 83
			{xorstr("Head"), 4}, // 8
		//	{xorstr("Gloves"), 41},
			{xorstr("Torso"), 15}
		};

		int GetBone()
		{
			return g[Settings::Setings.Aimbot.aimbone].second;
		}

	}

	void Getscreensize(int* x, int* y)
	{
		if (!Settings::Aimbot::PC) return;

		static uintptr_t addr = 0;
		if (!addr)
			addr = (Utils::FindPattern(Settings::Module, xorstr("48 89 5C 24 10 48 89 74 24 18 57 48 83 EC 20 33 C0 49 8B F8"), '?'));

		auto fn = (void(__fastcall*)(uintptr_t, int*, int*))addr;

		//fn(Settings::Aimbot::PC, x, y);
		return spoof(fn, Settings::Aimbot::PC, x, y);
	}

	void GetCursor(LPPOINT cur)
	{
		if (!Settings::Aimbot::PC) return;

		static uintptr_t addr = 0;
		if (!addr)
			addr = (Utils::FindPattern(Settings::Module, xorstr("48 89 5C 24 10 48 89 74 24 18 57 48 83 EC 20 48 8B 99 B0"), '?'));

		auto fn = (char(__fastcall*)(uintptr_t, float*, float*))addr;
		float x, y;


		spoof(fn, Settings::Aimbot::PC, &x, &y);

		cur->x = x;
		cur->y = y;
	}

	int GetNumMaterials(uintptr_t mesh)
	{
		static PVOID object = 0;
		if (!object) object = GObjects->FindObject(xorstr("Function Engine.PrimitiveComponent.GetNumMaterials"));

		struct {
			int return_value;
		} params;

		ProcessEvent((void*)mesh, object, &params);

		return params.return_value;
	}

	void CreativeStartFlyUp(uintptr_t PC)
	{
		static void* addr = 0;
		if (!addr) addr = GObjects->FindObject(xorstr("Function FortniteGame.FortPlayerControllerGameplay.ServerCreativeStartFlyUp"));

		struct {

		}Params;

		Engine::ProcessEvent((void*)PC, addr, &Params);
	}


	uintptr_t GetBaseMaterial(uintptr_t mesh)
	{
		static PVOID object = 0;
		if (!object) object = GObjects->FindObject(xorstr("Function Engine.MaterialInterface.GetBaseMaterial"));

		struct {
			uintptr_t return_value;
		} params;

		ProcessEvent((void*)mesh, object, &params);

		return params.return_value;
	}

	uintptr_t GetMaterial(uintptr_t mesh, int index)
	{
		static PVOID object = 0;
		if (!object) object = GObjects->FindObject(xorstr("Function Engine.PrimitiveComponent.GetMaterial"));

		struct {
			int index;
			uintptr_t return_value;
		} params;

		params.index = index;

		ProcessEvent((void*)mesh, object, &params);

		return params.return_value;
	}

	void CopyMaterialInstanceParameters(uintptr_t Material, uintptr_t SourceMaterial)
	{
		static PVOID object = 0;
		if (!object) object = GObjects->FindObject(xorstr("Function Engine.MaterialInstanceDynamic.K2_CopyMaterialInstanceParameters"));

		struct {
			uintptr_t SourceMaterial;
		} params;

		params.SourceMaterial = SourceMaterial;

		ProcessEvent((void*)Material, object, &params);
	}

	void SetVectorParameterValue(uintptr_t Material, FName ParameterName, FLinearColor Value)
	{
		static PVOID object = 0;
		if (!object) object = GObjects->FindObject(xorstr("Function Engine.MaterialInstanceDynamic.SetVectorParameterValue"));

		struct {
			FName ParameterName;
			FLinearColor Value;
		} params;

		params.ParameterName = ParameterName;
		params.Value = Value;

		ProcessEvent((void*)Material, object, &params);
	}

	FVector2D WorldToScreen(FVector world_location)
	{
		if (!Settings::Aimbot::PC) return { 0.f, 0.f };

		static void* GameplayStats = 0;
		if (!GameplayStats) GameplayStats = GObjects->FindObject(xorstr("Class Engine.GameplayStatics"));

		static void* Func_ = 0;
		if (!Func_) Func_ = GObjects->FindObject(xorstr("Function Engine.GameplayStatics.ProjectWorldToScreen"));

		struct {
			uintptr_t PC;
			FVector World;
			FVector2D Screen;
			bool bPlayer;
			bool return_value;
		} Params;

		Params.PC = Settings::Aimbot::PC;
		Params.World = world_location;
		Params.bPlayer = false;

		Engine::ProcessEvent(GameplayStats, Func_, &Params);

		if (Params.return_value)
			return Params.Screen;
		else
			return { 0.f, 0.f };
	}

	void SetControlRotation(FRotator NewRot)
	{
		if (!Settings::Aimbot::PC) return;

		static void* addr = 0;
		if (!addr) addr = GObjects->FindObject(xorstr("Function Engine.Controller.SetControlRotation"));

		struct {
			FRotator return_value;
		} Params;

		Params.return_value = NewRot;

		Engine::ProcessEvent((void*)Settings::Aimbot::PC, addr, &Params);
	}

	void SetFirstPersonCamera(uint64_t actor, bool bNewUseFirstPersonCamera)
	{
		static void* addr = 0;
		if (!addr) addr = GObjects->FindObject(xorstr("Function FortniteGame.FortPlayerPawn.SetFirstPersonCamera"));

		struct {
		} Params;

		Engine::ProcessEvent((void*)actor, addr, &Params);
	}

	FDataTableRowHandle GetWeaponStatHandle(uintptr_t WeaponData)
	{
		static void* addr = 0;
		if (!addr) addr = GObjects->FindObject(xorstr("Function FortniteGame.FortWeaponItemDefinition.GetWeaponStatHandle"));

		struct {
			FDataTableRowHandle row;
		} Params;

		Engine::ProcessEvent((void*)WeaponData, addr, &Params);

		return Params.row;
	}

	uintptr_t GetWeaponStats(uintptr_t Weapon)
	{
		uintptr_t WeaponData = Utils::read<uintptr_t>(Weapon + 0x390);
		if (WeaponData) {
			FDataTableRowHandle WeaponStats = Engine::GetWeaponStatHandle(WeaponData);
			if (WeaponStats.DataTable != nullptr) {
				auto row = WeaponStats.DataTable->FindRow(WeaponStats.RowName);
				return row;
			}
		}
		return 0;
	}

	void MoveInterpolationTarget(uintptr_t Movement, FVector Loc, FRotator Rot)
	{
		if (!Movement) return;

		static void* addr = 0;
		if (!addr) addr = (void*)GObjects->FindObject(xorstr("Function Engine.ProjectileMovementComponent.MoveInterpolationTarget"));

		struct
		{
			FVector Loc;
			FRotator Rot;
		} params;

		params.Loc = Loc;
		params.Rot = Rot;

		Engine::ProcessEvent((void*)Movement, addr, &params);
	}

	void StopMovementImmediately(uintptr_t Movement)
	{
		if (!Movement) return;

		static void* addr = 0;
		if (!addr) addr = (void*)GObjects->FindObject(xorstr("Function Engine.MovementComponent.StopMovementImmediately"));

		struct
		{
		} params;

		Engine::ProcessEvent((void*)Movement, addr, &params);
	}

	enum class EFortQuality : uint8
	{
		Common = 0,
		Uncommon = 1,
		Rare = 2,
		NumQualityValues = 3,
		EFortQuality_MAX = 4,
	};
	enum class EFortRarity : uint8
	{
		Handmade = 0,
		Ordinary = 1,
		Sturdy = 2,
		Quality = 3,
		Fine = 4,
		Elegant = 5,
		Masterwork = 6,
		Epic = 7,
		Badass = 8,
		Legendary = 9,
		NumRarityValues = 10,
		EFortRarity_MAX = 11,
	};

	FLinearColor get_weapon_color(uintptr_t weaponData)
	{
		EFortRarity rarity = Utils::read<EFortRarity>(weaponData + 0x50);
		EFortQuality quality = Utils::read<EFortQuality>(weaponData + 0x54);

		switch (quality)
		{
		case EFortQuality::Common:
			return { 211,211,211, 1 };
			break;
		case EFortQuality::Uncommon:
			return { 50,205,50 , 1 };
			break;
		case EFortQuality::Rare:
			return Colors::Blue;
			break;
		}

		switch (rarity)
		{
		case EFortRarity::Epic:
			return { 255,0,255, 1 };
			break;
		case EFortRarity::Legendary:
			return { 255,165,0 , 1 };
			break;
		default:
			return Colors::White;
		}

		return Colors::White;
	}

	int GetMagAmmoCount(uintptr_t weapon)
	{
		static void* addr = 0;
		if (!addr) addr = GObjects->FindObject(xorstr("Function FortniteGame.FortWeapon.GetMagazineAmmoCount"));

		struct {
			int return_value;
		} Params;

		Engine::ProcessEvent((void*)weapon, addr, &Params);

		return Params.return_value;
	}

	D3DMATRIX _inline MatrixMultiplication(D3DMATRIX pM1, D3DMATRIX pM2)
	{
		D3DMATRIX pOut;
		pOut._11 = pM1._11 * pM2._11 + pM1._12 * pM2._21 + pM1._13 * pM2._31 + pM1._14 * pM2._41;
		pOut._12 = pM1._11 * pM2._12 + pM1._12 * pM2._22 + pM1._13 * pM2._32 + pM1._14 * pM2._42;
		pOut._13 = pM1._11 * pM2._13 + pM1._12 * pM2._23 + pM1._13 * pM2._33 + pM1._14 * pM2._43;
		pOut._14 = pM1._11 * pM2._14 + pM1._12 * pM2._24 + pM1._13 * pM2._34 + pM1._14 * pM2._44;
		pOut._21 = pM1._21 * pM2._11 + pM1._22 * pM2._21 + pM1._23 * pM2._31 + pM1._24 * pM2._41;
		pOut._22 = pM1._21 * pM2._12 + pM1._22 * pM2._22 + pM1._23 * pM2._32 + pM1._24 * pM2._42;
		pOut._23 = pM1._21 * pM2._13 + pM1._22 * pM2._23 + pM1._23 * pM2._33 + pM1._24 * pM2._43;
		pOut._24 = pM1._21 * pM2._14 + pM1._22 * pM2._24 + pM1._23 * pM2._34 + pM1._24 * pM2._44;
		pOut._31 = pM1._31 * pM2._11 + pM1._32 * pM2._21 + pM1._33 * pM2._31 + pM1._34 * pM2._41;
		pOut._32 = pM1._31 * pM2._12 + pM1._32 * pM2._22 + pM1._33 * pM2._32 + pM1._34 * pM2._42;
		pOut._33 = pM1._31 * pM2._13 + pM1._32 * pM2._23 + pM1._33 * pM2._33 + pM1._34 * pM2._43;
		pOut._34 = pM1._31 * pM2._14 + pM1._32 * pM2._24 + pM1._33 * pM2._34 + pM1._34 * pM2._44;
		pOut._41 = pM1._41 * pM2._11 + pM1._42 * pM2._21 + pM1._43 * pM2._31 + pM1._44 * pM2._41;
		pOut._42 = pM1._41 * pM2._12 + pM1._42 * pM2._22 + pM1._43 * pM2._32 + pM1._44 * pM2._42;
		pOut._43 = pM1._41 * pM2._13 + pM1._42 * pM2._23 + pM1._43 * pM2._33 + pM1._44 * pM2._43;
		pOut._44 = pM1._41 * pM2._14 + pM1._42 * pM2._24 + pM1._43 * pM2._34 + pM1._44 * pM2._44;

		return pOut;
	}

#define x X
#define y Y
#define z Z
#define w W

	D3DMATRIX ToMatrixWithScale(const FVector& translation, const FVector& scale, const FPlane& rot)
	{
		D3DMATRIX m;
		m._41 = translation.x;
		m._42 = translation.y;
		m._43 = translation.z;

		float x2 = rot.x + rot.x;
		float y2 = rot.y + rot.y;
		float z2 = rot.z + rot.z;

		float xx2 = rot.x * x2;
		float yy2 = rot.y * y2;
		float zz2 = rot.z * z2;
		m._11 = (1.0f - (yy2 + zz2)) * scale.x;
		m._22 = (1.0f - (xx2 + zz2)) * scale.y;
		m._33 = (1.0f - (xx2 + yy2)) * scale.z;

		float yz2 = rot.y * z2;
		float wx2 = rot.w * x2;
		m._32 = (yz2 - wx2) * scale.z;
		m._23 = (yz2 + wx2) * scale.y;

		float xy2 = rot.x * y2;
		float wz2 = rot.w * z2;
		m._21 = (xy2 - wz2) * scale.y;
		m._12 = (xy2 + wz2) * scale.x;

		float xz2 = rot.x * z2;
		float wy2 = rot.w * y2;
		m._31 = (xz2 + wy2) * scale.z;
		m._13 = (xz2 - wy2) * scale.x;

		m._14 = 0.0f;
		m._24 = 0.0f;
		m._34 = 0.0f;
		m._44 = 1.0f;

		return m;
	}
#undef x
#undef y
#undef z
#undef w



#define BONE_ARRAY 0x798       //0x5CC

	FName GetBoneName(uintptr_t mesh, int bone)
	{
		static uintptr_t addr = 0;
		if (!addr)
			addr = (uintptr_t)GObjects->FindObject(xorstr("Function Engine.SkinnedMeshComponent.GetBoneName"));

		struct {
			int bone_id;
			FName return_value;
		} Parameters;

		Parameters.bone_id = bone;

		Engine::ProcessEvent((void*)mesh, (void*)addr, &Parameters);

		return Parameters.return_value;
	}

	int NumSockets(uintptr_t skeletal)
	{
		static void* addr = 0;
		if (!addr) addr = GObjects->FindObject(xorstr("Function Engine.SkeletalMesh.NumSockets"));

		struct {
			int return_value;
		} Params;

		Engine::ProcessEvent((void*)skeletal, addr, &Params);

		return Params.return_value;
	}

	uintptr_t GetSocketByIndex(uintptr_t skeletal, int idx)
	{
		static void* addr = 0;
		if (!addr) addr = GObjects->FindObject(xorstr("Function Engine.SkeletalMesh.GetSocketByIndex"));

		struct {
			int idx;
			uintptr_t return_value;
		} Params;

		Params.idx = idx;

		Engine::ProcessEvent((void*)skeletal, addr, &Params);

		return Params.return_value;
	}

	uintptr_t FindSocket(uintptr_t skeletal, FName idx)
	{
		static void* addr = 0;
		if (!addr) addr = GObjects->FindObject(xorstr("Function Engine.SkeletalMesh.FindSocket"));

		struct {
			FName idx;
			uintptr_t return_value;
		} Params;

		Params.idx = idx;

		Engine::ProcessEvent((void*)skeletal, addr, &Params);

		return Params.return_value;
	}

	FVector GetSocketLoc(uintptr_t socket, uintptr_t skel)
	{
		static void* addr = 0;
		if (!addr) addr = GObjects->FindObject(xorstr("Function Engine.SkeletalMeshSocket.GetSocketLocation"));

		struct {
			uintptr_t idx;
			FVector return_value;
		} Params;

		Params.idx = skel;

		Engine::ProcessEvent((void*)socket, addr, &Params);

		return Params.return_value;
	}

	FVector GetAimboneBone(uintptr_t mesh, int bone)
	{
		FVector loc = FVector();
		if (!mesh)  return loc;

		uintptr_t skeletal_mesh = Utils::read<uintptr_t>(mesh + 0x5A0);
		if (!skeletal_mesh) return loc;

		auto socket = GetSocketByIndex(skeletal_mesh, bone);
		if (!socket) return loc;

		loc = GetSocketLoc(socket, skeletal_mesh);

		return loc;
	}

	FVector GetSocketLocation(uintptr_t mesh, FName socket)
	{
		static void* addr = 0;
		if (!addr) {
			addr = GObjects->FindObject(xorstr("Function Engine.SceneComponent.GetSocketLocation"));
		}

		struct {
			FName Socket;
			FVector return_value;
		}Params;

		Params.Socket = socket;

		Engine::ProcessEvent((void*)mesh, addr, &Params);

		return Params.return_value;
	}

	FVector GetCameraLocation(uintptr_t PCM)
	{
		static void* addr = 0;
		if (!addr) addr = GObjects->FindObject(xorstr("Function Engine.PlayerCameraManager.GetCameraLocation"));

		struct {
			FVector return_value;
		} Params;

		Engine::ProcessEvent((void*)PCM, addr, &Params);

		return Params.return_value;
	}

	FRotator GetCameraRotation(uintptr_t PCM)
	{
		static void* addr = 0;
		if (!addr) addr = GObjects->FindObject(xorstr("Function Engine.PlayerCameraManager.GetCameraRotation"));

		struct {
			FRotator return_value;
		} Params;

		Engine::ProcessEvent((void*)PCM, addr, &Params);

		return Params.return_value;
	}

	FName ConvStringToName(FString str)
	{
		static void* KismetLib = 0;
		if (!KismetLib) KismetLib = GObjects->FindObject(xorstr("Class Engine.KismetStringLibrary"));

		static void* ConvStrToName = 0;
		if (!ConvStrToName) ConvStrToName = GObjects->FindObject(xorstr("Function Engine.KismetStringLibrary.Conv_StringToName"));

		struct {
			FString InString;
			FName return_value;
		} Params;

		Params.InString = str;

		Engine::ProcessEvent(KismetLib, ConvStrToName, &Params);

		return Params.return_value;
	}

	bool IsA(uintptr_t actor, uintptr_t other)
	{
		for (uintptr_t super = Utils::read<uintptr_t>(actor + 0x10); super; super = Utils::read<uintptr_t>(super + 0x40))
		{
			if (super == other)
			{
				return true;
			}
		}

		return false;
	}

	void K2_DrawText(void* canvas, FString RenderText, FVector2D ScreenPosition, FVector2D Scale, FLinearColor RenderColor, float Kerning, FLinearColor ShadowColor, FVector2D ShadowOffset, bool bCentreX, bool bCentreY, bool bOutlined, FLinearColor OutlineColor)
	{
		static uintptr_t addr = 0;
		if (!addr) addr = (uintptr_t)GObjects->FindObject("Function Engine.Canvas.K2_DrawText");

		struct {
			PVOID RenderFont; //UFont* 
			FString RenderText;
			FVector2D ScreenPosition;
			FVector2D Scale;
			FLinearColor RenderColor;
			float Kerning;
			FLinearColor ShadowColor;
			FVector2D ShadowOffset;
			bool bCentreX;
			bool bCentreY;
			bool bOutlined;
			FLinearColor OutlineColor;
		} Parameters;

		Parameters.RenderFont = Settings::Offsets::Font;
		Parameters.RenderText = RenderText;
		Parameters.ScreenPosition = ScreenPosition;
		Parameters.Scale = Scale;
		Parameters.RenderColor = RenderColor;
		Parameters.Kerning = Kerning;
		Parameters.ShadowColor = ShadowColor;
		Parameters.ShadowOffset = ShadowOffset;
		Parameters.bCentreX = bCentreX;
		Parameters.bCentreY = bCentreY;
		Parameters.bOutlined = bOutlined;

		Engine::ProcessEvent(canvas, (void*)addr, &Parameters);


	}

	void K2_DrawBox(void* canvas, const struct FVector2D& ScreenPosition, const struct FVector2D& ScreenSize, float Thickness, const struct FLinearColor& RenderColor)
	{
		static uintptr_t addr = 0;
		if (!addr)
			addr = (uintptr_t)GObjects->FindObject("Function Engine.Canvas.K2_DrawBox");

		//return reinterpret_cast<void(__fastcall*)(void* _this, FVector2D ScreenPosition, FVector2D ScreenSize, float thick, FLinearColor LineColor)>(addr)(canvas, ScreenPosition, ScreenSize, Thickness, RenderColor);

		struct {
			FVector2D                                   ScreenPosition;                                           // (Parm, ZeroConstructor, IsPlainOldData)
			FVector2D                                   ScreenSize;                                               // (Parm, ZeroConstructor, IsPlainOldData)
			float                                              Thickness;                                                // (Parm, ZeroConstructor, IsPlainOldData)
			FLinearColor                                RenderColor;
		} Parameters;

		Parameters.ScreenPosition = ScreenPosition;
		Parameters.ScreenSize = ScreenSize;
		Parameters.Thickness = Thickness;
		Parameters.RenderColor = RenderColor;

		Engine::ProcessEvent(canvas, (void*)addr, &Parameters);
	}

	void K2_DrawLine(void* canvas, FVector2D a, FVector2D b, float thicc, FLinearColor clr)
	{
		static uintptr_t addr = 0;
		if (!addr)
			addr = (uintptr_t)GObjects->FindObject("Function Engine.Canvas.K2_DrawLine");

		//return reinterpret_cast<void(__fastcall*)(void* _this, FVector2D ScreenPositionA, FVector2D ScreenPositionB, float thick, FLinearColor LineColor)>(addr)(canvas, a, b, thicc, clr);

		struct {
			FVector2D                                   ScreenPositionA;                                          // (Parm, ZeroConstructor, IsPlainOldData)
			FVector2D                                   ScreenPositionB;                                          // (Parm, ZeroConstructor, IsPlainOldData)
			float                                              Thickness;                                                // (Parm, ZeroConstructor, IsPlainOldData)
			FLinearColor                                RenderColor;
		} Parameters;

		Parameters.ScreenPositionA = a;
		Parameters.ScreenPositionB = b;
		Parameters.Thickness = thicc;
		Parameters.RenderColor = clr;

		Engine::ProcessEvent(canvas, (void*)addr, &Parameters);
	}

	FRotator CalcAngle(FVector cameraLocation, FVector boneLocation) {

		FRotator result;

		auto delta = boneLocation - cameraLocation;
		auto distance = delta.Size();

		result.Yaw = atan2f(delta.Y, delta.X) * (180.0f / 3.14159265358979323846264338327950288419716939937510); // MPI
		result.Pitch = (-((acosf((delta.Z / distance)) * 180.0f / 3.14159265358979323846264338327950288419716939937510) - 90.0f));
		result.Roll = NULL;

		return result;
	}

	struct FRotator STATIC_FindLookAtRotation(const struct FVector& Start, const struct FVector& Target)
	{
		static uintptr_t addr = 0;
		if (!addr)
			addr = (uintptr_t)GObjects->FindObject("Function Engine.KismetMathLibrary.FindLookAtRotation");

		struct {
			struct FVector                                     Start;                                                    // (ConstParm, Parm, OutParm, ZeroConstructor, ReferenceParm, IsPlainOldData)
			struct FVector                                     Target;                                                   // (ConstParm, Parm, OutParm, ZeroConstructor, ReferenceParm, IsPlainOldData)
			struct FRotator                                    ReturnValue;
		} Parameters;
		Parameters.Start = Start;
		Parameters.Target = Target;

		ProcessEvent(Settings::Offsets::KismetMathLibrary, (void*)addr, &Parameters);

		return Parameters.ReturnValue;
	}

	FRotator SmoothMe(FRotator startRotation, FRotator endRotation, float SmoothScale)
	{
		FRotator ret = endRotation;

		ret.Pitch -= startRotation.Pitch;
		ret.Yaw -= startRotation.Yaw;

		//clamp
		if (ret.Yaw < -180.0f)
			ret.Yaw += 360.0f;

		if (ret.Yaw > 180.0f)
			ret.Yaw -= 360.0f;

		if (ret.Pitch < -74.0f)
			ret.Pitch = -74.0f;

		if (ret.Pitch > 74.0f)
			ret.Pitch = 74.0f;

		ret.Pitch = (ret.Pitch / SmoothScale) + startRotation.Pitch;
		ret.Yaw = (ret.Yaw / SmoothScale) + startRotation.Yaw;

		//clamp
		if (ret.Yaw < -180.0f)
			ret.Yaw += 360.0f;

		if (ret.Yaw > 180.0f)
			ret.Yaw -= 360.0f;

		if (ret.Pitch < -74.0f)
			ret.Pitch = -74.0f;

		if (ret.Pitch > 74.0f)
			ret.Pitch = 74.0f;

		ret.Roll = 0;

		return ret;
	}

	template<typename Fn>
	inline Fn GetVFunction(const void* instance, std::size_t index)
	{
		auto vtable = *reinterpret_cast<const void***>(const_cast<void*>(instance));
		return reinterpret_cast<Fn>(vtable[index]);
	}

	int get_distance(uintptr_t mypawn, uintptr_t actor)
	{
		if (!mypawn || !actor) return 0;

		uintptr_t myroot = Utils::read<uintptr_t>(mypawn + 0x158);
		if (!myroot) return 0;
		FVector mylocation = Utils::read<FVector>(myroot + 0x164);
		if (!mylocation.valid()) return 0;

		uintptr_t root = Utils::read<uintptr_t>(actor + 0x158);
		if (!root) return 0;
		FVector location = Utils::read<FVector>(root + 0x164);
		if (!location.valid()) return 0;

		return (int)(mylocation.DistanceFrom(location) / 100.f);
	}

	FVector GetLocation(uintptr_t actor)
	{
		FVector loc = FVector();

		uintptr_t myroot = Utils::read<uintptr_t>(actor + 0x158);
		if (!myroot) return loc;

		loc = Utils::read<FVector>(myroot + 0x164);

		return loc;
	}

	struct FColor
	{
		unsigned char B;
		unsigned char G;
		unsigned char R;
		unsigned char A;

		FLinearColor GetColor() {
			return { R / 255.f, G / 255.f, B / 255.f, A / 255.f };
		}
	};

	FLinearColor GetItemRarity(uintptr_t actor)
	{
		if (actor)
		{
			FColor clr = *(FColor*)(actor + 0x3d0);
			return clr.GetColor();
		}

		return Colors::White;
	}

#define ws(x) Engine::WorldToScreen(x)
#define Line(a, b, c, d) K2_DrawLine(d, a, b, 1.0f, c)

	void DBox(void* canvas, FVector origin, FVector extends, FLinearColor clr, float* y)
	{
		extends.X *= 2.f;
		extends.Y *= 2.f;
		extends.Z *= 2.f;

		origin.X -= extends.X / 2.f;
		origin.Y -= extends.Y / 2.f;
		origin.Z -= extends.Z / 2.f;

		FVector one = origin;
		FVector two = origin;
		two.X += extends.X;
		FVector three = origin;
		three.X += extends.X;
		three.Y += extends.Y;
		FVector four = origin;
		four.Y += extends.Y;

		FVector five = one;
		five.Z += extends.Z;
		FVector six = two;
		six.Z += extends.Z;
		FVector seven = three;
		seven.Z += extends.Z;
		FVector eight = four;
		eight.Z += extends.Z;

		FVector2D s1, s2, s3, s4, s5, s6, s7, s8;

		s1 = ws(one);

		s2 = ws(two);
		s3 = ws(three);
		s4 = ws(four);
		s5 = ws(five);
		s6 = ws(six);
		s7 = ws(seven);
		s8 = ws(eight);

		Line(s1, s2, clr, canvas);
		Line(s2, s3, clr, canvas);
		Line(s3, s4, clr, canvas);
		Line(s4, s1, clr, canvas);

		Line(s5, s6, clr, canvas);
		Line(s6, s7, clr, canvas);
		*y = s7.Y - 15;
		Line(s7, s8, clr, canvas);
		Line(s8, s5, clr, canvas);

		Line(s1, s5, clr, canvas);
		//*y = s1.y - s5.y;
		Line(s2, s6, clr, canvas);
		Line(s3, s7, clr, canvas);
		Line(s4, s8, clr, canvas);

	}

	FString GetPlayerName(uintptr_t state)
	{
		static uintptr_t addr = 0;
		if (!addr)
			addr = (uintptr_t)GObjects->FindObject(xorstr("Function Engine.PlayerState.GetPlayerName"));

		struct {
			FString return_value;
		} Parameters;

		Engine::ProcessEvent((void*)state, (void*)addr, &Parameters);

		return Parameters.return_value;
	}

	uintptr_t GetFont() {
		static uintptr_t addr = 0;
		if (!addr) {
			addr = Utils::FindPattern(Settings::Module, xorstr("48 8B 05 ? ? ? ? 48 8B 40 78"), '?');
		}
		auto fn = (uintptr_t(__fastcall*)())addr;
		return spoof(fn);
	}
}