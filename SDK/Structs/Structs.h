#pragma once
#include <array>
#include <string>
#include <iostream>
#include <Windows.h>

typedef __int8 int8;
typedef __int16 int16;
typedef __int32 int32;
typedef __int64 int64;

typedef unsigned __int8 uint8;
typedef unsigned __int16 uint16;
typedef unsigned __int32 uint32;
typedef unsigned __int64 uint64;

template<class T>
class TArray
{
	friend class FString;

protected:
	T* Data;
	int32 NumElements;
	int32 MaxElements;

public:

	TArray() = default;

	inline TArray(int32 Num)
		:NumElements(0), MaxElements(Num), Data((T*)malloc(sizeof(T)* Num))
	{
	}

	inline T& operator[](uint32 Index)
	{
		return Data[Index];
	}
	inline const T& operator[](uint32 Index) const
	{
		return Data[Index];
	}

	int32 Num() const
	{
		return this->NumElements;
	}

	inline int32 Max()
	{
		return this->MaxElements;
	}

	inline int32 GetSlack()
	{
		return MaxElements - NumElements;
	}

	inline bool IsValid()
	{
		return this->Data != nullptr;
	}

	inline bool IsValidIndex(int32 Index)
	{
		return Index >= 0 && Index < this->NumElements;
	}

	inline bool IsValidIndex(uint32 Index)
	{
		return Index < this->NumElements;
	}

	inline void ResetNum()
	{
		NumElements = 0;
	}

protected:
	inline void FreeArray()
	{
		NumElements = 0;
		MaxElements = 0;
		if (Data) free(Data);
		Data = nullptr;
	}
};

class FString : public TArray<wchar_t>
{
public:
	using TArray::TArray;

	inline FString(const wchar_t* WChar)
	{
		MaxElements = NumElements = *WChar ? std::wcslen(WChar) + 1 : 0;

		if (NumElements)
		{
			Data = const_cast<wchar_t*>(WChar);
		}
	}

	inline FString operator=(const wchar_t*&& Other)
	{
		return FString(Other);
	}

	inline const wchar_t* c_str() const
	{
		return Data;
	}

	inline std::wstring ToWString()
	{
		if (IsValid())
		{
			return Data;
		}
	}

	inline std::string ToString()
	{
		if (IsValid())
		{
			std::wstring WData(Data);
			return std::string(WData.begin(), WData.end());
		}
		return "";
	}
};

class FFreableString : public FString
{
public:
	using FString::FString;

	~FFreableString()
	{
		FreeArray();
	}
};

struct FVector
{
	float                                              X;                                                        // 0x0000(0x0004) (Edit, BlueprintVisible, ZeroConstructor, SaveGame, IsPlainOldData)
	float                                              Y;                                                        // 0x0004(0x0004) (Edit, BlueprintVisible, ZeroConstructor, SaveGame, IsPlainOldData)
	float                                              Z;                                                        // 0x0008(0x0004) (Edit, BlueprintVisible, ZeroConstructor, SaveGame, IsPlainOldData)

	inline FVector()
		: X(0), Y(0), Z(0)
	{ }

	inline FVector(float x, float y, float z)
		: X(x),
		Y(y),
		Z(z)
	{ }

	__forceinline FVector operator-(const FVector& V) {
		return FVector(X - V.X, Y - V.Y, Z - V.Z);
	}

	__forceinline FVector operator+(const FVector& V) {
		return FVector(X + V.X, Y + V.Y, Z + V.Z);
	}

	__forceinline FVector operator*(float Scale) const {
		return FVector(X * Scale, Y * Scale, Z * Scale);
	}

	__forceinline FVector operator/(float Scale) const {
		const float RScale = 1.f / Scale;
		return FVector(X * RScale, Y * RScale, Z * RScale);
	}

	__forceinline FVector operator+(float A) const {
		return FVector(X + A, Y + A, Z + A);
	}

	__forceinline FVector operator-(float A) const {
		return FVector(X - A, Y - A, Z - A);
	}

	__forceinline FVector operator*(const FVector& V) const {
		return FVector(X * V.X, Y * V.Y, Z * V.Z);
	}

	__forceinline FVector operator/(const FVector& V) const {
		return FVector(X / V.X, Y / V.Y, Z / V.Z);
	}

	__forceinline float operator|(const FVector& V) const {
		return X * V.X + Y * V.Y + Z * V.Z;
	}

	__forceinline float operator^(const FVector& V) const {
		return X * V.Y - Y * V.X - Z * V.Z;
	}

	__forceinline FVector& operator+=(const FVector& v) {
		(*this);
		(v);
		X += v.X;
		Y += v.Y;
		Z += v.Z;
		return *this;
	}

	__forceinline FVector& operator-=(const FVector& v) {
		(*this);
		(v);
		X -= v.X;
		Y -= v.Y;
		Z -= v.Z;
		return *this;
	}

	__forceinline FVector& operator*=(const FVector& v) {
		(*this);
		(v);
		X *= v.X;
		Y *= v.Y;
		Z *= v.Z;
		return *this;
	}

	__forceinline FVector& operator/=(const FVector& v) {
		(*this);
		(v);
		X /= v.X;
		Y /= v.Y;
		Z /= v.Z;
		return *this;
	}

	__forceinline bool operator==(const FVector& src) const {
		(src);
		(*this);
		return (src.X == X) && (src.Y == Y) && (src.Z == Z);
	}

	__forceinline bool operator!=(const FVector& src) const {
		(src);
		(*this);
		return (src.X != X) || (src.Y != Y) || (src.Z != Z);
	}

	__forceinline float Size() const {
		return crt::sqrtf(X * X + Y * Y + Z * Z);
	}

	__forceinline float Size2D() const {
		return crt::sqrtf(X * X + Y * Y);
	}

	__forceinline float SizeSquared() const {
		return X * X + Y * Y + Z * Z;
	}

	__forceinline float SizeSquared2D() const {
		return X * X + Y * Y;
	}

	__forceinline bool valid() const
	{
		return (X != 0 && Y != 0 && Z != 0);
	}

	__forceinline float Dot(const FVector& vOther) const {
		const FVector& a = *this;

		return (a.X * vOther.X + a.Y * vOther.Y + a.Z * vOther.Z);
	}

	__forceinline float DistanceFrom(const FVector& Other) const {
		const FVector& a = *this;
		float dx = (a.X - Other.X);
		float dy = (a.Y - Other.Y);
		float dz = (a.Z - Other.Z);

		return (crt::sqrtf((float)((dx * dx) + (dy * dy) + (dz * dz))));
	}

	float dis(FVector v)
	{
		return crt::sqrtf(v.X * v.X + v.Y * v.Y + v.Z * v.Z);
	}

	__forceinline FVector Normalize() {
		FVector vector;
		float length = this->Size();

		if (length != 0) {
			vector.X = X / length;
			vector.Y = Y / length;
			vector.Z = Z / length;
		}
		else
			vector.X = vector.Y = 0.0f;
		vector.Z = 1.0f;

		return vector;
	}

	static FORCEINLINE float InvSqrt(float F)
	{
		return 1.0f / crt::sqrtf(F);
	}

	FORCEINLINE FVector GetSafeNormal(float Tolerance) const
	{
		const float SquareSum = X * X + Y * Y + Z * Z;

		if (SquareSum == 1.f)
		{
			return *this;
		}
		else if (SquareSum < Tolerance)
		{
			return FVector();
		}

		const float Scale = InvSqrt(SquareSum);

		return FVector(X * Scale, Y * Scale, Z * Scale);
	}

	__forceinline void reset() {

		this->X = 0;
		this->Y = 0;
		this->Z = 0;
	}

};

class FTextData {
public:
	char pad_0x0000[0x28];  //0x0000
	wchar_t* Name;          //0x28 
	__int32 Length;         //0x40 

};

struct FText {
	FTextData* Data;
	char UnknownData[0x10];

	wchar_t* Get() const {
		if (Data)
			return Data->Name;

		return nullptr;
	}
};

class Matrix4
{
public:
	// Constructors
	Matrix4()
	{

	}
	~Matrix4()
	{

	}

	// Variables
	union {
		struct {
			float _11, _12, _13, _14;
			float _21, _22, _23, _24;
			float _31, _32, _33, _34;
			float _41, _42, _43, _44;
		};
		float m[4][4];
	};
};

struct FLinearColor
{
	float R;
	float G;
	float B;
	float A;

	inline FLinearColor()
		: R(0), G(0), B(0), A(0)
	{ }

	inline FLinearColor(float r, float g, float b, float a)
		: R(r / 255.0f),
		G(g / 255.0f),
		B(b / 255.0f),
		A(a)
	{ }
};

enum e_bones : int
{
	Root = 0,
	Hip = 1,
	Stabiliser = 2,
	Spine1 = 3,
	Spine2 = 4,
	Spine3 = 5,
	Spine4 = 6,
	UpperBody = 7,
	Neck = 8,
	NeckMiddle = 9,
	Head = 10,
	Face_Eye_L = 11,
	Face_Eye_R = 12,
	Face_Expressions = 13,
	Face_EyeLidUpperBase_L = 14,
	Face_EyeLidUpper_L = 15,
	Face_EyeLidLowerBase_L = 16,
	Face_EyeLidLower_L = 17,
	Face_EyeLidUpperBase_R = 18,
	Face_EyeLidUpper_R = 19,
	Face_EyeLidLowerBase_R = 20,
	Face_EyeLidLower_R = 21,
	Face_EyeBrow_C = 22,
	Face_EyeBrowInner_L = 23,
	Face_EyeBrowMiddle_L = 24,
	Face_EyeBrowOuter_L = 25,
	Face_EyeBrowInner_R = 26,
	Face_EyeBrowMiddle_R = 27,
	Face_EyeBrowOuter_R = 28,
	Face_EyeSocketInner_L = 29,
	Face_EyeSocketMiddle_L = 30,
	Face_EyeSocketInner_R = 31,
	Face_EyeSocketMiddle_R = 32,
	Face_CheekBoneInner_L = 33,
	Face_CheekBoneInner_R = 34,
	Face_Crease_L = 35,
	Face_Crease_R = 36,
	Face_Nose_C = 37,
	Face_Nose_L = 38,
	Face_Nose_R = 39,
	Face_Cheek_L = 40,
	Face_Cheek_R = 41,
	Face_LipUpper_C = 42,
	Face_LipUpperMid_L = 43,
	Face_LipUpperMid_R = 44,
	Face_Jaw = 45,
	Face_Tongue1 = 46,
	Face_Tongue2 = 47,
	Face_LipLower_C = 48,
	Face_LipLowerMid_L = 49,
	Face_LipCorner_L = 50,
	Face_LipLowerMid_R = 51,
	Face_LipCorner_R = 52,
	Face_Chin_C = 53,
	ArmsBase = 54,
	Clavicle_L = 55,
	Arm_L = 56,
	ForeArm_L = 57,
	Hand_L = 58,
	Thumb1_L = 59,
	Thumb2_L = 60,
	Thumb3_L = 61,
	Index0_L = 62,
	Index1_L = 63,
	Index2_L = 64,
	Index3_L = 65,
	Middle0_L = 66,
	Middle1_L = 67,
	Middle2_L = 68,
	Middle3_L = 69,
	Ring0_L = 70,
	Ring1_L = 71,
	Ring2_L = 72,
	Ring3_L = 73,
	Pinky0_L = 74,
	Pinky1_L = 75,
	Pinky2_L = 76,
	Pinky3_L = 77,
	Weapon_L = 78,
	ForeArm_Roll_L = 79,
	ForeArm_Roll2_L = 80,
	ForeArm_Roll3_L = 81,
	Arm_Roll_L = 82,
	Arm_Roll2_L = 83,
	Arm_Roll3_L = 84,
	Clavicle_R = 85,
	Arm_R = 86,
	ForeArm_R = 87,
	Hand_R = 88,
	Thumb1_R = 89,
	Thumb2_R = 90,
	Thumb3_R = 91,
	Index0_R = 92,
	Index1_R = 93,
	Index2_R = 94,
	Index3_R = 95,
	Middle0_R = 96,
	Middle1_R = 97,
	Middle2_R = 98,
	Middle3_R = 99,
	Ring0_R = 100,
	Ring1_R = 101,
	Ring2_R = 102,
	Ring3_R = 103,
	Pinky0_R = 104,
	Pinky1_R = 105,
	Pinky2_R = 106,
	Pinky3_R = 107,
	Weapon_R = 108,
	ForeArm_Roll_R = 109,
	ForeArm_Roll2_R = 110,
	ForeArm_Roll3_R = 111,
	Arm_Roll_R = 112,
	Arm_Roll2_R = 113,
	Arm_Roll3_R = 114,
	Leg_L = 115,
	Knee_L = 116,
	Ankle_L = 117,
	Ball_L = 118,
	Leg_R = 119,
	Knee_R = 120,
	Ankle_R = 121,
	Ball_R = 122,
	UpperBody_1stP = 123,
	HelmetBase = 124,
	Camera_1stP = 125,
	ArmsBase_1stP = 126,
	Clavicle_L_1stP = 127,
	Arm_L_1stP = 128,
	ForeArm_L_1stP = 129,
	Hand_L_1stP = 130,
	Thumb1_L_1stP = 131,
	Thumb2_L_1stP = 132,
	Thumb3_L_1stP = 133,
	Index0_L_1stP = 134,
	Index1_L_1stP = 135,
	Index2_L_1stP = 136,
	Index3_L_1stP = 137,
	Middle0_L_1stP = 138,
	Middle1_L_1stP = 139,
	Middle2_L_1stP = 140,
	Middle3_L_1stP = 141,
	Ring0_L_1stP = 142,
	Ring1_L_1stP = 143,
	Ring2_L_1stP = 144,
	Ring3_L_1stP = 145,
	Pinky0_L_1stP = 146,
	Pinky1_L_1stP = 147,
	Pinky2_L_1stP = 148,
	Pinky3_L_1stP = 149,
	Weapon_L_1stP = 150,
	ForeArm_Roll_L_1stP = 151,
	ForeArm_Roll2_L_1stP = 152,
	ForeArm_Roll3_L_1stP = 153,
	Arm_Roll_L_1stP = 154,
	Arm_Roll2_L_1stP = 155,
	Arm_Roll3_L_1stP = 156,
	Clavicle_R_1stP = 157,
	Arm_R_1stP = 158,
	ForeArm_R_1stP = 159,
	Hand_R_1stP = 160,
	Thumb1_R_1stP = 161,
	Thumb2_R_1stP = 162,
	Thumb3_R_1stP = 163,
	Index0_R_1stP = 164,
	Index1_R_1stP = 165,
	Index2_R_1stP = 166,
	Index3_R_1stP = 167,
	Middle0_R_1stP = 168,
	Middle1_R_1stP = 169,
	Middle2_R_1stP = 170,
	Middle3_R_1stP = 171,
	Ring0_R_1stP = 172,
	Ring1_R_1stP = 173,
	Ring2_R_1stP = 174,
	Ring3_R_1stP = 175,
	Pinky0_R_1stP = 176,
	Pinky1_R_1stP = 177,
	Pinky2_R_1stP = 178,
	Pinky3_R_1stP = 179,
	Weapon_R_1stP = 180,
	ForeArm_Roll_R_1stP = 181,
	ForeArm_Roll2_R_1stP = 182,
	ForeArm_Roll3_R_1stP = 183,
	Arm_Roll_R_1stP = 184,
	Arm_Roll2_R_1stP = 185,
	Arm_Roll3_R_1stP = 186,
	IK_Ankle_L = 187,
	IK_Ankle_R = 188,
	IK_Hand_L = 189,
	IK_Hand_R = 190
};

struct FPlane : public FVector
{
	double W;
};

struct FMatrix
{
	FPlane XPlane, YPlane, ZPlane, WPlane;

	FMatrix MatrixMultiplication(FMatrix Other)
	{
		FMatrix Copy = {};

		Copy.WPlane.X = this->WPlane.X * Other.XPlane.X + this->WPlane.Y * Other.YPlane.X + this->WPlane.Z * Other.ZPlane.X + this->WPlane.W * Other.WPlane.X;
		Copy.WPlane.Y = this->WPlane.X * Other.XPlane.Y + this->WPlane.Y * Other.YPlane.Y + this->WPlane.Z * Other.ZPlane.Y + this->WPlane.W * Other.WPlane.Y;
		Copy.WPlane.Z = this->WPlane.X * Other.XPlane.Z + this->WPlane.Y * Other.YPlane.Z + this->WPlane.Z * Other.ZPlane.Z + this->WPlane.W * Other.WPlane.Z;
		Copy.WPlane.W = this->WPlane.X * Other.XPlane.W + this->WPlane.Y * Other.YPlane.W + this->WPlane.Z * Other.ZPlane.W + this->WPlane.W * Other.WPlane.W;

		return Copy;
	}

	static FMatrix RotatorToMatrix(FVector Rotation)
	{
		auto ReturnValue = FMatrix();

		auto Pitch = (Rotation.X * 3.14159265358979323846 / 180);
		auto Yaw = (Rotation.Y * 3.14159265358979323846 / 180);
		auto Roll = (Rotation.Z * 3.14159265358979323846 / 180);

		auto sp = sin(Pitch);
		auto cp = cos(Pitch);
		auto sy = sin(Yaw);
		auto cy = cos(Yaw);
		auto sr = sin(Roll);
		auto cr = cos(Roll);

		ReturnValue.XPlane.X = cp * cy;
		ReturnValue.XPlane.Y = cp * sy;
		ReturnValue.XPlane.Z = sp;

		ReturnValue.YPlane.X = sr * sp * cy - cr * sy;
		ReturnValue.YPlane.Y = sr * sp * sy + cr * cy;
		ReturnValue.YPlane.Z = -sr * cp;

		ReturnValue.ZPlane.X = -(cr * sp * cy + sr * sy);
		ReturnValue.ZPlane.Y = cy * sr - cr * sp * sy;
		ReturnValue.ZPlane.Z = cr * cp;

		ReturnValue.WPlane.W = 0x1;

		return ReturnValue;
	}
};

struct FTransform
{
	FPlane Rotation;
	FVector Translation;
	char pad_0[4];
	FVector Scale3D;
	char pad_1[4];

	FMatrix ToMatrix()
	{
		auto ReturnValue = FMatrix();

		auto x2 = this->Rotation.X * 2;
		auto y2 = this->Rotation.Y * 2;
		auto z2 = this->Rotation.Z * 2;

		auto xx2 = this->Rotation.X * x2;
		auto yy2 = this->Rotation.Y * y2;
		auto zz2 = this->Rotation.Z * z2;

		auto yz2 = this->Rotation.Y * z2;
		auto wx2 = this->Rotation.W * x2;

		auto xy2 = this->Rotation.X * y2;
		auto wz2 = this->Rotation.W * z2;

		auto xz2 = this->Rotation.X * z2;
		auto wy2 = this->Rotation.W * y2;

		ReturnValue.XPlane.X = (1.0 - (yy2 + zz2)) * this->Scale3D.X;
		ReturnValue.XPlane.Y = (xy2 + wz2) * this->Scale3D.X;
		ReturnValue.XPlane.Z = (xz2 - wy2) * this->Scale3D.X;

		ReturnValue.YPlane.X = (xy2 - wz2) * this->Scale3D.Y;
		ReturnValue.YPlane.Y = (1.0 - (xx2 + zz2)) * this->Scale3D.Y;
		ReturnValue.YPlane.Z = (yz2 + wx2) * this->Scale3D.Y;

		ReturnValue.ZPlane.X = (xz2 + wy2) * this->Scale3D.Z;
		ReturnValue.ZPlane.Y = (yz2 - wx2) * this->Scale3D.Z;
		ReturnValue.ZPlane.Z = (1.0 - (xx2 + yy2)) * this->Scale3D.Z;

		ReturnValue.WPlane.X = this->Translation.X;
		ReturnValue.WPlane.Y = this->Translation.Y;
		ReturnValue.WPlane.Z = this->Translation.Z;

		ReturnValue.WPlane.W = 1.0;

		return ReturnValue;
	}
};



struct FVector2D
{
	float                                              X;                                                        // 0x0000(0x0004) (Edit, BlueprintVisible, ZeroConstructor, SaveGame, IsPlainOldData)
	float                                              Y;                                                        // 0x0004(0x0004) (Edit, BlueprintVisible, ZeroConstructor, SaveGame, IsPlainOldData)

	inline FVector2D()
		: X(0), Y(0)
	{ }

	inline FVector2D(float x, float y)
		: X(x),
		Y(y)
	{ }

	__forceinline FVector2D operator-(const FVector2D& V) {
		return FVector2D(X - V.X, Y - V.Y);
	}

	__forceinline FVector2D operator+(const FVector2D& V) {
		return FVector2D(X + V.X, Y + V.Y);
	}

	__forceinline FVector2D operator*(float Scale) const {
		return FVector2D(X * Scale, Y * Scale);
	}

	__forceinline FVector2D operator/(float Scale) const {
		const float RScale = 1.f / Scale;
		return FVector2D(X * RScale, Y * RScale);
	}

	__forceinline FVector2D operator+(float A) const {
		return FVector2D(X + A, Y + A);
	}

	__forceinline FVector2D operator-(float A) const {
		return FVector2D(X - A, Y - A);
	}

	__forceinline FVector2D operator*(const FVector2D& V) const {
		return FVector2D(X * V.X, Y * V.Y);
	}

	__forceinline FVector2D operator/(const FVector2D& V) const {
		return FVector2D(X / V.X, Y / V.Y);
	}

	__forceinline float operator|(const FVector2D& V) const {
		return X * V.X + Y * V.Y;
	}

	__forceinline float operator^(const FVector2D& V) const {
		return X * V.Y - Y * V.X;
	}

	__forceinline FVector2D& operator+=(const FVector2D& v) {
		(*this);
		(v);
		X += v.X;
		Y += v.Y;
		return *this;
	}

	__forceinline FVector2D& operator-=(const FVector2D& v) {
		(*this);
		(v);
		X -= v.X;
		Y -= v.Y;
		return *this;
	}

	__forceinline FVector2D& operator*=(const FVector2D& v) {
		(*this);
		(v);
		X *= v.X;
		Y *= v.Y;
		return *this;
	}

	__forceinline FVector2D& operator/=(const FVector2D& v) {
		(*this);
		(v);
		X /= v.X;
		Y /= v.Y;
		return *this;
	}

	__forceinline bool operator==(const FVector2D& src) const {
		(src);
		(*this);
		return (src.X == X) && (src.Y == Y);
	}

	__forceinline bool operator!=(const FVector2D& src) const {
		(src);
		(*this);
		return (src.X != X) || (src.Y != Y);
	}

	__forceinline float Size() const {
		return sqrt(X * X + Y * Y);
	}

	__forceinline float SizeSquared() const {
		return X * X + Y * Y;
	}

	__forceinline float Dot(const FVector2D& vOther) const {
		const FVector2D& a = *this;

		return (a.X * vOther.X + a.Y * vOther.Y);
	}

	__forceinline bool valid() {

		return (X != 0 && Y != 0);
	}

	__forceinline FVector2D Normalize() {
		FVector2D vector;
		float length = this->Size();

		if (length != 0) {
			vector.X = X / length;
			vector.Y = Y / length;
		}
		else
			vector.X = vector.Y = 0.0f;

		return vector;
	}

	__forceinline float DistanceFrom(const FVector2D& Other) const {
		const FVector2D& a = *this;
		float dx = (a.X - Other.X);
		float dy = (a.Y - Other.Y);

		return sqrt((dx * dx) + (dy * dy));
	}

	__forceinline void reset() const {

		auto tz = *this;
		tz.X = 0;
		tz.Y = 0;
	}

};

struct FRotator
{
	float                                              Pitch;                                                    // 0x0000(0x0004) (Edit, BlueprintVisible, ZeroConstructor, SaveGame, IsPlainOldData)
	float                                              Yaw;                                                      // 0x0004(0x0004) (Edit, BlueprintVisible, ZeroConstructor, SaveGame, IsPlainOldData)
	float                                              Roll;                                                     // 0x0008(0x0004) (Edit, BlueprintVisible, ZeroConstructor, SaveGame, IsPlainOldData)
};

struct FGuid
{
	uint32_t A;
	uint32_t B;
	uint32_t C;
	uint32_t D;
};

enum class EMaterialParameterAssociation : uint8
{
	LayerParameter = 0,
	BlendParameter = 1,
	GlobalParameter = 2,
	EMaterialParameterAssociation_MAX = 3,
};
struct FName {
	uint32_t Index;
	uint32_t Number;

	FName() :
		Index(0),
		Number(0)
	{ }

	FName(int32_t i) :
		Index(i),
		Number(0)
	{ }

	__forceinline bool operator==(const FName& src) const {
		(src);
		(*this);

		return (src.Index == Index);
	}

	std::string GetName();
};

namespace Colors
{
	FLinearColor Blue = { 0.f, 0.f, 255.f, 1.f };
	FLinearColor Cyan = { 0.f, 255.f, 255.f, 1.f };
	FLinearColor Red = { 255.f, 0.f, 0.f, 1.f };
	FLinearColor White = { 255.f, 255.f, 255.0f, 1.f };
	FLinearColor Yellow = { 255.f, 255.f, 0.f, 1.f };
	FLinearColor Black = { 0.f, 0.f, 0.f, 255.f };
};

struct FMinimalViewInfo {
	struct FVector Location; // 0x00(0x0c)
	struct FRotator Rotation; // 0x0c(0x0c)
	float FOV; // 0x18(0x04)
};

enum class ECollisionChannel : uint8_t
{
	ECC_WorldStatic = 0,
	ECC_WorldDynamic = 1,
	ECC_Pawn = 2,
	ECC_Visibility = 3,
	ECC_Camera = 4,
	ECC_PhysicsBody = 5,
	ECC_Vehicle = 6,
	ECC_Destructible = 7,
	ECC_EngineTraceChannel1 = 8,
	ECC_EngineTraceChannel2 = 9,
	ECC_EngineTraceChannel3 = 10,
	ECC_EngineTraceChannel4 = 11,
	ECC_EngineTraceChannel5 = 12,
	ECC_EngineTraceChannel6 = 13,
	ECC_GameTraceChannel1 = 14,
	ECC_GameTraceChannel2 = 15,
	ECC_GameTraceChannel3 = 16,
	ECC_GameTraceChannel4 = 17,
	ECC_GameTraceChannel5 = 18,
	ECC_GameTraceChannel6 = 19,
	ECC_GameTraceChannel7 = 20,
	ECC_GameTraceChannel8 = 21,
	ECC_GameTraceChannel9 = 22,
	ECC_GameTraceChannel10 = 23,
	ECC_GameTraceChannel11 = 24,
	ECC_GameTraceChannel12 = 25,
	ECC_GameTraceChannel13 = 26,
	ECC_GameTraceChannel14 = 27,
	ECC_GameTraceChannel15 = 28,
	ECC_GameTraceChannel16 = 29,
	ECC_GameTraceChannel17 = 30,
	ECC_GameTraceChannel18 = 31,
	ECC_OverlapAll_Deprecated = 32,
	ECC_MAX = 33
};

template<class TEnum>
class TEnumAsByte
{
public:
	inline TEnumAsByte()
	{
	}

	inline TEnumAsByte(TEnum _value)
		: value(static_cast<uint8_t>(_value))
	{
	}

	explicit inline TEnumAsByte(int32_t _value)
		: value(static_cast<uint8_t>(_value))
	{
	}

	explicit inline TEnumAsByte(uint8_t _value)
		: value(_value)
	{
	}

	inline operator TEnum() const
	{
		return (TEnum)value;
	}

	inline TEnum GetValue() const
	{
		return (TEnum)value;
	}

private:
	uint8_t value;
};