// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MenuWidget.h"
#include "Misc/Optional.h"
#include "MainMenu.generated.h"

USTRUCT()
struct FServerData 
{
	GENERATED_BODY()

	FString Name;
	uint16 CurrentPlayers; // uint16 stores Smaller Amount of Data
	uint16 MaxPlayers;
	FString HostUserName;
};

/**
 * 
 */
UCLASS()
class PUZZLEPLATFORMS_API UMainMenu : public UMenuWidget
{
	GENERATED_BODY()
public:
	UMainMenu(const FObjectInitializer& ObjectInitializer);

	void SetServerList(TArray<FServerData> ServerNames);

	void SelectIndex(uint32 Index);

protected:
	virtual bool Initialize() override; // Method Of UUserWidget

private:
	TSubclassOf<class UUserWidget> ServerRowClass;

	// Assigned Button By Exact Name
	UPROPERTY(meta = (BindWidget))
	class UButton* HostButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* JoinButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* JoinIPButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* CancelJoinMenuButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* CancelHostButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* Host;

	UPROPERTY(meta = (BindWidget))
	class UButton* ExitButton;

	// Assigning UE4 Spacial Property
	UPROPERTY(meta = (BindWidget))
	class UWidgetSwitcher* MenuSwitcher;

	UPROPERTY(meta = (BindWidget))
	class UEditableTextBox* ServerHostName;
	
	// Widget Designed MainMenu & JoinMenu & HostMenu
	UPROPERTY(meta = (BindWidget))
	class UWidget* HostMenu; 

	UPROPERTY(meta = (BindWidget))
	class UWidget* JoinMenu;

	UPROPERTY(meta = (BindWidget))
	class UWidget* MainMenu;

	UPROPERTY(meta = (BindWidget))
	class UPanelWidget* ServerList; // ScrollBox

	// Dynamic Delegates
	UFUNCTION()
	void HostServer();

	UFUNCTION()
	void OpenJoinMenu();

	UFUNCTION()
	void OpenHostMenu();

	UFUNCTION()
	void OpenMainMenu();

	UFUNCTION()
	void ExitGame();

	UFUNCTION()
	void JoinServer();

	TOptional<uint32> SelectedIndex;

	void UpdateChildren();
};