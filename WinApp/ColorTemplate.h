#pragma once


struct ColorTemplate
{
	uint8_t r;
	uint8_t g;
	uint8_t b;
};

namespace color
{
	constexpr ColorTemplate black = {0x00, 0x00, 0x00};

	constexpr ColorTemplate red = {0x0F, 0x00, 0x00};
	constexpr ColorTemplate green = {0x00, 0x0F, 0x00};
	constexpr ColorTemplate blue = {0x00, 0x00, 0x0F};
}


