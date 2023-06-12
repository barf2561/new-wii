// This module is used to communicate with the JDI host. The host can be on the network, in a pluggable DLL or statically linked.

#pragma once

namespace JDI
{
	typedef Json::Value* (*CmdDelegate)(std::vector<std::string>& args);
	typedef void (*JdiReflector)();
}

#ifdef _WINDOWS
typedef Json::Value* (__cdecl* CALL_JDI)(const char* request);
typedef bool(__cdecl* CALL_JDI_NO_RETURN)(const char* request);
typedef bool(__cdecl* CALL_JDI_RETURN_INT)(const char* request, int* valueOut);
typedef bool(__cdecl* CALL_JDI_RETURN_STRING)(const char* request, char* valueOut, size_t valueSize);
typedef bool(__cdecl* CALL_JDI_RETURN_BOOL)(const char* request, bool* valueOut);

typedef void(__cdecl* JDI_ADD_NODE)(const char* filename, JDI::JdiReflector reflector);
typedef void(__cdecl* JDI_REMOVE_NODE)(const char* filename);
typedef void(__cdecl* JDI_ADD_CMD)(const char* name, JDI::CmdDelegate command);
#endif

namespace Debug
{
	class JdiClient
	{
#ifdef _WINDOWS
		CALL_JDI CallJdi = nullptr;
		CALL_JDI_NO_RETURN CallJdiNoReturn = nullptr;
		CALL_JDI_RETURN_INT CallJdiReturnInt = nullptr;
		CALL_JDI_RETURN_STRING CallJdiReturnString = nullptr;
		CALL_JDI_RETURN_BOOL CallJdiReturnBool = nullptr;

		HMODULE dll = nullptr;
#endif

	public:

#ifdef _WINDOWS
		JDI_ADD_NODE JdiAddNode = nullptr;
		JDI_REMOVE_NODE JdiRemoveNode = nullptr;
		JDI_ADD_CMD JdiAddCmd = nullptr;
#endif

		JdiClient();
		~JdiClient();

		// Generic

		std::string GetVersion();
		void ExecuteCommand(const std::string& cmdline);

		// Generic debug

		std::string DebugChannelToString(int chan);
		void QueryDebugMessages(std::list<std::pair<int, std::string>>& queue);
		void Report(const std::string& text);
		bool IsLoaded();
		bool IsCommandExists(const std::string& cmdline);

		// Gekko

		bool IsRunning();
		void GekkoRun();
		void GekkoSuspend();
		void GekkoStep();
		void GekkoSkipInstruction();

		uint32_t GetGpr(size_t n);
		uint64_t GetPs0(size_t n);
		uint64_t GetPs1(size_t n);
		uint32_t GetPc();
		uint32_t GetMsr();
		uint32_t GetCr();
		uint32_t GetFpscr();
		uint32_t GetSpr(size_t n);
		uint32_t GetSr(size_t n);
		uint32_t GetTbu();
		uint32_t GetTbl();

		void* TranslateDMmu(uint32_t address);
		void* TranslateIMmu(uint32_t address);
		uint32_t VirtualToPhysicalDMmu(uint32_t address);
		uint32_t VirtualToPhysicalIMmu(uint32_t address);

		bool GekkoTestBreakpoint(uint32_t address);
		void GekkoToggleBreakpoint(uint32_t address);
		void GekkoAddOneShotBreakpoint(uint32_t address);

		std::string GekkoDisasm(uint32_t address);
		bool GekkoIsBranch(uint32_t address, uint32_t& targetAddress);

		uint32_t AddressByName(const std::string& name);
		std::string NameByAddress(uint32_t address);

		// DSP

		bool DspIsRunning();
		void DspRun();
		void DspSuspend();
		void DspStep();

		uint16_t DspGetReg(size_t n);
		uint16_t DspGetPsr();
		uint16_t DspGetPc();
		uint64_t DspPackProd();

		void* DspTranslateDMem(uint32_t address);
		void* DspTranslateIMem(uint32_t address);

		bool DspTestBreakpoint(uint32_t address);
		void DspToggleBreakpoint(uint32_t address);
		void DspAddOneShotBreakpoint(uint32_t address);

		std::string DspDisasm(uint32_t address, size_t& instrSizeWords, bool& flowControl);
		bool DspIsCall(uint32_t address, uint32_t& targetAddress);
		bool DspIsCallOrJump(uint32_t address, uint32_t& targetAddress);

	};

	extern JdiClient * Jdi;
}
