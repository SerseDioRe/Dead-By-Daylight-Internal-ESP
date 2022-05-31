#pragma once
#include "Engine.h"

int Width;
int Height;

/*
	Splitgate Internal

	Credit to guttir14 https://github.com/guttir14/CheatIt I pasted his Engine.cpp/h and Util. Shits bussin thanks
*/

void PostRender(UGameViewportClient* UGameViewportClient, Canvas* canvas)
{
	do {
		UWorld* World = *(UWorld**)(WRLD);
		if (!World) break;

		UGameInstance* OwningGameInstance = World->OwningGameInstance;
		if (!OwningGameInstance) break;

		ULevel* PersistentLevel = World->PersistentLevel;
		if (!PersistentLevel) break;

		AGameStateBase* GameState = World->GameState;
		if (!GameState) break;

		TArray<AActor*> Actors = PersistentLevel->Actors;

		TArray<UPlayer*> LocalPlayers = OwningGameInstance->LocalPlayers;

		TArray<APlayerState*> PlayerArray = GameState->PlayerArray;

		UPlayer* LocalPlayer = LocalPlayers[0];
		if (!LocalPlayer) break;

		APlayerController* PlayerController = LocalPlayer->PlayerController;
		if (!PlayerController) break;

		PlayerController->GetViewportSize(Width, Height);

		APawn* MyPlayer = PlayerController->AcknowledgedPawn;
		if (!MyPlayer) break;

		APlayerState* PlayerState = MyPlayer->PlayerState;
		if (!PlayerState) break;

		for (auto i = 0; i < PlayerArray.Num(); i++) {

			if (!PlayerArray.IsValidIndex(i)) break;

			AActor* Actor = Actors[i];

			if (!Actor || Actor == MyPlayer) continue;

			APlayerState* CurrentPlayer = PlayerArray[i];

			APawn* Pawn = CurrentPlayer->PawnPrivate;

			if (Pawn == MyPlayer) continue;

			USkeletalMeshComponent* Mesh = Pawn->Mesh;
			if (!Mesh) continue;

			APlayerState* State = Pawn->PlayerState;

			FVector2D headPos2D = GetBone(Mesh, GetBoneIndex(Mesh, BoneFNames::Head), PlayerController);
			FVector2D neckPos2D = GetBone(Mesh, GetBoneIndex(Mesh, BoneFNames::Neck), PlayerController);
			FVector2D torsoAPos2D = GetBone(Mesh, GetBoneIndex(Mesh, BoneFNames::TorsoA), PlayerController);
			FVector2D hipMasterPos2D = GetBone(Mesh, GetBoneIndex(Mesh, BoneFNames::HipMaster), PlayerController);
			FVector2D hipRTPos2D = GetBone(Mesh, GetBoneIndex(Mesh, BoneFNames::HipRT), PlayerController);
			FVector2D kneeRTPos2D = GetBone(Mesh, GetBoneIndex(Mesh, BoneFNames::KneeRT), PlayerController);
			FVector2D footRTPos2D = GetBone(Mesh, GetBoneIndex(Mesh, BoneFNames::FootRT), PlayerController);
			FVector2D hipLTPos2D = GetBone(Mesh, GetBoneIndex(Mesh, BoneFNames::HipLT), PlayerController);
			FVector2D kneeLTPos2D = GetBone(Mesh, GetBoneIndex(Mesh, BoneFNames::KneeLT), PlayerController);
			FVector2D footLTPos2D = GetBone(Mesh, GetBoneIndex(Mesh, BoneFNames::FootLT), PlayerController);
			FVector2D clavicleLTPos2D = GetBone(Mesh, GetBoneIndex(Mesh, BoneFNames::ClavicleLT), PlayerController);
			FVector2D shoulderLTPos2D = GetBone(Mesh, GetBoneIndex(Mesh, BoneFNames::ShoulderLT), PlayerController);
			FVector2D elbowLTPos2D = GetBone(Mesh, GetBoneIndex(Mesh, BoneFNames::ElbowLT), PlayerController);
			FVector2D HandLTPos2D = GetBone(Mesh, GetBoneIndex(Mesh, BoneFNames::HandLT), PlayerController);
			FVector2D clavicleRTPos2D = GetBone(Mesh, GetBoneIndex(Mesh, BoneFNames::ClavicleRT), PlayerController);
			FVector2D shoulderRTPos2D = GetBone(Mesh, GetBoneIndex(Mesh, BoneFNames::ShoulderRT), PlayerController);
			FVector2D elbowRTPos2D = GetBone(Mesh, GetBoneIndex(Mesh, BoneFNames::ElbowRT), PlayerController);
			FVector2D HandRTPos2D = GetBone(Mesh, GetBoneIndex(Mesh, BoneFNames::HandRT), PlayerController);

			if (!headPos2D.X && !headPos2D.Y ||
				!neckPos2D.X && !neckPos2D.Y ||
				!torsoAPos2D.X && !torsoAPos2D.Y ||
				!hipMasterPos2D.X && !hipMasterPos2D.Y ||
				!hipRTPos2D.X && !hipRTPos2D.Y ||
				!kneeRTPos2D.X && !kneeRTPos2D.Y ||
				!footRTPos2D.X && !footRTPos2D.Y ||
				!hipLTPos2D.X && !hipLTPos2D.Y ||
				!kneeLTPos2D.X && !kneeLTPos2D.Y ||
				!footLTPos2D.X && !footLTPos2D.Y ||
				!clavicleLTPos2D.X && !clavicleLTPos2D.Y ||
				!shoulderLTPos2D.X && !shoulderLTPos2D.Y ||
				!elbowLTPos2D.X && !elbowLTPos2D.Y ||
				!HandLTPos2D.X && !HandLTPos2D.Y ||
				!clavicleRTPos2D.X && !clavicleRTPos2D.Y ||
				!shoulderRTPos2D.X && !shoulderRTPos2D.Y ||
				!elbowRTPos2D.X && !elbowRTPos2D.Y ||
				!HandRTPos2D.X && !HandRTPos2D.Y
				) continue;

			FLinearColor Color = { 0.f, 1.f, 0.f, 1.f };

			if (MyPlayer->GetPlayerTeam() != Pawn->GetPlayerTeam())
			{
				if (PlayerController->LineOfSightTo(Pawn))
					Color = { 1.f, 0.f, 0.f, 1.f };
				else
					Color = { 1.f, 1.f, 0.f, 1.f };
			}

			// Draw Player Name
			canvas->K2_DrawText(State->PlayerNamePrivate, { headPos2D.X, headPos2D.Y - 20.0F }, FVector2D{ 1.0f, 1.0f }, Color, 1.0f, FLinearColor{ 0, 0, 0, 0 }, FVector2D{ 0, 0 }, true, false, false, FLinearColor{ 0, 0, 0, 0 });

			// Draw Skeleton
			canvas->K2_DrawLine(headPos2D, neckPos2D, 0.8F, Color);
			canvas->K2_DrawLine(neckPos2D, torsoAPos2D, 0.8F, Color);
			canvas->K2_DrawLine(torsoAPos2D, hipMasterPos2D, 0.8F, Color);
			canvas->K2_DrawLine(hipMasterPos2D, hipRTPos2D, 0.8F, Color);
			canvas->K2_DrawLine(hipRTPos2D, kneeRTPos2D, 0.8F, Color);
			canvas->K2_DrawLine(kneeRTPos2D, footRTPos2D, 0.8F, Color);
			canvas->K2_DrawLine(hipMasterPos2D, hipLTPos2D, 0.8F, Color);
			canvas->K2_DrawLine(hipLTPos2D, kneeLTPos2D, 0.8F, Color);
			canvas->K2_DrawLine(kneeLTPos2D, footLTPos2D, 0.8F, Color);
			canvas->K2_DrawLine(neckPos2D, clavicleLTPos2D, 0.8F, Color);
			canvas->K2_DrawLine(clavicleLTPos2D, shoulderLTPos2D, 0.8F, Color);
			canvas->K2_DrawLine(shoulderLTPos2D, elbowLTPos2D, 0.8F, Color);
			canvas->K2_DrawLine(elbowLTPos2D, HandLTPos2D, 0.8F, Color);
			canvas->K2_DrawLine(neckPos2D, clavicleRTPos2D, 0.8F, Color);
			canvas->K2_DrawLine(clavicleRTPos2D, shoulderRTPos2D, 0.8F, Color);
			canvas->K2_DrawLine(shoulderRTPos2D, elbowRTPos2D, 0.8F, Color);
			canvas->K2_DrawLine(elbowRTPos2D, HandRTPos2D, 0.8F, Color);

			// Draw Snaplines
			//canvas->K2_DrawLine(rootPos2D, FVector2D{ (float)Width / 2.f , (float)Height }, 0.8F, Color);
		}

		for (auto i = 0; i < Actors.Num(); i++) {

			if (!Actors.IsValidIndex(i)) break;

			AActor* Actor = Actors[i];

			if (!Actor || Actor == MyPlayer) continue;

			// ESP GENERATOR
			if(Actor->IsA(AGeneratorClass))
			{
				AGenerator* Generator = (AGenerator*)Actor;
				if(!Generator->IsRepaired)
				{
					FVector WorldLocation = Actor->RootComponent->RelativeLocation;

					FVector2D ScreenLocation;

					PlayerController->ProjectWorldLocationToScreen(WorldLocation, ScreenLocation);

					if (!ScreenLocation.X && !ScreenLocation.Y) continue;

					FLinearColor Color = { 0.032f, 0.78f, 0.70f, 1.f };

					std::string generatorName = "Generator";
					std::string generatorDist = std::to_string(static_cast<int>(MyPlayer->GetDistanceTo(Actor) / 100.0F));
					std::string generator = generatorName + "[" + generatorDist + "m]";
					std::wstring generatorW(generator.begin(), generator.end());

					canvas->K2_DrawText(FString(generatorW.c_str()), ScreenLocation, FVector2D{ 1.0f, 1.0f }, Color, 1.0f, FLinearColor{ 0, 0, 0, 0 }, FVector2D{ 0, 0 }, true, false, false, FLinearColor{ 0, 0, 0, 0 });
				}
			}

			// ESP ESCAPE DOOR
			if (Actor->IsA(AEscapeDoorClass))
			{
				FVector WorldLocation = Actor->RootComponent->RelativeLocation;

				FVector2D ScreenLocation;

				PlayerController->ProjectWorldLocationToScreen(WorldLocation, ScreenLocation);

				if (!ScreenLocation.X && !ScreenLocation.Y) continue;

				FLinearColor Color = { 0.046f, 0.139f, 0.087f, 1.f };

				std::string escapeDoorName = "Escape Door";
				std::string escapeDoorDist = std::to_string(static_cast<int>(MyPlayer->GetDistanceTo(Actor) / 100.0F));
				std::string escapeDoor = escapeDoorName + "[" + escapeDoorDist + "m]";
				std::wstring escapeDoorW(escapeDoor.begin(), escapeDoor.end());

				canvas->K2_DrawText(FString(escapeDoorW.c_str()), ScreenLocation, FVector2D{ 1.0f, 1.0f }, Color, 1.0f, FLinearColor{ 0, 0, 0, 0 }, FVector2D{ 0, 0 }, true, false, false, FLinearColor{ 0, 0, 0, 0 });
			}

			// ESP TOTEM
			if (Actor->IsA(ATotemClass))
			{
				ATotem* Totem = (ATotem*)Actor;
				if(Totem->_totemState != 0)
				{
					FVector WorldLocation = Actor->RootComponent->RelativeLocation;

					FVector2D ScreenLocation;

					PlayerController->ProjectWorldLocationToScreen(WorldLocation, ScreenLocation);

					if (!ScreenLocation.X && !ScreenLocation.Y) continue;

					FLinearColor Color = { 0.128f, 0.128f, 0.128f, 1.f };

					std::string totemName = "Totem";
					switch (Totem->_totemState)
					{
					case 2:
						totemName += "_Hex";
						break;
					case 3:
						totemName += "_Boon";
						break;
					default:
						break;
					}

					std::string totemDist = std::to_string(static_cast<int>(MyPlayer->GetDistanceTo(Actor) / 100.0F));
					std::string totem = totemName + "[" + totemDist + "m]";
					std::wstring totemW(totem.begin(), totem.end());

					canvas->K2_DrawText(FString(totemW.c_str()), ScreenLocation, FVector2D{ 1.0f, 1.0f }, Color, 1.0f, FLinearColor{ 0, 0, 0, 0 }, FVector2D{ 0, 0 }, true, false, false, FLinearColor{ 0, 0, 0, 0 });
				}
			}

			// ESP CHEST
			if (Actor->IsA(ASearchableClass))
			{
				FVector WorldLocation = Actor->RootComponent->RelativeLocation;

				FVector2D ScreenLocation;

				PlayerController->ProjectWorldLocationToScreen(WorldLocation, ScreenLocation);

				if (!ScreenLocation.X && !ScreenLocation.Y) continue;

				FLinearColor Color = { 0.165f, 0.042f, 0.042f, 1.f };

				std::string chestName = "Chest";
				std::string chestDist = std::to_string(static_cast<int>(MyPlayer->GetDistanceTo(Actor) / 100.0F));
				std::string chest = chestName + "[" + chestDist + "m]";
				std::wstring chestW(chest.begin(), chest.end());

				canvas->K2_DrawText(FString(chestW.c_str()), ScreenLocation, FVector2D{ 1.0f, 1.0f }, Color, 1.0f, FLinearColor{ 0, 0, 0, 0 }, FVector2D{ 0, 0 }, true, false, false, FLinearColor{ 0, 0, 0, 0 });
			}

			// ESP MEATHOOK
			if (Actor->IsA(HookClass))
			{
				FVector WorldLocation = Actor->RootComponent->RelativeLocation;

				FVector2D ScreenLocation;

				PlayerController->ProjectWorldLocationToScreen(WorldLocation, ScreenLocation);

				if (!ScreenLocation.X && !ScreenLocation.Y) continue;

				FLinearColor Color = { 1.0f, 0.0f, 0.0f, 1.f };

				std::string hookName = "Hook";
				std::string hookDist = std::to_string(static_cast<int>(MyPlayer->GetDistanceTo(Actor) / 100.0F));
				std::string hook = hookName + "[" + hookDist + "m]";
				std::wstring hookW(hook.begin(), hook.end());

				canvas->K2_DrawText(FString(hookW.c_str()), ScreenLocation, FVector2D{ 1.0f, 1.0f }, Color, 1.0f, FLinearColor{ 0, 0, 0, 0 }, FVector2D{ 0, 0 }, true, false, false, FLinearColor{ 0, 0, 0, 0 });
			}
		}
	} while (false);

	OPostRender(UGameViewportClient, canvas);
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	if (ul_reason_for_call != DLL_PROCESS_ATTACH) return FALSE;

	if (!EngineInit()) return FALSE; 
	
	UWorld* World = *(UWorld**)(WRLD);
	if (!World) return FALSE;

	UGameInstance* OwningGameInstance = World->OwningGameInstance;
	if (!OwningGameInstance) return FALSE;

	TArray<UPlayer*>LocalPlayers = OwningGameInstance->LocalPlayers;

	UPlayer* LocalPlayer = LocalPlayers[0];
	if (!LocalPlayer) return FALSE;

	UGameViewportClient* ViewPortClient = LocalPlayer->ViewportClient;
	if (!ViewPortClient) return FALSE;

	void** ViewPortClientVTable = ViewPortClient->VFTable;
	if (!ViewPortClientVTable) return FALSE;

	DWORD protecc;
	VirtualProtect(&ViewPortClientVTable[102], 8, PAGE_EXECUTE_READWRITE, &protecc);
	OPostRender = reinterpret_cast<decltype(OPostRender)>(ViewPortClientVTable[102]);
	ViewPortClientVTable[102] = &PostRender;
	VirtualProtect(&ViewPortClientVTable[102], 8, protecc, 0);

    return TRUE;
}

