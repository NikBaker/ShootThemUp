// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/STUGameHUD.h"
#include "Engine/Canvas.h"

void ASTUGameHUD::DrawHUD()
{
	// Вызываем родительскую функцию
	Super::DrawHUD();

	DrowCrossHair();
}

void ASTUGameHUD::DrowCrossHair()
{
	// Canvas - Объект, в котором происходит отрисовка всех графических примитивов
	int32 SizeX = Canvas->SizeX; // SizeX - размер горизонтали экрана
	int32 SizeY = Canvas->SizeY; // SizeY - размер вертикали экрана
	// Вычисляем центр экрана
	const TInterval<float> Center(SizeX * 0.5f, SizeY * 0.5f);
	const float HalfLineSize = 10.0f;
	const float LineThickness = 2.0f;
	const FLinearColor LineColor = FLinearColor::Green;
	// Рисуем линию на экране
	// Первый параметр - координата X начальной точки на экране
	// Второй параметр - координата Y начальной точки на экране
	// Третий параметр - координата X конечной точки на экране
	// Четвертый параметр - координата Y конечной точки на экране
	// Пятый параметр - цвет линии
	// Шестой параметр - толщина линии
	//Горизонтальная линия
	DrawLine(Center.Min - HalfLineSize, Center.Max, Center.Min + HalfLineSize, Center.Max, LineColor, LineThickness);
	//Вертикальная линия
	DrawLine(Center.Min, Center.Max - HalfLineSize, Center.Min, Center.Max + HalfLineSize, LineColor, LineThickness);
}