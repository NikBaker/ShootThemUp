// Fill out your copyright notice in the Description page of Project Settings.


#include "STUGameModeBase.h"
#include "Player/STUBaseCharacter.h"
#include "Player/STUPlayerController.h"
#include "UI/STUGameHUD.h"

ASTUGameModeBase::ASTUGameModeBase() 
{
	// Property, ������� �������� �� ����� Pawn, ���������� DefaultPawnClass.
	// Character ����������� �� Pawn, 
	// ������� �� ����� �������������� ������ property � �������������� character:
	DefaultPawnClass = ASTUBaseCharacter::StaticClass();

	// Property, ������� �������� �� ����� ����������, ���������� PlayerControllerClass.
	PlayerControllerClass = ASTUPlayerController::StaticClass();

	// �������������� HUD ����� ��� ���������
	HUDClass = ASTUGameHUD::StaticClass();

}

