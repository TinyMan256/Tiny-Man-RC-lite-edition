#pragma once
#include <windows.h>
#include <psapi.h>
#include <tlhelp32.h>
#include <iostream>
#include <dwmapi.h>
#include  <d3d9.h>
#include  <d3dx9.h>
#include "other.h"
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "dwmapi.lib")
          // ====================== goto https://rc.dumps.host/ to update offsets =======================
inline namespace RogueCompany
{
	class Variables : public Singleton<Variables>
	{
	public:
		const char* dwProcessName = "RogueCompany.exe";
		DWORD dwProcessId = NULL;
		uint64_t dwProcess_Base = NULL;
		HWND gameHWND = NULL;
		int actor_count = NULL;
		int ScreenHeight = NULL;
		int ScreenWidth = NULL;
		int ScreenLeft = NULL;
		int ScreenRight = NULL;
		int ScreenTop = NULL;
		int ScreenBottom = NULL;
		float ScreenCenterX = ScreenWidth / 2;
		float ScreenCenterY = ScreenHeight / 2;
		DWORD_PTR game_instance = NULL;
		DWORD_PTR u_world = NULL;
		DWORD_PTR local_player_pawn = NULL;
		DWORD_PTR local_player_array = NULL;
		DWORD_PTR local_player = NULL;
		DWORD_PTR local_player_root = NULL;
		DWORD_PTR local_player_controller = NULL;
		DWORD_PTR local_player_state = NULL;
		DWORD_PTR persistent_level = NULL;
		DWORD_PTR actors = NULL;
	};
#define GameVars RogueCompany::Variables::Get()
	class Offsets : public Singleton<Offsets>
	{
	public:
		DWORD offset_g_names = 0x6934500; // https://rc.dumps.host/offsets GNames //ok
		DWORD offset_u_world = 0x6aa34d8; // https://rc.dumps.host/offsets GWorld //ok
		DWORD offset_camera_manager = 0x2b8; // APlayerController -> PlayerCameraManager //ok
		DWORD offset_camera_cache = 0x1aa0; //APlayerCameraManager -> CameraCachePrivate //ok
		DWORD offset_persistent_level = 0x30; //UWorld -> PersistentLevel //ok
		DWORD offset_game_instance = 0x188; //UWorld -> OwningGameInstance //ok
		DWORD offset_local_players_array = 0x38; //UGameInstance -> LocalPlayers //ok
		DWORD offset_player_controller = 0x30; //UPlayer -> PlayerController //ok
		DWORD offset_apawn = 0x2a0;  //APlayerController -> AcknowledgedPawn //ok
		DWORD offset_root_component = 0x130; //AActor -> RootComponent //ok
		DWORD offset_actor_array = 0x98; //UNetConnection -> OwningActor //ok
		DWORD offset_actor_count = 0xa0; //UNetConnection -> MaxPacket //ok
		DWORD offset_player_state = 0x240; //APawn -> PlayerState //ok
		DWORD offset_actor_mesh = 0x280; //ACharacter -> Mesh //ok
		DWORD offset_relative_location = 0x11c; //USceneComponent -> RelativeLocation //ok
		DWORD offset_health = 0x538; //AKSCharacterFoundation -> Health //ok
		DWORD offset_max_health = 0x28ac; //AKSCharacter -> CachedMaxHealth //ok
		DWORD offset_player_name = 0x300; // APlayerState -> PlayerNamePrivate //ok
		DWORD BaseAccuracy = 0x67c; // UKSWeaponAsset -> BaseAccuracy //ok
		DWORD bDowned = 0x28d9; // AKSCharacter -> bDowned //ok
		DWORD offset_bKickbackEnabled = 0xaf8; // AKSPlayerController -> bKickbackEnabled //ok
		DWORD bInRagdoll = 0x2e80; // AKSCharacter -> bInRagdoll
		DWORD bCanMoveWhileRevivingOther = 0x2a68; // AKSCharacter -> bCanMoveWhileRevivingOther //ok
		DWORD ping = 0x228; // APlayerState -> Ping //ok
		DWORD r_Team = 0x398; // AKSPlayerState -> r_Team //ok
		DWORD r_TeamNum = 0x220; // AKSTeamState -> r_TeamNum //ok
		DWORD EliminatedState = 0x448; // AKSPlayerState -> EliminatedState //ok
		DWORD TimeDilation = 0x2e8; // AWorldSettings -> TimeDilation //ok
		DWORD ActiveWeaponComponent = 0x22e8; // AKSCharacter -> ActiveWeaponComponent[0xb] //ok
		DWORD WeaponAsset = 0x138; // UKSWeaponComponent -> WeaponAsset //ok
		DWORD bUnlimitedAmmo = 0x3d8; // UKSWeaponAsset -> bUnlimitedAmmo //ok
		DWORD MaxLungeDistance = 0x9a4; //UKSWeaponAsset_Melee -> MaxLungeDistance //ok
		DWORD MeleeSphereRadius = 0x984; //UKSWeaponAsset_Melee -> MeleeSphereRadius //ok
		DWORD bRevealed = 0x12fc; //AKSCharacterBase -> bRevealed //ok
		DWORD bIsSpawnImmune = 0x1518; //AKSCharacterBase -> bIsSpawnImmune //ok
		DWORD bIgnoreCollisionWithTeammates = 0x4ed; //AKSCharacterFoundation -> bIgnoreCollisionWithTeammates //ok
		DWORD CurrentOverheal = 0xd90; //AKSCharacterBase -> CurrentOverheal //ok
		DWORD offset_last_submit_time = 0x2ac; // AServerStatReplicator -> NumRelevantDeletedActors //ok
		DWORD offset_last_render_time = 0x2b4; // AServerStatReplicator -> NumReplicatedActors //ok
		DWORD offset_actor_id = 0x18; // actor id //ok
		DWORD offset_component_to_world = 0x1c0; // component to world //ok
		DWORD offset_bone_array = 0x478; // UStaticMeshComponent -> StaticMesh //ok
		DWORD CurrentQuickMeleeWeapon = 0x2230; //AKSCharacter -> CurrentQuickMeleeWeapon //ok
		DWORD MeleeWeaponAsset = 0x868; //AKSWeapon_Melee -> MeleeWeaponAsset //ok
		DWORD BaseEyeHeight = 0x22c; //APawn -> BaseEyeHeight //ok
		DWORD AmmoInClip = 0x4a4; //AKSWeapon -> AmmoInClip //ok
		DWORD ClipSize = 0x4a8; //AKSWeapon -> ClipSize //ok
		DWORD DefaultFOV = 0x238; //APlayerCameraManager -> DefaultFOV //ok
		DWORD NoSpread = 0x67c; //UKSWeaponAsset -> BaseAccuracy //ok
		DWORD firemodetype = 0x368; //UKSWeaponAsset -> FireModeType //ok
		DWORD CurrentRevealColor = 0x1300; //AKSCharacterBase -> CurrentRevealColor //ok
		DWORD CharacterMovement = 0x288; //ACharacter -> CharacterMovement //ok
		DWORD KSCharacterOwner = 0x868; //UKSCharacterMovementComponent -> KSCharacterOwner //ok
		DWORD SpawnImmuneOutlineColorEnemy = 0x1318; //AKSCharacterBase -> SpawnImmuneOutlineColorEnemy //ok
		DWORD PreFireTime = 0x378; //UKSWeaponAsset -> PreFireTime //ok
		DWORD RoundsPerBurst = 0x36c; //UKSWeaponAsset -> RoundsPerBurst //ok
		DWORD AimDownSightsFov = 0x5c0; //UKSWeaponAsset -> AimDownSightsFov //ok
		DWORD PostReloadTime = 0x3c0; //UKSWeaponAsset -> PostReloadTime //ok
		DWORD PreReloadTime = 0x3bc; //UKSWeaponAsset -> PreReloadTime //ok
		DWORD trollnoreload = 0x3b0; //UKSWeaponAsset -> bCanEverReload //ok
		DWORD trollnofire = 0x330; //UKSWeaponAsset -> bCanEverFire //ok
		DWORD aimovershoulder = 0x5ac; //UKSWeaponAsset -> bCanEverAimOverShoulder //ok
		DWORD aimdownsights = 0x5b4; //UKSWeaponAsset -> bCanEverAimDownSights //ok
		DWORD ReloadType = 0x3b8; //UKSWeaponAsset -> ReloadType //ok
		DWORD DamageCategory = 0x40; //UKSDamageTypeBase -> DamageCategory //ok
		DWORD ReticleType = 0x3e9; //UKSWeaponAsset -> ReticleType //ok
		DWORD WeaponEquipType = 0x280; //UKSWeaponAsset -> WeaponEquipType //ok
		DWORD WalkSpeedModifier = 0x280; //UKSWeaponAsset -> WeaponEquipType //ok
		DWORD ClipSize2 = 0x3b4; //UKSWeaponAsset -> ClipSize //ok
		DWORD HeadDamageAmount = 0x534; //UKSWeaponAsset -> HeadDamageAmount //ok
		DWORD DamageAmount = 0x538; //UKSWeaponAsset -> DamageAmount //ok
		DWORD LimbDamageAmount = 0x53c; //UKSWeaponAsset -> LimbDamageAmount //ok
		DWORD BurstShotIndex = 0x45c; //AKSWeapon -> BurstShotIndex //ok
		DWORD PostFireTime = 0x384; //UKSWeaponAsset -> PostFireTime //ok
		DWORD AltPostFireTime = 0x388; //UKSWeaponAsset -> AltPostFireTime //ok
		DWORD PostFireForgivenessTime = 0x38c; //UKSWeaponAsset -> PostFireForgivenessTime //ok
		DWORD InitialPostFireTime = 0x39c; //UKSWeaponAsset -> InitialPostFireTime //ok
		DWORD FinalPostFireTime = 0x3a0; //UKSWeaponAsset -> FinalPostFireTime //ok
		DWORD PostFireChargeTime = 0x3a4; //UKSWeaponAsset -> PostFireChargeTime //ok
		DWORD PostFireDecayTime = 0x3a8; //UKSWeaponAsset -> PostFireDecayTime //ok
		DWORD PostFireDecayDelay = 0x3ac; //UKSWeaponAsset -> PostFireDecayDelay //ok
		DWORD AmmoType = 0x3d1; //UKSWeaponAsset -> AmmoType //ok
		DWORD ReloadCooldownTime = 0x3c4; //UKSWeaponAsset -> ReloadCooldownTime //ok
		DWORD FirstShotPreReloadTime = 0x3cc; //UKSWeaponAsset -> FirstShotPreReloadTime //ok
		DWORD etest = 0x371; //UKSWeaponAsset ->
		DWORD DamageTypeClass = 0x578; //UKSWeaponAsset -> DamageTypeClass //ok
	};
#define GameOffset RogueCompany::Offsets::Get()
	uintptr_t GNames = 0x6c7f180;
	uintptr_t GObjects = 0x6cab3f8;
	uintptr_t GWorld = 0x6dee1b8;
}
