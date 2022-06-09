#pragma once

namespace OCRA
{
union ColorValue {
	ColorValue() = default;
	ColorValue(float a_R, float a_G, float a_B, float a_A) {
		float32[0] = a_R;
		float32[1] = a_G;
		float32[2] = a_B;
		float32[3] = a_A;
	}
	ColorValue(int32_t a_R, int32_t a_G, int32_t a_B, int32_t a_A) {
		int32[0] = a_R;
		int32[1] = a_G;
		int32[2] = a_B;
		int32[3] = a_A;
	}
	ColorValue(uint32_t a_R, uint32_t a_G, uint32_t a_B, uint32_t a_A) {
		uint32[0] = a_R;
		uint32[1] = a_G;
		uint32[2] = a_B;
		uint32[3] = a_A;
	}
    float    float32[4]{ 0, 0, 0, 0 };
    int32_t  int32[4];
    uint32_t uint32[4];
};
}