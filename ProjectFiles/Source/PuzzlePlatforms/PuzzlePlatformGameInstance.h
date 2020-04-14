// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "OnlineSubsystem.h"
#include "MenuSystem/MenuInterface.h" // Add The Folder Prefix
#include "Interfaces/OnlineSessionInterface.h"
#include "PuzzlePlatformGameInstance.generated.h"

/**
 * (Now This is Child of MenuInterface) 
 */
UCLASS()
class PUZZLEPLATFORMS_API UPuzzlePlatformGameInstance : public UGameInstance, public IMenuInterface
{	
	GENERATED_BODY()
	
public:
	UPuzzlePlatformGameInstance(const FObjectInitializer& ObjectInitializer);

	virtual void Init();

	UFUNCTION(Exec)
	void Host(FString ServerName) override;

	UFUNCTION(Exec)
	void Join(uint32 Index) override;

	UFUNCTION(BlueprintCallable)
	void LoadMenu();

	UFUNCTION(BlueprintCallable)
	void InGameLoadMenu();

	virtual void LoadMainMenu() override;

	void RefreshServerList() override;
	
private:
	// For Construct The Widget From File
	TSubclassOf<class UUserWidget> MenuClass;

	TSubclassOf<class UUserWidget> InGameMenuClass;
	
	// Local Variable To Create Widget 
	class UMainMenu* Menu;

	IOnlineSessionPtr SessionInterface;

	TSharedPtr<class FOnlineSessionSearch> SessionSearch;

	void OnCreateSessionComplete(FName SessionName, bool Success);

	void OnDestroySessionComplete(FName SessionName, bool Success);

	void CreateSession();

	void OnFindSessionComplete(bool Success);

	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);

	FString DesiredServerName;
};