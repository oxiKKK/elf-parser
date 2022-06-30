#include "inc.h"

struct DebugLineHeader
{
	uint32_t length;
	uint16_t version;
	uint32_t header_length;
	uint8_t min_instruction_length;
	uint8_t default_is_stmt;
	int8_t line_base;
	uint8_t line_range;
	uint8_t opcode_base;
	uint8_t std_opcode_lengths[12];
};

#define SAFE_MEM_READ(type, start, offvar, size) *reinterpret_cast<type*>((uint8_t*)(start) + (offvar)); (offvar) += (size)

bool CDwarfParser::parse(const CElfParser& elf_parser)
{
	return true;
	const auto& elf_hdr = elf_parser.get_elf_hdr();

	const auto p_sc_dbg_line = elf_hdr.get_sc_hdr_by_name(".debug_info");

	if (!p_sc_dbg_line)
	{
		CPrintf::err("Couldn't find .debug_info  section\n");
		return false;
	}

	const auto p_sc_dbg_abbrev = elf_hdr.get_sc_hdr_by_name(".debug_abbrev");

	if (!p_sc_dbg_abbrev)
	{
		CPrintf::err("Couldn't find .debug_abbrev  section\n");
		return false;
	}

	dw_comp_unit cu_hdr;

	{
		uint32_t* start = CElfUtils::file_off<uint32_t*>(elf_parser.get_file_elf(), p_sc_dbg_line->get_offset());

		uint32_t off = 0;

		cu_hdr.cu_length = SAFE_MEM_READ(uint32_t, start, off, 4);
		cu_hdr.cu_version = SAFE_MEM_READ(uint16_t, start, off, 2);
		cu_hdr.cu_abbrev_offset = SAFE_MEM_READ(uint32_t, start, off, 4);
		cu_hdr.cu_pointer_size = SAFE_MEM_READ(uint8_t, start, off, 1);
	}

	{
		uint32_t* start = CElfUtils::file_off<uint32_t*>(elf_parser.get_file_elf(), 
														 p_sc_dbg_abbrev->get_offset());

		uint32_t* abbrev = reinterpret_cast<uint32_t*>((uint8_t*)start + cu_hdr.cu_abbrev_offset);

		uint32_t off = 0;

	}

	return true;
}
