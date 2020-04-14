// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenu.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "Components/EditableTextBox.h"
#include "Components/PanelWidget.h"
#include "Components/TextBlock.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/Engine.h"
#include "ServerRow.h"

UMainMenu::UMainMenu(const FObjectInitializer& ObjectInitializer)
{
	ConstructorHelpers::FClassFinder<UUserWidget> ServerRowBPClass(TEXT("/Game/MenuSystem/WBP_ServerRow"));
	if (!ensure(ServerRowBPClass.Class != nullptr)) return;
	ServerRowClass = ServerRowBPClass.Class;
}

bool UMainMenu::Initialize()
{
	bool Success = Super::Initialize(); // If This Method Fails
	if(!Success) return false;

	// Main Menu
	if (!ensure(HostButton != nullptr)) return false;
	HostButton->OnClicked.AddDynamic(this, &UMainMenu::OpenHostMenu);
	
	if (!ensure(JoinButton != nullptr)) return false;
	JoinButton->OnClicked.AddDynamic(this, &UMainMenu::OpenJoinMenu);

	// Open Join Menu
	if (!ensure(CancelJoinMenuButton != nullptr)) return false;
	CancelJoinMenuButton->OnClicked.AddDynamic(this, &UMainMenu::OpenMainMenu);

	if (!ensure(JoinIPButton != nullptr)) return false;
	JoinIPButton->OnClicked.AddDynamic(this, &UMainMenu::JoinServer);

	// Main Menu Exit Button
	if (!ensure(ExitButton != nullptr)) return false;
	ExitButton->OnClicked.AddDynamic(this, &UMainMenu::ExitGame);
	
	//Host Menu
	if (!ensure(CancelHostButton != nullptr)) return false;
	CancelHostButton ->OnClicked.AddDynamic(this, &UMainMenu::OpenMainMenu);

	if (!ensure(Host != nullptr)) return false;
	Host->OnClicked.AddDynamic(this, &UMainMenu::HostServer);

	return true;
}

void UMainMenu::HostServer()
{
	FString ServerName = ServerHostName->Text.ToString();
	if (MenuInterface != nullptr)
	{
		MenuInterface->Host(ServerName);
	}
}

void UMainMenu::SetServerList(TArray<FServerData> ServerNames)
{
	ServerList->ClearChildren();

	uint32 i = 0;
	for (const FServerData& ServerData : ServerNames)
	{
		UServerRow* Row = CreateWidget<UServerRow>(this, ServerRowClass);
		if (!ensure(Row != nullptr)) return;

		Row->ServerName->SetText(FText::FromString(ServerData.Name));
		Row->HostUser->SetText(FText::FromString(ServerData.HostUserName));
		FString FractionText = FString::Printf(TEXT("[%d/%d]"), ServerData.CurrentPlayers, ServerData.MaxPlayers);
		Row->ConnectionFraction->SetText(FText::FromString(FractionText));

		Row->Setup(this, i); // Its Just Passing The Index Value And WBP
		++i;

		ServerList->AddChild(Row); //ServerList Is ScrollBox
	}
}

void UMainMenu::SelectIndex(uint32 Index)
{
	SelectedIndex = Index;
	UpdateChildren();
}

void UMainMenu::UpdateChildren() //  Comparing All Children with SelectedIndex And Assigning Them True Or False 
{
	for (int32 i = 0; i < ServerList->GetChildrenCount(); ++i)
	{
		auto Row = Cast<UServerRow>(ServerList->GetChildAt(i));
		if (Row != nullptr)
		{
			Row->Selected = (SelectedIndex.IsSet() && SelectedIndex.GetValue() == i);
		}
	}
}

void UMainMenu::JoinServer()
{
	if (SelectedIndex.IsSet() && MenuInterface != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Selected Index %d"), SelectedIndex.GetValue())
		MenuInterface->Join(SelectedIndex.GetValue());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Selected Index Not Set"))
	}
}

void UMainMenu::OpenHostMenu()
{
	if (!ensure(MenuSwitcher != nullptr)) return;
	if (!ensure(HostMenu != nullptr)) return;
	MenuSwitcher->SetActiveWidget(HostMenu);
}

void UMainMenu::OpenJoinMenu()
{
	if (!ensure(MenuSwitcher != nullptr)) return;
	if (!ensure(JoinMenu != nullptr)) return;
	MenuSwitcher->SetActiveWidget(JoinMenu);
	if (MenuInterface != nullptr)
	{
		MenuInterface->RefreshServerList();
	}
}

void UMainMenu::OpenMainMenu()
{
	if (!ensure(MenuSwitcher != nullptr)) return;
	if (!ensure(MainMenu != nullptr)) return;
	MenuSwitcher->SetActiveWidget(MainMenu);
}

void UMainMenu::ExitGame()
{
	UWorld* World = GetWorld();
	if (!ensure(World != nullptr)) return;
	APlayerController* PlayerController = World->GetFirstPlayerController();
	if (!ensure(PlayerController != nullptr)) return;

	PlayerController->ConsoleCommand("quit");
}
