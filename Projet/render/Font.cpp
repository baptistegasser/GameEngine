#include "stdafx.h"
#include "Font.h"

using namespace DirectX;

Gdiplus::Font* Font::CreateFont(const wchar_t* FontName, Gdiplus::FontStyle FontStyle, float FontSize)
{
	const Gdiplus::FontFamily Family(FontName, nullptr);
	return new Gdiplus::Font{ &Family, FontSize, FontStyle, Gdiplus::UnitPixel };
}

Font::Font(const wchar_t* FontName, Gdiplus::FontStyle FontStyle, float FontSize, Gdiplus::Color Color)
	: Font(
		CreateFont(FontName, FontStyle, FontSize),
		new Gdiplus::SolidBrush(Color))
{}

Font::Font(float FontSize, Gdiplus::Color Color)
	: Font(
		CreateFont(L"Arial Rounded MT Bold", Gdiplus::FontStyleBold, FontSize),
		new Gdiplus::SolidBrush(Color))
{}

Font::Font(Gdiplus::Font* Font, Gdiplus::Brush* Brush)
	: Brush{ Brush }
	, FontType{ Font }
{}

