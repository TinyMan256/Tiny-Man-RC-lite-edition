#define _CRT_SECURE_NO_WARNINGS
#include "function.h"
#include "overlay.h"
#include <mmsystem.h>
#include <d3dx9.h>
#include "other.h";
auto characterbase()
{
	auto m_Charatermovement = read<uint64_t>(GameVars.local_player_pawn + GameOffset.CharacterMovement);
	return read<uint64_t>(m_Charatermovement + GameOffset.KSCharacterOwner);
}
auto weaponasset()
{
	auto ActiveWeapon = read<uint64_t>(GameVars.local_player_pawn + GameOffset.ActiveWeaponComponent);
	return read<uint64_t>(ActiveWeapon + GameOffset.WeaponAsset);
}
auto MeleeWeaponAsset()
{
	uint64_t CurrentQuickMeleeWeapon = read<uint64_t>(GameVars.local_player_pawn + GameOffset.CurrentQuickMeleeWeapon);
	return read<uint64_t>(CurrentQuickMeleeWeapon + GameOffset.MeleeWeaponAsset);
}
enum class EReloadType : uint8_t
{
	Clip = 0,
	SingleShot = 1,
	EReloadType_MAX = 2,
};
enum class EFireModeType : uint8_t
{
	Single = 0,
	Burst = 1,
	SemiAuto = 2,
	EFireModeType_MAX = 3
};
namespace OverlayWindow
{
	WNDCLASSEX WindowClass;
	HWND Hwnd;
	LPCSTR Name;
}
void removecheat()
{
	rcdriv::unload();
	char filename[] = rcdriv_SYSFILE_NAME;
	int result = remove(filename);
	exit(0);
}
bool find(const char* name)
{
	PROCESSENTRY32 entry;
	entry.dwSize = sizeof(PROCESSENTRY32);

	auto snapshot = LI_FN(CreateToolhelp32Snapshot).safe()(TH32CS_SNAPPROCESS, NULL);

	if (LI_FN(Process32First).safe()(snapshot, &entry) == TRUE)
	{
		while (LI_FN(Process32Next).safe()(snapshot, &entry) == TRUE)
		{
			if (!strcmp(entry.szExeFile, name))
			{

				return true;
			}
		}
	}

	LI_FN(CloseHandle).safe()(snapshot);
	return false;
}
namespace DirectX9Interface
{
	IDirect3D9Ex* Direct3D9 = NULL;
	IDirect3DDevice9Ex* pDevice = NULL;
	D3DPRESENT_PARAMETERS pParams = { NULL };
	MARGINS Margin = { -1 };
	MSG Message = { NULL };
}
typedef struct _EntityList
{
	uintptr_t actor_pawn;
	uintptr_t actor_mesh;
	uintptr_t actor_state;
	int actor_id;

}EntityList;
vector<EntityList> entityList;
auto CallAimbot()->VOID
{
	while (true)
	{
		auto EntityList_Copy = entityList;
			cfg.aimbot = GetAsyncKeyState(VK_RBUTTON);
		if (cfg.aimbot == true)
		{
			float target_dist = FLT_MAX;
			EntityList target_entity = {};
			for (int index = 0; index < EntityList_Copy.size(); ++index)
			{
				auto Entity = EntityList_Copy[index];
				auto Health = read<float>(Entity.actor_pawn + GameOffset.offset_health);
				auto MaxHealth = read<float>(Entity.actor_pawn + GameOffset.offset_max_health);
				auto Percentage = Health * 100 / MaxHealth;
				if (!Entity.actor_mesh || !isVisible(Entity.actor_mesh)) continue;

				if (cfg.Nodieaim) {
					if (Percentage == 0 || !Entity.actor_state || !Entity.actor_pawn)
					{
						continue;
					}
				}
				auto Playerstate = read<uintptr_t>(Entity.actor_pawn + GameOffset.offset_player_state);
				uint64_t LAKSTeamState = read<uint64_t>(GameVars.local_player_state + GameOffset.r_Team);
				uint64_t LTeamNum = read<uint64_t>(LAKSTeamState + GameOffset.r_TeamNum);
				uint64_t AKSTeamState = read<uint64_t>(Playerstate + GameOffset.r_Team);
				uint64_t TeamNum = read<uint64_t>(AKSTeamState + GameOffset.r_TeamNum);
				if (LTeamNum != TeamNum)
				{
				}
				else {
					if (cfg.friendsaim == false)
					{
						continue;
					}
				}
				if (cfg.nodownaim == true)
				{
					auto knocked = read<uintptr_t>(Entity.actor_pawn + GameOffset.bDowned);
					if (knocked == true)
					{
						continue;
					}
					else {
						NULL;
					}
				}
				auto head_pos = GetBoneWithRotation(Entity.actor_mesh, 110);
				auto targethead = ProjectWorldToScreen(Vector3(head_pos.x, head_pos.y, head_pos.z));
				float x = targethead.x - GameVars.ScreenWidth / 2.0f;
				float y = targethead.y - GameVars.ScreenHeight / 2.0f;
				float crosshair_dist = sqrtf((x * x) + (y * y));
				if (crosshair_dist <= FLT_MAX && crosshair_dist <= target_dist)
				{
					if (crosshair_dist > cfg.AimbotFOV)
					{
						continue;
					}
					target_dist = crosshair_dist;
					target_entity = Entity;
				}
			}
			if (target_entity.actor_mesh != 0 || target_entity.actor_pawn != 0 || target_entity.actor_id != 0)
			{
						auto target = GetBoneWithRotation(target_entity.actor_mesh, 110);
						auto loc = ProjectWorldToScreen(Vector3(target.x, target.y, target.z));
						move_to(loc.x, loc.y);
			}
			else {
				continue;
			}
			auto head_pos = GetBoneWithRotation(target_entity.actor_mesh, 110);
			auto targethead = ProjectWorldToScreen(Vector3(head_pos.x, head_pos.y, head_pos.z));
		}
		Sleep(1);
	}
}
auto GameCache()->VOID
{
	while (true)
	{
		vector<EntityList> tmpList;
		GameVars.u_world = read<DWORD_PTR>(GameVars.dwProcess_Base + GameOffset.offset_u_world);
		GameVars.game_instance = read<DWORD_PTR>(GameVars.u_world + GameOffset.offset_game_instance);
		GameVars.local_player_array = read<DWORD_PTR>(GameVars.game_instance + GameOffset.offset_local_players_array);
		GameVars.local_player = read<DWORD_PTR>(GameVars.local_player_array);
		GameVars.local_player_controller = read<DWORD_PTR>(GameVars.local_player + GameOffset.offset_player_controller);
		GameVars.local_player_pawn = read<DWORD_PTR>(GameVars.local_player_controller + GameOffset.offset_apawn);
		GameVars.local_player_root = read<DWORD_PTR>(GameVars.local_player_pawn + GameOffset.offset_root_component);
		GameVars.local_player_state = read<DWORD_PTR>(GameVars.local_player_pawn + GameOffset.offset_player_state);
		GameVars.persistent_level = read<DWORD_PTR>(GameVars.u_world + GameOffset.offset_persistent_level);
		GameVars.actors = read<DWORD_PTR>(GameVars.persistent_level + GameOffset.offset_actor_array);
		GameVars.actor_count = read<int>(GameVars.persistent_level + GameOffset.offset_actor_count);
		for (int index = 0; index < GameVars.actor_count; ++index)
		{
			auto actor_pawn = read<DWORD_PTR>(GameVars.actors + index * 0x8);
			if (actor_pawn == 0x00){continue;}
			auto actor_id = read<int>(actor_pawn + GameOffset.offset_actor_id);
			auto actor_state = read<DWORD_PTR>(actor_pawn + GameOffset.offset_player_state);
			auto actor_mesh = read<DWORD_PTR>(actor_pawn + GameOffset.offset_actor_mesh);
			auto name = GetNameFromFName(actor_id);
			EntityList Entity{ };
			Entity.actor_pawn = actor_pawn;
			Entity.actor_id = actor_id;
			if (cfg.enemiesesp == true)
			{
				if (name == "MainCharacter_C" || name == "MainCharacter_Royale_C" || name == "DefaultPVPBotCharacter_C" || name == "DefaultBotCharacter_C")
				{
					if (actor_pawn != NULL || actor_id != NULL || actor_state != NULL || actor_mesh != NULL)
					{
						EntityList Entity{ };
						Entity.actor_pawn = actor_pawn;
						Entity.actor_id = actor_id;
						Entity.actor_state = actor_state;
						Entity.actor_mesh = actor_mesh;
						tmpList.push_back(Entity);
					}
				}
			}
		}
		entityList = tmpList;
	}
}
auto RenderVisual()->VOID
{
	auto EntityList_Copy = entityList;
	for (int index = 0; index < EntityList_Copy.size(); ++index)
	{
		auto Entity = EntityList_Copy[index];
		auto Health = read<float>(Entity.actor_pawn + GameOffset.offset_health);
		auto MaxHealth = read<float>(Entity.actor_pawn + GameOffset.offset_max_health);
		auto Percentage = Health * 100 / MaxHealth;
			if (Percentage == 0 || !Entity.actor_mesh || !Entity.actor_state || !Entity.actor_pawn)
			{
				continue;
			}
			auto knocked = read<uintptr_t>(Entity.actor_pawn + GameOffset.bDowned);
			if (knocked == true)
			{
				continue;
			}
		auto Playerstate = read<uintptr_t>(Entity.actor_pawn + GameOffset.offset_player_state);
		uint64_t LAKSTeamState = read<uint64_t>(GameVars.local_player_state + GameOffset.r_Team);
		uint64_t LTeamNum = read<uint64_t>(LAKSTeamState + GameOffset.r_TeamNum);
		uint64_t AKSTeamState = read<uint64_t>(Playerstate + GameOffset.r_Team);
		uint64_t TeamNum = read<uint64_t>(AKSTeamState + GameOffset.r_TeamNum);
		if (LTeamNum != TeamNum)
		{
		}
		else {
			if (cfg.friendsesp == false)
			{
				continue;
			}
		}
		auto local_pos = read<Vector3>(GameVars.local_player_root + GameOffset.offset_relative_location);
		auto head_pos = GetBoneWithRotation(Entity.actor_mesh, 110);
		auto bone_pos = GetBoneWithRotation(Entity.actor_mesh, 0);
		auto BottomBox = ProjectWorldToScreen(bone_pos);
		auto TopBox = ProjectWorldToScreen(Vector3(head_pos.x, head_pos.y, head_pos.z + 15));
		auto entity_distance = local_pos.Distance(bone_pos);
		auto CornerHeight = abs(TopBox.y - BottomBox.y);
		auto CornerWidth = CornerHeight * 0.65;
		auto bVisible = isVisible(Entity.actor_mesh);
		auto ESP_Color = GetVisibleColor(bVisible);
			write<float>(weaponasset() + GameOffset.PostFireTime, cfg.f_fastbulletsmelee);
			write<float>(weaponasset() + GameOffset.AltPostFireTime, cfg.f_fastbulletsmelee);
			write<float>(weaponasset() + GameOffset.PostFireForgivenessTime, cfg.f_fastbulletsmelee);
			write<float>(weaponasset() + GameOffset.InitialPostFireTime, cfg.f_fastbulletsmelee);
			write<float>(weaponasset() + GameOffset.FinalPostFireTime, cfg.f_fastbulletsmelee);
			write<float>(weaponasset() + GameOffset.PostFireChargeTime, cfg.f_fastbulletsmelee);
			write<float>(weaponasset() + GameOffset.PostFireDecayTime, cfg.f_fastbulletsmelee);
			write<float>(weaponasset() + GameOffset.PostFireDecayDelay, cfg.f_fastbulletsmelee);
			write<float>(weaponasset() + GameOffset.PreReloadTime, 0.1f);
			write<EReloadType>(weaponasset() + GameOffset.ReloadType, EReloadType::Clip);
			write<EFireModeType>(weaponasset() + GameOffset.firemodetype, EFireModeType::SemiAuto);
			write<float>(weaponasset() + GameOffset.NoSpread, 100.0f);
			write<bool>(weaponasset() + GameOffset.bUnlimitedAmmo, true);
			write<bool>(weaponasset() + GameOffset.bUnlimitedAmmo, false);
			write<bool>(GameVars.local_player_controller + GameOffset.offset_bKickbackEnabled, false);
			if (entity_distance < cfg.max_distance)
			{
						DrawCorneredBox(TopBox.x - (CornerWidth / 2), TopBox.y, CornerWidth, CornerHeight, ESP_Color, cfg.boxwidth);
			
						DrawLine(ImVec2(static_cast<float>(GameVars.ScreenWidth / 2), 0.f), ImVec2(TopBox.x, TopBox.y), ESP_Color, 1.5f); //LINE FROM TOP SCREEN
				if (cfg.b_EspDistance)
				{
					char dist[64];
					sprintf_s(dist, "D:%.fm", entity_distance);
					DrawOutlinedText(Verdana, dist, ImVec2(BottomBox.x, BottomBox.y), 14.0f, ImColor(255, 255, 255), true);
				}
				if (cfg.healthbar)
				{
					auto Health = read<float>(Entity.actor_pawn + GameOffset.offset_health);
					auto MaxHealth = read<float>(Entity.actor_pawn + GameOffset.offset_max_health);
					float Percentage = Health * 100 / MaxHealth;
					float width = CornerWidth / 10;
					if (width < 2.f) width = 2.;
					if (width > 3) width = 3.;
					HealthBar(TopBox.x - (CornerWidth / 2) - 8, TopBox.y, width, BottomBox.y - TopBox.y, Percentage);
				}
				if (cfg.healthpercenet)
				{
					float Health = read<float>(Entity.actor_pawn + GameOffset.offset_health);
					float MaxHealth = read<float>(Entity.actor_pawn + GameOffset.offset_max_health);
					float Percentage = Health * 100 / MaxHealth;
					if (Percentage <= 25)
					{
						char healthpercenet[64];
						sprintf(healthpercenet, "H:%.f%%", Percentage);
						DrawOutlinedText(Verdana, healthpercenet, ImVec2(TopBox.x, TopBox.y), 14.0f, ImColor(255, 38, 0), true);
					}
					if (Percentage <= 50 && Percentage >= 25)
					{
						char healthpercenet[64];
						sprintf(healthpercenet, "H:%.f%%", Percentage);
						DrawOutlinedText(Verdana, healthpercenet, ImVec2(TopBox.x, TopBox.y), 14.0f, ImColor(255, 77, 0), true);
					}
					if (Percentage <= 75 && Percentage >= 50)
					{
						char healthpercenet[64];
						sprintf(healthpercenet, "H:%.f%%", Percentage);
						DrawOutlinedText(Verdana, healthpercenet, ImVec2(TopBox.x, TopBox.y), 14.0f, ImColor(255, 200, 0), true);
					}
					if (Percentage <= 100 && Percentage >= 75)
					{
						char healthpercenet[64];
						sprintf(healthpercenet, "H:%.f%%", Percentage);
						DrawOutlinedText(Verdana, healthpercenet, ImVec2(TopBox.x, TopBox.y), 14.0f, ImColor(0, 255, 0, 255), true);
					}
				}
				if (cfg.fullnameesp == true)
				{
					auto PlayerName = read<FString>(Entity.actor_state + GameOffset.offset_player_name);
					DrawOutlinedText(Verdana, PlayerName.ToString(), ImVec2(TopBox.x, TopBox.y - 20), 14.0f, cfg.fullPlayerName, true);
				}
			}
		}
}
void Render()
{
	bool Unloadkey = GetUnloadKey();
	if (Unloadkey)
	{
		removecheat();
	}
	GameVars.dwProcessId = GetProcessId(GameVars.dwProcessName);
	if (!GameVars.dwProcessId)
	{
		MessageBox(0, "RC Cheat was completely unloaded", "Information", MB_OK | MB_TOPMOST | MB_ICONINFORMATION);
		removecheat();
	}
	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	RenderVisual();
	ImGui::EndFrame();
	DirectX9Interface::pDevice->SetRenderState(D3DRS_ZENABLE, false);
	DirectX9Interface::pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
	DirectX9Interface::pDevice->SetRenderState(D3DRS_SCISSORTESTENABLE, false);
	DirectX9Interface::pDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_ARGB(0, 0, 0, 0), 1.0f, 0);
	if (DirectX9Interface::pDevice->BeginScene() >= 0) {
		ImGui::Render();
		ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
		DirectX9Interface::pDevice->EndScene();
	}
	HRESULT result = DirectX9Interface::pDevice->Present(NULL, NULL, NULL, NULL);
	if (result == D3DERR_DEVICELOST && DirectX9Interface::pDevice->TestCooperativeLevel() == D3DERR_DEVICENOTRESET) {
		ImGui_ImplDX9_InvalidateDeviceObjects();
		DirectX9Interface::pDevice->Reset(&DirectX9Interface::pParams);
		ImGui_ImplDX9_CreateDeviceObjects();
	}
}
void MainLoop() {
	static RECT OldRect;
	ZeroMemory(&DirectX9Interface::Message, sizeof(MSG));
	while (DirectX9Interface::Message.message != WM_QUIT) {
		if (PeekMessage(&DirectX9Interface::Message, OverlayWindow::Hwnd, 0, 0, PM_REMOVE)) {
			TranslateMessage(&DirectX9Interface::Message);
			DispatchMessage(&DirectX9Interface::Message);
		}
		HWND ForegroundWindow = GetForegroundWindow();
		if (ForegroundWindow == GameVars.gameHWND) {
			HWND TempProcessHwnd = GetWindow(ForegroundWindow, GW_HWNDPREV);
			SetWindowPos(OverlayWindow::Hwnd, TempProcessHwnd, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
		}
		RECT TempRect;
		POINT TempPoint;
		ZeroMemory(&TempRect, sizeof(RECT));
		ZeroMemory(&TempPoint, sizeof(POINT));
		GetClientRect(GameVars.gameHWND, &TempRect);
		ClientToScreen(GameVars.gameHWND, &TempPoint);
		TempRect.left = TempPoint.x;
		TempRect.top = TempPoint.y;
		ImGuiIO& io = ImGui::GetIO();
		io.ImeWindowHandle = GameVars.gameHWND;

		POINT TempPoint2;
		GetCursorPos(&TempPoint2);
		io.MousePos.x = TempPoint2.x - TempPoint.x;
		io.MousePos.y = TempPoint2.y - TempPoint.y;

		if (GetAsyncKeyState(0x1)) {
			io.MouseDown[0] = true;
			io.MouseClicked[0] = true;
			io.MouseClickedPos[0].x = io.MousePos.x;
			io.MouseClickedPos[0].x = io.MousePos.y;
		}
		else {
			io.MouseDown[0] = false;
		}

		if (TempRect.left != OldRect.left || TempRect.right != OldRect.right || TempRect.top != OldRect.top || TempRect.bottom != OldRect.bottom) {
			OldRect = TempRect;
			GameVars.ScreenWidth = TempRect.right;
			GameVars.ScreenHeight = TempRect.bottom;
			DirectX9Interface::pParams.BackBufferWidth = GameVars.ScreenWidth;
			DirectX9Interface::pParams.BackBufferHeight = GameVars.ScreenHeight;
			SetWindowPos(OverlayWindow::Hwnd, (HWND)0, TempPoint.x, TempPoint.y, GameVars.ScreenWidth, GameVars.ScreenHeight, SWP_NOREDRAW);
			DirectX9Interface::pDevice->Reset(&DirectX9Interface::pParams);
		}
		Render();
	}
	if (DirectX9Interface::pDevice != NULL) {
		DirectX9Interface::pDevice->EndScene();
		DirectX9Interface::pDevice->Release();
	}
	if (DirectX9Interface::Direct3D9 != NULL) {
		DirectX9Interface::Direct3D9->Release();
	}
}
bool DirectXInit() {
	if (FAILED(Direct3DCreate9Ex(D3D_SDK_VERSION, &DirectX9Interface::Direct3D9))) {
		return false;
	}

	D3DPRESENT_PARAMETERS Params = { 0 };
	Params.Windowed = TRUE;
	Params.SwapEffect = D3DSWAPEFFECT_DISCARD;
	Params.hDeviceWindow = OverlayWindow::Hwnd;
	Params.MultiSampleQuality = D3DMULTISAMPLE_NONE;
	Params.BackBufferFormat = D3DFMT_A8R8G8B8;
	Params.BackBufferWidth = GameVars.ScreenWidth;
	Params.BackBufferHeight = GameVars.ScreenHeight;
	Params.PresentationInterval = D3DPRESENT_INTERVAL_ONE;
	Params.EnableAutoDepthStencil = TRUE;
	Params.AutoDepthStencilFormat = D3DFMT_D16;
	Params.PresentationInterval = D3DPRESENT_INTERVAL_ONE;
	Params.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;

	if (FAILED(DirectX9Interface::Direct3D9->CreateDeviceEx(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, OverlayWindow::Hwnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &Params, 0, &DirectX9Interface::pDevice))) {
		DirectX9Interface::Direct3D9->Release();
		return false;
	}

	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	ImGui::GetIO().WantCaptureMouse || ImGui::GetIO().WantTextInput || ImGui::GetIO().WantCaptureKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

	ImGui_ImplWin32_Init(OverlayWindow::Hwnd);
	ImGui_ImplDX9_Init(DirectX9Interface::pDevice);
	DirectX9Interface::Direct3D9->Release();
	return true;
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK WinProc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam) {
	if (ImGui_ImplWin32_WndProcHandler(hWnd, Message, wParam, lParam))
		return true;

	switch (Message) {
	case WM_DESTROY:
		if (DirectX9Interface::pDevice != NULL) {
			DirectX9Interface::pDevice->EndScene();
			DirectX9Interface::pDevice->Release();
		}
		if (DirectX9Interface::Direct3D9 != NULL) {
			DirectX9Interface::Direct3D9->Release();
		}
		PostQuitMessage(0);
		exit(4);
		break;
	case WM_SIZE:
		if (DirectX9Interface::pDevice != NULL && wParam != SIZE_MINIMIZED) {
			ImGui_ImplDX9_InvalidateDeviceObjects();
			DirectX9Interface::pParams.BackBufferWidth = LOWORD(lParam);
			DirectX9Interface::pParams.BackBufferHeight = HIWORD(lParam);
			HRESULT hr = DirectX9Interface::pDevice->Reset(&DirectX9Interface::pParams);
			if (hr == D3DERR_INVALIDCALL)
				IM_ASSERT(0);
			ImGui_ImplDX9_CreateDeviceObjects();
		}
		break;
	default:
		return DefWindowProc(hWnd, Message, wParam, lParam);
		break;
	}
	return 0;
}
int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	rcdriv::init();
	HANDLE handle = CreateMutex(NULL, TRUE, "TinyMan RC v1.5 lite");
	if (GetLastError() != ERROR_SUCCESS){MessageBox(0, "Cheat is already running! press END to close it", "Information", MB_OK | MB_TOPMOST | MB_ICONINFORMATION);}
	GameVars.dwProcessId = GetProcessId(GameVars.dwProcessName);
	if (!GameVars.dwProcessId)
	{
		MessageBox(0, "Open Rogue company then run the cheat", "Information", MB_TOPMOST | MB_OK | MB_ICONINFORMATION);
		removecheat();
	}
	PlaySoundA(startup, NULL, SND_ASYNC | SND_MEMORY);
	if (!rcdriv::driver_impl::driver_init(false, false)){}
	GameVars.dwProcess_Base = GetProcessBase(GameVars.dwProcessId);
	CreateThread(nullptr, NULL, reinterpret_cast<LPTHREAD_START_ROUTINE>(GameCache), nullptr, NULL, nullptr);
	CreateThread(nullptr, NULL, reinterpret_cast<LPTHREAD_START_ROUTINE>(CallAimbot), nullptr, NULL, nullptr);
	bool WindowFocus = false;
	while (WindowFocus == false)
	{
		DWORD ForegroundWindowProcessID;
		GetWindowThreadProcessId(GetForegroundWindow(), &ForegroundWindowProcessID);
		if (GetProcessId(GameVars.dwProcessName) == ForegroundWindowProcessID)
		{
			GameVars.gameHWND = GetForegroundWindow();
			RECT TempRect;
			GetWindowRect(GameVars.gameHWND, &TempRect);
			GameVars.ScreenWidth = TempRect.right - TempRect.left;
			GameVars.ScreenHeight = TempRect.bottom - TempRect.top;
			GameVars.ScreenLeft = TempRect.left;
			GameVars.ScreenRight = TempRect.right;
			GameVars.ScreenTop = TempRect.top;
			GameVars.ScreenBottom = TempRect.bottom;
			WindowFocus = true;
		}
	}
	OverlayWindow::Name = RandomString(10).c_str();
	OverlayWindow::WindowClass = {
			sizeof(WNDCLASSEX), 0, WinProc, 0, 0, nullptr, LoadIcon(nullptr, IDI_APPLICATION), LoadCursor(nullptr, IDC_ARROW), nullptr, nullptr, OverlayWindow::Name, LoadIcon(nullptr, IDI_APPLICATION)
	};

	RegisterClassEx(&OverlayWindow::WindowClass);
	if (GameVars.gameHWND) {
		static RECT TempRect = { NULL };
		static POINT TempPoint;
		GetClientRect(GameVars.gameHWND, &TempRect);
		ClientToScreen(GameVars.gameHWND, &TempPoint);
		TempRect.left = TempPoint.x;
		TempRect.top = TempPoint.y;
		GameVars.ScreenWidth = TempRect.right;
		GameVars.ScreenHeight = TempRect.bottom;
	}
	OverlayWindow::Hwnd = CreateWindowEx(NULL, OverlayWindow::Name, OverlayWindow::Name, WS_POPUP | WS_VISIBLE, GameVars.ScreenLeft, GameVars.ScreenTop, GameVars.ScreenWidth, GameVars.ScreenHeight, NULL, NULL, 0, NULL);
	DwmExtendFrameIntoClientArea(OverlayWindow::Hwnd, &DirectX9Interface::Margin);
	SetWindowLong(OverlayWindow::Hwnd, GWL_EXSTYLE, WS_EX_LAYERED | WS_EX_TRANSPARENT | WS_EX_TOOLWINDOW);
	UpdateWindow(OverlayWindow::Hwnd);
	DirectXInit();
	ImGuiIO& io = ImGui::GetIO();
	DefaultFont = io.Fonts->AddFontDefault();
	Verdana = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\Verdana.ttf", 16.0f);
	io.Fonts->Build();
	while (TRUE)
	{
		MainLoop();
	}
}
