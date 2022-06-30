#ifndef _DWARF2_H
#define	_DWARF2_H

struct dw_comp_unit
{
	uint32_t	cu_length;
	uint16_t	cu_version;
	uint32_t	cu_abbrev_offset;
	uint8_t		cu_pointer_size;
};

// The data in the .debug_line statement prologue looks like this
struct dw_line_header
{
	uint32_t		li_length;
	uint16_t		li_version;
	uint32_t		li_prologue_length;
	uint8_t			li_min_insn_length;
	uint8_t			li_max_ops_per_insn;
	uint8_t			li_default_is_stmt;
	int8_t			li_line_base;
	uint8_t			li_line_range;
	uint8_t			li_opcode_base;
};

// Blocks are a bunch of untyped bytes.
struct dw_block
{
	uint32_t size;
	uint8_t* data;
};

// Attributes have a name and a value
struct dw_attribute
{
	e_dw_attr name;
	e_dw_form form;
	union
	{
		char* str;
		dw_block* blk;
		uint64_t val;
		int64_t sval;
	} u;
};

// Used to return info about a parsed die.
struct die_info
{
	unsigned long length;
	unsigned long sibling;
	unsigned long low_pc;
	unsigned long high_pc;
	unsigned long stmt_list_offset;

	char* name;

	int has_stmt_list;

	unsigned short tag;
};

#endif	/* dwarf.h */