#pragma once
#include <gdiplus.h>
#pragma comment(lib, "gdiplus.lib")

struct Font {
	std::unique_ptr<Gdiplus::Brush> Brush;
	std::unique_ptr<Gdiplus::Font> FontType;

	[[nodiscard]] static Gdiplus::Font* CreateFont(const wchar_t* FontName, Gdiplus::FontStyle FontStyle, float FontSize);

	Font(const wchar_t* FontName, Gdiplus::FontStyle FontStyle, float FontSize, Gdiplus::Color Color);
	Font(float FontSize, Gdiplus::Color Color);
	Font(Gdiplus::Font* Font, Gdiplus::Brush* Brush);
	~Font() = default;
};