// Fill out your copyright notice in the Description page of Project Settings.


#include "STUGameModeBase.h"
#include "Player/STUBaseCharacter.h"
#include "Player/STUPlayerController.h"
#include "UI/STUGameHUD.h"

ASTUGameModeBase::ASTUGameModeBase() 
{
	// Property, которое отвечает за класс Pawn, называется DefaultPawnClass.
	// Character наследуется от Pawn, 
	// поэтому мы можем переопределить данное property с использованием character:
	DefaultPawnClass = ASTUBaseCharacter::StaticClass();

	// Property, которое отвечает за класс контроллер, называется PlayerControllerClass.
	PlayerControllerClass = ASTUPlayerController::StaticClass();

	// переопределяем HUD класс для отрисовки
	HUDClass = ASTUGameHUD::StaticClass();

}

