// Do not forget that DSP addressing is done with freaking 16-bit slots (`words`)

#pragma once

namespace Debug
{

	class DspDmem : public CuiWindow
	{
		static const uint32_t DROM_START_ADDRESS = 0x1000;

		uint32_t current = 0;

	public:
		DspDmem(RECT& rect, std::string name, Cui* parent);

		virtual void OnDraw();
		virtual void OnKeyPress(char Ascii, int Vkey, bool shift, bool ctrl);
	};

}
