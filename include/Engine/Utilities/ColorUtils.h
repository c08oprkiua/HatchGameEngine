#ifndef ENGINE_UTILITIES_COLORUTILS_H
#define ENGINE_UTILITIES_COLORUTILS_H

#include <Engine/Includes/Standard.h>

class ColorUtils {
public:
	static Uint32 ToRGB(int r, int g, int b);
	static Uint32 ToRGB(int r, int g, int b, int a);
	static Uint32 ToRGB(float r, float g, float b);
	static Uint32 ToRGB(float r, float g, float b, float a);
	static Uint32 ToRGB(float* r);
	static Uint32 ToRGBA(float* r);
	static void SeparateRGB(Uint32 color, float* dest);
	static void Separate(Uint32 color, float* dest);
	static void SeparateRGB(Uint32 color, Uint8* dest);
	static void Separate(Uint32 color, Uint8* dest);
	static void SeparateRGB(float* color, Uint8* dest);
	static void Separate(float* color, Uint8* dest);
	inline static Uint32 Blend(Uint32 color1, Uint32 color2, int percent){
		Uint32 rb = color1 & 0xFF00FFU;
		Uint32 g = color1 & 0x00FF00U;
		rb += (((color2 & 0xFF00FFU) - rb) * percent) >> 8;
		g += (((color2 & 0x00FF00U) - g) * percent) >> 8;
		return (rb & 0xFF00FFU) | (g & 0x00FF00U);
	}
	static Uint32 Tint(Uint32 color, Uint32 colorMult);
	inline static Uint32 Tint(Uint32 color, Uint32 colorMult, Uint16 percentage){
		return Blend(color, Tint(color, colorMult), percentage);
	}
	static Uint32 Multiply(Uint32 color, Uint32 colorMult);
	static void ConvertFromARGBtoABGR(Uint32* argb, int count);
	static void ConvertFromABGRtoARGB(Uint32* argb, int count);
	static int NearestColor(Uint8 r, Uint8 g, Uint8 b, Uint32* palette, unsigned numColors);
};

#endif /* ENGINE_UTILITIES_COLORUTILS_H */
