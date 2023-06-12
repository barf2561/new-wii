#include "../pch.h"

namespace Gekko
{

	void Jitc::FallbackStub(DecoderInfo* info, CodeSegment* seg)
	{
		// Call ExecuteInterpeterFallback

		seg->Write(IntelAssembler::mov<64>(IntelCore::Param::rax, IntelCore::Param::imm64, 0, (int64_t)Jitc::ExecuteInterpeterFallback));
		seg->Write(IntelAssembler::call<64>(IntelCore::Param::rax));

		// If an exception occurs during the execution of an instruction - abort the segment.

		// test   al, al
		// je     EpilogSize <label>
		// ...    <EPILOG>
		//<label>:

		seg->Write(IntelAssembler::test<64>(IntelCore::Param::al, IntelCore::Param::al));
		seg->Write(IntelAssembler::je<64>(IntelCore::Param::rel8, EpilogSize()));
		Epilog(seg);
	}

}
