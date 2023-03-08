// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectGameInstance.h"
#include "Kismet/KismetStringLibrary.h"
#include "Kismet/GameplayStatics.h"

#include "GraphManager.h"
#include "LevelManager.h"

#include "XmlFile.h"
#include <Runtime/XmlParser/Public/XmlFile.h>


void UProjectGameInstance::Init()
{
	Super::Init();

	LoadPlayerModel(TEXT("PlayerModel.xml"));

	GraphManager = NewObject<UGraphManager>(this);

	LevelManager = NewObject<ULevelManager>(this);
	LevelManager->Initialise();
}

/* Set the player model values to those stored in Xml */
void UProjectGameInstance::LoadPlayerModel(const FString& PlayerModelFile)
{
	FXmlFile* xmlFile = new FXmlFile(FPaths::ProjectConfigDir() + PlayerModelFile);
	if (!xmlFile->IsValid()) return;

	const FXmlNode* root = xmlFile->GetRootNode();
	const FXmlNode* element = root->GetFirstChildNode();

	PlayerModel.Killer		= UKismetStringLibrary::Conv_StringToInt(element->GetAttribute("Killer"));
	PlayerModel.Achiever	= UKismetStringLibrary::Conv_StringToInt(element->GetAttribute("Achiever"));
	PlayerModel.Socialiser	= UKismetStringLibrary::Conv_StringToInt(element->GetAttribute("Socialiser"));
	PlayerModel.Explorer	= UKismetStringLibrary::Conv_StringToInt(element->GetAttribute("Explorer"));
}

FPlayerModel UProjectGameInstance::GetPlayerModel()
{
	return PlayerModel;
}

void UProjectGameInstance::IncreaseKillerType()
{
	PlayerModel.Killer += 5;
	PlayerModel.Explorer -= 5;
}

void UProjectGameInstance::IncreaseAchieverType()
{
	PlayerModel.Achiever += 5;
	PlayerModel.Socialiser -= 5;
}

void UProjectGameInstance::IncreaseSocialiserType()
{
	PlayerModel.Socialiser += 5;
	PlayerModel.Achiever -= 5;
}

void UProjectGameInstance::IncreaseExplorerType()
{
	PlayerModel.Explorer += 5;
	PlayerModel.Killer -= 5;
}

void UProjectGameInstance::NextLevel()
{
	/* 
	*  Call GenerateLevel passing the new level value
	*  Increase the level number
	*/

	LevelManager->GenerateLevel(++LevelNumber);
}
