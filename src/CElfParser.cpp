#include "inc.h"

const CStringPair<uint32_t> CElfDictionary::m_elf_hdr_type[] =
{
	{ ET_NONE, "An unknown type" },
	{ ET_REL, "A relocatable file" },
	{ ET_EXEC, "An executable file" },
	{ ET_DYN , "A shared object " },
	{ ET_CORE , "A core file " },
};

const CStringPair<uint16_t> CElfDictionary::m_elf_hdr_machine[] =
{
	{ EM_NONE, "An unknown machine" },
	{ EM_M32 , "AT&T WE 32100" },
	{ EM_SPARC, "Sun Microsystems SPARC" },
	{ EM_386 , "Intel 80386" },
	{ EM_68K , "Motorola 68000" },
	{ EM_88K , "Motorola 88000" },
	{ EM_860 , "Intel 80860" },
	{ EM_MIPS, "MIPS RS3000 (big-endian only)" },
	{ EM_PARISC, "HP/PA" },
	{ EM_SPARC32PLUS, "SPARC with enhanced instruction set" },
	{ EM_PPC , "PowerPC" },
	{ EM_PPC64, "PowerPC 64-bit" },
	{ EM_S390, "IBM S/390" },
	{ EM_SH  , "Renesas SuperH" },
	{ EM_SPARCV9, "SPARC v9 64-bit" },
	{ EM_IA_64, "Intel Itanium" },
	{ EM_X86_64, "AMD x86-64" },
};

const CStringPair<uint32_t> CElfDictionary::m_elf_hdr_version[] =
{
	{ EV_NONE, "Invalid version" },
	{ EV_CURRENT, "Current version" },
};

// Figure 2-2: Segment Types
const CStringPair<uint32_t> CElfDictionary::m_elf_phdr_type[] =
{
	{ PT_NULL, "PT_NULL" },
	{ PT_LOAD, "PT_LOAD" },
	{ PT_DYNAMIC, "PT_DYNAMIC" },
	{ PT_INTERP, "PT_INTERP" },
	{ PT_NOTE, "PT_NOTE" },
	{ PT_SHLIB, "PT_SHLIB" },
	{ PT_PHDR, "PT_PHDR" },
	{ PT_LOPROC, "PT_LOPROC" },
	{ PT_HIPROC, "PT_HIPROC" },
	{ PT_GNU_STACK, "PT_GNU_STACK" },
};

const CStringPair<uint32_t> CElfDictionary::m_elf_phdr_flags[] =
{
	{ PF_X, "An executable segment" },
	{ PF_W, "A writable segment" },
	{ PF_R, "A readable segment" },
};

const CStringPair<uint32_t> CElfDictionary::m_elf_shdr_type[] =
{
	{ SHT_NULL, "SHT_NULL" },
	{ SHT_PROGBITS, "SHT_PROGBITS" },
	{ SHT_SYMTAB, "SHT_SYMTAB" },
	{ SHT_STRTAB, "SHT_STRTAB" },
	{ SHT_RELA, "SHT_RELA" },
	{ SHT_HASH, "SHT_HASH" },
	{ SHT_DYNAMIC, "SHT_DYNAMIC" },
	{ SHT_NOTE, "SHT_NOTE" },
	{ SHT_NOBITS, "SHT_NOBITS" },
	{ SHT_REL, "SHT_REL" },
	{ SHT_SHLIB, "SHT_SHLIB" },
	{ SHT_DYNSYM, "SHT_DYNSYM" },
	{ SHT_LOPROC, "SHT_LOPROC" },
	{ SHT_HIPROC, "SHT_HIPROC" },
	{ SHT_LOUSER, "SHT_LOUSER" },
	{ SHT_HIUSER, "SHT_HIUSER" },
};

// Figure 1-18: Symbol Types
const CStringPair<uint8_t> CElfDictionary::m_elf_symbol_type[] =
{
	{ STT_NOTYPE, "Undefined" },
	{ STT_OBJECT, "Data object" },
	{ STT_FUNC, "Function" },
	{ STT_SECTION, "Paired w/o section" },
	{ STT_FILE, "Source file" },
};

// Figure 1-17: Symbol Binding
const CStringPair<uint8_t> CElfDictionary::m_elf_symbol_binding[] =
{
	{ STB_LOCAL, "Local symbol" },
	{ STB_GLOBAL, "Global symbol" },
	{ STB_WEAK, "Weak symbol" },
};

const CStringPair<uint8_t> CElfDictionary::m_elf_symbol_other[] =
{
	{ STV_DEFAULT, "Default" },
	{ STV_INTERNAL, "Internal" },
	{ STV_HIDDEN, "Hidden" },
	{ STV_PROTECTED, "Protected" },
};

const CStringPair<uint32_t> CElfDictionary::m_elf_section_idx[] =
{
	{ SHN_UNDEF, "Undefined" },
	{ SHN_LOPROC, "Proc-spefific (L)" },
	{ SHN_HIPROC, "Proc-spefific (H)" },
	{ SHN_ABS, "Absolute values" },
	{ SHN_COMMON, "Common" },
	{ SHN_HIRESERVE, "HI-Reserved" },
};

CElfHeaderIdentification::CElfHeaderIdentification(const uint8_t* ident)
{
	strcpy(reinterpret_cast<char*>(m_szident), reinterpret_cast<const char*>(ident));
}

void CElfHeaderIdentification::dump() const
{
	CPrintf::log({ " Magic number       : %lu\n", get_magic_num() });
	CPrintf::log({ " Name               : %c%c%c\n", get_ident()[EI_MAG1], get_ident()[EI_MAG2], get_ident()[EI_MAG3] });
	CPrintf::log({ " Class              : %lu\n", get_class() });
	CPrintf::log({ " Data               : %lu\n", get_data() });
	CPrintf::log({ " Version            : %lu\n", get_version() });
}

bool CElfHeaderIdentification::is_valid() const
{
	if (get_magic_num() != ELFMAG0)
	{
		CPrintf::err("ELF Header EI_MAG0 incorrect\n");
		return false;
	}

	if (m_szident[EI_MAG1] != ELFMAG1)
	{
		CPrintf::err("ELF Header EI_MAG1 incorrect\n");
		return false;
	}

	if (m_szident[EI_MAG2] != ELFMAG2)
	{
		CPrintf::err("ELF Header EI_MAG2 incorrect\n");
		return false;
	}

	if (m_szident[EI_MAG3] != ELFMAG3)
	{
		CPrintf::err("ELF Header EI_MAG3 incorrect\n");
		return false;
	}

	if (get_class() != ELFCLASS32)
	{
		CPrintf::err("Only 32-bit elf file is supported\n");
		return false;
	}

	if (get_data() != ELFDATA2LSB)
	{
		CPrintf::err("Unsupported ELF File byte order\n");
		return false;
	}

	if (get_version() != EV_CURRENT)
	{
		CPrintf::err("Unsupported ELF File version\n");
		return false;
	}

	CPrintf::log("ELF header is ok\n");

	return true;
}

bool CElfParser::init(const std::filesystem::path& full_path)
{
	file_path = full_path;

	if (!std::filesystem::exists(file_path))
	{
		CPrintf::err("Input file does not exist\n");
		return false;
	}

	file_size = std::filesystem::file_size(file_path);

	CPrintf::log({ "File size: %s bytes\n", CElfUtils::large_number_format(file_size).c_str() });

	if (!mem_alloc())
	{
		CPrintf::err("Failed to allocate memory\n");
		return false;
	}

	if (!read_file())
	{
		CPrintf::err("Failed to read file\n");
		return false;
	}

	CPrintf::log("Elf parser initialized\n");

	// Set this to true for now, we succeed if we parse
	m_succeeded = false;

	return true;
}

bool CElfParser::parse()
{
	file_elf = reinterpret_cast<elf32_hdr*>(file_buf);

	auto start = std::chrono::high_resolution_clock::now();

	if (!elf_hdr.parse(file_elf))
	{
		CPrintf::err("Couldn't parse elf file\n");
		return false;
	}

	m_process_time_us = std::chrono::duration_cast<std::chrono::microseconds>(
		std::chrono::high_resolution_clock::now() - start).count();

	// Dump the header by default
	elf_hdr.dump();

	CPrintf::log("Elf header parsed\n");

	mem_cleanup();

	m_succeeded = true;

	return true;
}

void CElfParser::clear()
{
	elf_hdr.clear();
}

bool CElfParser::read_file()
{
	std::ifstream ifs(file_path, std::ios_base::in | std::ios_base::binary);

	if (ifs.bad())
		return false;

	ifs.read(reinterpret_cast<char*>(file_buf), file_size);

	ifs.close();

	return true;
}

bool CElfParser::mem_alloc()
{
	file_buf = new uint8_t[file_size];

	if (!file_buf)
	{
		CPrintf::err("Couldn't allocated memory\n");
		return false;
	}

	return true;
}

void CElfParser::mem_cleanup()
{
	delete[] file_buf;
	file_buf = nullptr;
}

bool CElfHeader::parse(elf32_hdr* file_elf)
{
	m_hdr_ident = CElfHeaderIdentification(file_elf->e_ident);

	m_type = { file_elf->e_type, CElfDictionary::get_str(CElfDictionary::m_elf_hdr_type, file_elf->e_type) };
	m_machine = { file_elf->e_machine, CElfDictionary::get_str(CElfDictionary::m_elf_hdr_machine, file_elf->e_machine) };
	m_version = { file_elf->e_version, CElfDictionary::get_str(CElfDictionary::m_elf_hdr_version, file_elf->e_version) };

	// Check if the elf file is valid
	if (!is_valid())
	{
		CPrintf::err("Bad ELF header\n");
		return false;
	}

	m_entry_point_addr = file_elf->e_entry;

	if (!m_entry_point_addr)
		CPrintf::wrn("Elf with no entry point");

	// Program headers
	m_ph_hdr_offset = file_elf->e_phoff;
	m_ph_hdr_size = file_elf->e_phentsize;
	m_num_ph_entries = file_elf->e_phnum;

	CPrintf::log({ "Program headers: %d (offset=" FMT_ADDR ")\n", m_num_ph_entries, m_ph_hdr_offset });

	if (m_num_ph_entries)
		parse_program_hdrs(file_elf);
	else CPrintf::wrn("No program headers");

	// Section headers
	m_sh_hdr_offset = file_elf->e_shoff;
	m_sh_hdr_size = file_elf->e_shentsize;
	m_num_sh_entries = file_elf->e_shnum;

	CPrintf::log({ "Section headers: %d (offset=" FMT_ADDR ")\n", m_num_sh_entries, m_sh_hdr_offset });

	if (m_num_sh_entries)
		parse_section_hdrs(file_elf);
	else CPrintf::wrn("No section headers");

	m_section_name_table_index = file_elf->e_shstrndx;

	if (m_section_name_table_index == SHN_UNDEF)
		CPrintf::wrn("Elf doesn't contain any section name string table\n");

	m_header_size = file_elf->e_ehsize;

	// Check if the size of the elf header is the same
	if (m_header_size != sizeof(elf32_hdr))
	{
		CPrintf::err({ "Mismatch in elf entry size (our=%d, their=%d)\n", sizeof(elf32_hdr), m_header_size});
		return false;
	}

	// String table
	if (m_num_sh_entries)
		parse_strings(file_elf);
	else CPrintf::wrn("Couldn't parse strings because there aren't any sections available\n");

	// Symbol table
	if (m_num_sh_entries)
		parse_symbol_table(file_elf);
	else CPrintf::wrn("Couldn't parse symbols because there aren't any sections available\n");

	CPrintf::newline();
	CPrintf::separator();
	CPrintf::newline();

	return true;
}

void CElfHeader::clear()
{
	m_ph_hdrs.clear();
	m_sh_hdrs.clear();

	m_string_table.clear();

	m_symbol_table.clear();

	m_note_table.clear();
}

void CElfHeader::dump() const
{
	CPrintf::log("--- Header dump ---\n");
	CPrintf::log({ "Identification: \n" });
	m_hdr_ident.dump();
	CPrintf::log({ "Type                : %s\n", m_type.get_str() });
	CPrintf::log({ "Machine             : %s\n", m_machine.get_str() });
	CPrintf::log({ "Version             : %s\n", m_version.get_str() });
	CPrintf::log({ "Entry point         : " FMT_ADDR "\n", m_entry_point_addr });
	CPrintf::log({ "Program header      : " FMT_ADDR "\n", m_ph_hdr_offset });
	CPrintf::log({ "Section header      : " FMT_ADDR "\n", m_sh_hdr_offset });
	CPrintf::log({ "Header size         : %s bytes\n", CElfUtils::large_number_format(m_header_size).c_str() });
	CPrintf::log({ "PH entry size       : %s bytes\n", CElfUtils::large_number_format(m_ph_hdr_size).c_str() });
	CPrintf::log({ "PH entries          : %hu\n", m_num_ph_entries });
	CPrintf::log({ "SH entry size       : %s bytes\n", CElfUtils::large_number_format(m_sh_hdr_size).c_str() });
	CPrintf::log({ "SH entries          : %hu\n", m_num_sh_entries });
	CPrintf::log({ "Entry to sec names  : %hu\n", m_section_name_table_index });

	CPrintf::newline();

}

const CElfSectionHeader* CElfHeader::get_sc_hdr_by_name(const std::string& name) const
{
	for (const auto& sec : m_sh_hdrs)
	{
		if (sec.get_name() == name)
			return &sec;
	}

	CPrintf::err({ "Didn't find section header %s\n", name.c_str() });

	return nullptr;
}

void CElfHeader::parse_program_hdrs(const elf32_hdr* file_elf)
{
	// Get the first header
	elf32_phdr* ph = CElfUtils::get_program_header(file_elf);

	for (uint32_t i = 0; i < m_num_ph_entries; i++, ph++)
	{
		CElfProgramHeader ph_hdr;

		CPrintf::log({ "Found program header at " FMT_ADDR "\n", ph });

		ph_hdr.m_type = { ph->p_type, CElfDictionary::get_str(CElfDictionary::m_elf_phdr_type, ph->p_type) };
		ph_hdr.m_flags = { ph->p_flags, CElfDictionary::get_str(CElfDictionary::m_elf_phdr_flags, ph->p_flags) };

		ph_hdr.m_offset = ph->p_offset;

		ph_hdr.m_filesz = ph->p_filesz;
		ph_hdr.m_memsz = ph->p_memsz;

		ph_hdr.m_align = ph->p_align;

		ph_hdr.m_index = i;

		m_ph_hdrs.emplace_back(ph_hdr);
	}

	CPrintf::log({ "Parsed %d program headers\n", m_num_ph_entries });
	CPrintf::separator();
}

void CElfHeader::parse_section_hdrs(const elf32_hdr* file_elf)
{
	// Get the first header
	elf32_shdr* sh = CElfUtils::get_section_header(file_elf);

	for (uint32_t i = 0; i < m_num_sh_entries; i++, sh++)
	{
		CElfSectionHeader sh_hdr;

		sh_hdr.m_name = CElfUtils::get_section_name_by_index(file_elf, i);

		CPrintf::log({ "Found section %s at " FMT_ADDR "\n", sh_hdr.m_name.c_str(), sh });

		sh_hdr.m_type = { sh->sh_type, CElfDictionary::get_str(CElfDictionary::m_elf_shdr_type, sh->sh_type) };
		sh_hdr.m_flags = sh->sh_flags;

		sh_hdr.m_offset = sh->sh_offset;
		sh_hdr.m_size = sh->sh_size;

		sh_hdr.m_entry_size = sh->sh_entsize;

		sh_hdr.m_link = sh->sh_link;

		sh_hdr.m_index = i;

		// Let's find the string table for symbol names
		if (sh_hdr.m_type.get_first() == SHT_SYMTAB)
		{
			CPrintf::log({ "Section %s marked as symbol section\n", sh_hdr.m_name.c_str() });

			m_sym_strtable_section_index = { sh_hdr.m_link, CElfDictionary::get_str(CElfDictionary::m_elf_section_idx, sh_hdr.m_link) };

			CPrintf::log({ "Found symbol str-table section index in section %s\n", sh_hdr.m_name.c_str() });

			if (m_sym_strtable_section_index.get_first() == SHN_UNDEF)
				CPrintf::wrn({ "A section %s links against underfined section index (%d)\n", sh_hdr.m_name.c_str(), SHN_UNDEF });
		}

		m_sh_hdrs.emplace_back(sh_hdr);
	}

	CPrintf::log({ "Parsed %d sections\n",m_num_sh_entries });
	CPrintf::separator();
}

void CElfHeader::parse_symbol_table(const elf32_hdr* file_elf)
{
	uint32_t symtab_entries = 0;

	for (const auto& sec : m_sh_hdrs)
	{
		// Search for the symbol table section
		if (sec.get_type().get_first() != SHT_SYMTAB)
			continue;

		if (!sec.get_size())
		{
			CPrintf::wrn({ "Got a zero-sized section: %s\n", sec.get_name().c_str() });
			continue;
		}

		symtab_entries = sec.get_size() / sec.get_entry_size();

		elf32_sym* sym = CElfUtils::file_off<elf32_sym*>(file_elf, sec.get_offset());

		if (!sym)
		{
			CPrintf::err({ "Couldn't get symbol table from %s\n", sec.get_name().c_str() });
			continue;
		}

		CPrintf::log({ "Getting symbol table from: %s\n", sec.get_name().c_str() });

		const CElfSectionHeader& sym_strtab_sec = m_sh_hdrs[m_sym_strtable_section_index.get_first()];

		uint32_t* string_table_ptr = CElfUtils::file_off<uint32_t*>(file_elf, sym_strtab_sec.get_offset());

		for (uint32_t i = 0; i < symtab_entries; i++, sym++)
		{
			CElfSymbol elf_sym;

			elf_sym.m_name = CElfUtils::get_symbol_name(string_table_ptr, sym->st_name);

			elf_sym.m_size = sym->st_size;

			uint8_t type, binding;

			type = ELF32_ST_TYPE(sym->st_info);
			binding = ELF32_ST_BIND(sym->st_info);

			elf_sym.m_type = { type, CElfDictionary::get_str(CElfDictionary::m_elf_symbol_type, type) };
			elf_sym.m_binding = { binding, CElfDictionary::get_str(CElfDictionary::m_elf_symbol_binding, binding) };

#if 0
			// Try to demangle itz using MSVC demangler
			if (elf_sym.m_type.get_first() == STT_FUNC)
			{
				const char* decorated_name = elf_sym.m_name.c_str();
				char undecorated_name[1024];
				elf_sym.m_name = __unDName(undecorated_name, decorated_name + 1, 1024, malloc, free, 0x2800);
			}
#endif

			uint8_t other = ELF32_ST_VISIBILITY(sym->st_other);

			elf_sym.m_other = { other, CElfDictionary::get_str(CElfDictionary::m_elf_symbol_other, other) };

			elf_sym.m_relevant_sec_idx = { sym->st_shndx, CElfDictionary::get_str(CElfDictionary::m_elf_section_idx, sym->st_shndx) };

			uint32_t rel_sec_idx = elf_sym.m_relevant_sec_idx.get_first();

			/*if (!CElfUtils::is_special_sc_idx(rel_sec_idx) &&
				elf_sym.m_type.get_first() == STT_OBJECT)
			{
				//uint32_t* ptr = CElfUtils::file_off<uint32_t*>(
				//	CElfUtils::file_off<uint32_t*>(
				//		file_elf, m_sh_hdrs[rel_sec_idx].get_offset()), sym->st_value);

				uint32_t* ptr = CElfUtils::file_off<uint32_t*>(file_elf, sym->st_value);

				CPrintf::log({ "%s: %d " FMT_ADDR "\n", elf_sym.m_name.c_str(), *ptr, sym->st_value });

				elf_sym.m_value = *ptr;//
			}*/

			elf_sym.m_value = sym->st_value;

			elf_sym.m_index = i;

			m_symbol_table.emplace_back(elf_sym);
		}
	}

	CPrintf::log({ "Parsed %d symbols\n", symtab_entries });
	CPrintf::separator();
}

void CElfHeader::parse_note_table(const elf32_hdr* file_elf)
{
	for (const auto& sec : m_sh_hdrs)
	{
		if (sec.get_type().get_first() != SHT_NOTE)
			continue;

		CPrintf::log({ "Found a note section %s\n", sec.get_name().c_str() });

		elf32_note* note = CElfUtils::file_off<elf32_note*>(file_elf, sec.get_offset());

		if (!note->n_namesz || !note->n_descsz)
		{
			CPrintf::wrn({ "There aren't any note entries inside %s\n", sec.get_name().c_str() });
			continue;
		}

		while (note->n_namesz && note->n_descsz)
		{
			CElfNote elf_nt;

			elf_nt.m_name_len = note->n_namesz;
			elf_nt.m_desc_len = note->n_descsz;
			elf_nt.m_type = note->n_type;

			// Name
			for (uint32_t i = 0; i < elf_nt.m_name_len; i++)
				elf_nt.m_name.push_back(*reinterpret_cast<const char*>(((uint8_t*)note + sizeof(elf32_note)) + i));

			// Description
			for (uint32_t i = 0; i < elf_nt.m_desc_len; i++)
				elf_nt.m_desc.push_back(*reinterpret_cast<const char*>(((uint8_t*)note + sizeof(elf32_note) + elf_nt.m_name_len) + i));

			m_note_table.emplace_back(elf_nt);

			note++;
		}
	}

	CPrintf::log({ "Parsed %d note entries\n", m_note_table.size() });
	CPrintf::separator();
}

void CElfHeader::parse_strings(const elf32_hdr* file_elf)
{
	if (m_section_name_table_index == SHN_UNDEF)
	{
		CPrintf::wrn("Can't parse any strings because there isn't a section name table\n");
		return;
	}

	for (const auto& sec : m_sh_hdrs)
	{
		// Look for sections only tagged as SHT_STRTAB. These sections
		// contain strings data such as variable or function names, class
		// names etc.
		if (sec.get_type().get_first() != SHT_STRTAB)
			continue;

		// Since the section that contains names for sections is also
		// tagged as SHT_STRTAB, we skip this one, because we are already
		// aware of the section names, and so we don't have to print them
		// again.
		if (sec.get_index() == m_section_name_table_index)
			continue;

		if (!sec.get_size())
		{
			CPrintf::wrn({ "Got a zero-sized section: %s\n", sec.get_name().c_str() });
			continue;
		}

		char* c_start = CElfUtils::file_off<char*>(file_elf, sec.get_offset());

		std::string cur;

		///CPrintf::log({ "Parsing strings stored inside %s\n", sec.get_name().c_str() });

		uint32_t total_chars = 0;
		for (uint32_t k = 0; k < sec.get_size(); k++)
		{
			const char c = c_start[k];

			if (c == '\0') // We're at the end of current string
			{
				if (cur.empty())
					cur = ELF_UNKNOWN_STR;

				m_string_table.emplace_back(cur + '\0');

				cur.clear();

				// We don't count null-terminating char as a char (:
				total_chars--;

				//CPrintf::raw("\n");

				continue;
			}

			cur.push_back(c);
			//CPrintf::raw({ "%c", c });
			total_chars++;
		}

		//CPrintf::log({ "Parsed %d strings of %d characters\n", m_program_strings.size(), total_chars });
	}
}

elf32_phdr* CElfUtils::get_program_header(const elf32_hdr* elf_hdr)
{
	return CElfUtils::file_off<elf32_phdr*>(elf_hdr, elf_hdr->e_phoff);
}

elf32_shdr* CElfUtils::get_section_header(const elf32_hdr* elf_hdr)
{
	return CElfUtils::file_off<elf32_shdr*>(elf_hdr, elf_hdr->e_shoff);
}

bool CElfUtils::is_special_sc_idx(uint32_t idx)
{
	switch (idx)
	{
		case SHN_UNDEF:
		case SHN_LOPROC:
		case SHN_HIPROC:
		case SHN_LIVEPATCH:
		case SHN_ABS:
		case SHN_COMMON:
		case SHN_HIRESERVE:
			return true;
	}

	return false;
}

elf32_sym* CElfUtils::get_symbol_table(const elf32_shdr* elf_shdr)
{
	return CElfUtils::file_off<elf32_sym*>(elf_shdr, elf_shdr->sh_offset);
}

elf32_shdr* CElfUtils::find_shdr_names(const elf32_hdr* elf_hdr)
{
	if (elf_hdr->e_shstrndx == SHN_UNDEF)
		return nullptr;

	return &reinterpret_cast<elf32_shdr*>(get_section_header(elf_hdr))[elf_hdr->e_shstrndx];
}

std::string CElfUtils::get_symbol_name(uint32_t* string_table_ptr, uint32_t n)
{
	std::string name;

	char* c = reinterpret_cast<char*>((uint8_t*)string_table_ptr + n);

	if (*c == '\0')
		return ELF_UNKNOWN_STR;

	while (*c != '\0')
	{
		name.push_back(*c);

		c++;
	}

	return name;
}

uint32_t* CElfUtils::find_section_names_table_ptr(const elf32_hdr* elf_hdr)
{
	elf32_shdr* sh_names = CElfUtils::find_shdr_names(elf_hdr);

	if (!sh_names)
		return nullptr;

	return CElfUtils::file_off<uint32_t*>(elf_hdr, sh_names->sh_offset);
}

std::string CElfUtils::get_section_name_by_index(const elf32_hdr* elf_hdr, uint32_t n)
{
	std::string sc_name;

	char* c = reinterpret_cast<char*>(find_section_names_table_ptr(elf_hdr));

	uint32_t curr_idx = 0;
	while (n != curr_idx)
	{
		// We're at the end of current string
		if (*c == '\0')
			curr_idx++;

		c++;
	}

	if (!*c)
		return "<unnamed>";

	// Now we're at the position we want to
	while (*c)
	{
		sc_name.push_back(*c);

		c++;
	}

	return sc_name;
}

bool CElfHeader::is_valid() const
{
	if (!m_hdr_ident.is_valid())
	{
		CPrintf::err("Bad header identification\n");
		return false;
	}

	if (m_machine.get_first() != EM_386)
	{
		CPrintf::err("Unsupported ELF File target\n");
		return false;
	}

	return true;
}

void CElfSectionHeader::dump() const
{
	CPrintf::log({ "%-5d %-24s %-16s %-8s 0x%-10X %-12s %-6d\n",
				 m_index, m_name.c_str(),
				 m_type.get_str(), get_flags_str().c_str(),
				 m_offset, CElfUtils::large_number_format(m_size).c_str(), 
				 m_link });
}

void CElfProgramHeader::dump() const
{
	CPrintf::log({ "%-5d %-16s 0x%-10X %-12s %-12s %-20s %-6lu\n",
				 m_index, m_type.get_str(),
				 m_offset, CElfUtils::large_number_format(m_filesz).c_str(),
				 CElfUtils::large_number_format(m_memsz).c_str(),
				 m_flags.get_str(), m_align });
}

void CElfSymbol::dump() const
{
	char rel_sec_idx_buf[20];

	uint16_t val = m_relevant_sec_idx.get_first();

	if (!CElfUtils::is_special_sc_idx(val))
		sprintf(rel_sec_idx_buf, "%d", val);
	else sprintf(rel_sec_idx_buf, "%s", m_relevant_sec_idx.get_str());

	CPrintf::log({ "%-16s %-12s %-12s %-20s %-12s 0x%-10X %s\n", 
				 rel_sec_idx_buf, m_other.get_str(),
				 m_binding.get_str(), m_type.get_str(),
				 CElfUtils::large_number_format(m_size).c_str(),
				 m_value, m_name.c_str() });
}

void CElfDumper::dump_all(const CElfHeader& elf_hdr)
{
	dump_pheaders(elf_hdr);
	dump_sections(elf_hdr);

	dump_symbols(elf_hdr);
	dump_sym_source_files(elf_hdr);
	dump_sym_data(elf_hdr);
	dump_sym_funcs(elf_hdr);
}

void CElfDumper::dump_pheaders(const CElfHeader& elf_hdr)
{
	CPrintf::log({ "--- Dumping %d program headers ---\n", elf_hdr.m_ph_hdrs.size() });
	CPrintf::log({ "%-5s %-16s %-12s %-12s %-12s %-20s %-6s\n", 
				 "Index", "Type", "Offset", "Filesize", "Memsize", "Flags", "Align" });

	for (const auto& ph : elf_hdr.m_ph_hdrs)
		ph.dump();
}

void CElfDumper::dump_sections(const CElfHeader& elf_hdr)
{
	CPrintf::log({ "--- Dumping %d sections ---\n", elf_hdr.m_sh_hdrs.size() });
	CPrintf::log({ "%-5s %-24s %-16s %-8s %-12s %-12s %-6s\n", 
				 "Index", "Name", "Type", "Flags", "Offset", "Size", "Link" });

	for (const auto& sh : elf_hdr.m_sh_hdrs)
		sh.dump();
}

void CElfDumper::dump_symbols(const CElfHeader& elf_hdr)
{
	CPrintf::log({ "--- Dumping %d symbols ---\n", elf_hdr.m_symbol_table.size() });
	CPrintf::log({ "%-16s %-12s %-12s %-20s %-12s %-12s %s\n", 
				 "Relevant sec idx", "Visibility", "Binding", "Type", "Size", "Value", "Name" });

	for (const auto& sym : elf_hdr.m_symbol_table)
		sym.dump();
}

void CElfDumper::dump_notes(const CElfHeader& elf_hdr)
{
	CPrintf::log({ "--- Dumping %d notes ---\n", elf_hdr.m_note_table.size() });
	CPrintf::log({ "%-8s %-8s %-12s %-24s %s\n", 
				 "Name len", "Desc len", "Type", "Name", "Desc" });

	for (const auto& nt : elf_hdr.m_note_table)
		nt.dump();
}

void CElfDumper::cmdline_param_decide(const CElfHeader& elf_hdr)
{
	// Take care of dumping the parsed data
	if (g_arg_parser.get_arg_n<ARG_DUMP_ALL>().exist())
	{
		CElfDumper::dump_all(elf_hdr);
		return;
	}

	if (g_arg_parser.get_arg_n<ARG_DUMP_PHEADERS>().exist())
		CElfDumper::dump_pheaders(elf_hdr);

	if (g_arg_parser.get_arg_n<ARG_DUMP_SECTIONS>().exist())
		CElfDumper::dump_sections(elf_hdr);

	if (g_arg_parser.get_arg_n<ARG_DUMP_SYMBOLS>().exist())
		CElfDumper::dump_symbols(elf_hdr);

	if (g_arg_parser.get_arg_n<ARG_DUMP_SYM_SOURCE_FILES>().exist())
		CElfDumper::dump_sym_source_files(elf_hdr);

	if (g_arg_parser.get_arg_n<ARG_DUMP_SYM_DATA>().exist())
		CElfDumper::dump_sym_data(elf_hdr);

	if (g_arg_parser.get_arg_n<ARG_DUMP_SYM_FUNCTIONS>().exist())
		CElfDumper::dump_sym_funcs(elf_hdr);

	if (g_arg_parser.get_arg_n<ARG_DUMP_NOTES>().exist())
		CElfDumper::dump_notes(elf_hdr);
}

void CElfDumper::dump_sym_source_files(const CElfHeader& elf_hdr)
{
	dump_sym_by_type(elf_hdr, STT_FILE, " source files");
}

void CElfDumper::dump_sym_data(const CElfHeader& elf_hdr)
{
	dump_sym_by_type(elf_hdr, STT_OBJECT, "data");
}

void CElfDumper::dump_sym_funcs(const CElfHeader& elf_hdr)
{
	dump_sym_by_type(elf_hdr, STT_FUNC, "functions");
}

void CElfDumper::dump_sym_by_type(const CElfHeader& elf_hdr, uint8_t type, const char* sz_thing)
{
	CPrintf::log({ "--- Dumping symbol %s ---\n", sz_thing });

	CPrintf::log({ "%-16s %-12s %-16s %-20s %-12s %-12s %s\n",
			 "Relevant sec idx", "Visibility", "Binding", "Type", "Size", "Value", "Name" });

	uint32_t total = 0;
	for (const auto& sym : elf_hdr.m_symbol_table)
	{
		if (sym.get_type().get_first() != type)
			continue;

		char rel_sec_idx_buf[20];

		const auto& rel_idx = sym.get_relevant_sec_idx();

		if (!CElfUtils::is_special_sc_idx(rel_idx.get_first()))
			sprintf(rel_sec_idx_buf, "%d", rel_idx.get_first());
		else sprintf(rel_sec_idx_buf, "%s", rel_idx.get_str());

		CPrintf::log({ "%-16s %-12s %-16s %-20s %-12s 0x%-10X %s\n",
					 rel_sec_idx_buf, sym.get_other().get_str(),
					 sym.get_binding().get_str(), sym.get_type().get_str(),
					 CElfUtils::large_number_format(sym.get_size()).c_str(),
					 sym.get_value(), sym.get_name().c_str() });

		total++;
	}

	CPrintf::log("--- End ---\n");
	CPrintf::log({ "Total amount of %d %s\n", total, sz_thing });
}

void CElfNote::dump() const
{
	CPrintf::log({ "%-8d %-8d 0x%-10X %-24s %s\n",
			 m_name_len, m_desc_len,
			 m_type, 
			 m_name.c_str(), m_desc.c_str() });
}

bool CElfLogger::init(const std::filesystem::path& initial_proc_dir, const std::filesystem::path& curr_proc_file)
{
	const auto log_path_full = prepare_curr_log_path(initial_proc_dir, curr_proc_file);

	if (!CLogger::prepare_new_file(log_path_full))
	{
		CPrintf::err("Couldn't prepare new file for the logger.\n");
		return false;
	}

	CPrintf::log({ "The logging file is %s\n", log_path_full.string().c_str() });

	return true;
}

bool CElfLogger::cmdline_param_decide(const CElfHeader& elf_hdr, const std::filesystem::path& initial_proc_dir, const std::filesystem::path& curr_proc_file)
{
	CPrintf::log("Logging started\n");

	if (!init(initial_proc_dir, curr_proc_file))
	{
		CPrintf::err({ "Couldn't initialize logger, fatal error\n"});
		return false;
	}

	// Take care of dumping the parsed data
	if (g_arg_parser.get_arg_n<ARG_LOG_SYM_ALL>().exist())
	{
		CElfLogger::log_all(elf_hdr);

		CPrintf::log("Logging endded\n");
		return true;
	}

	if (g_arg_parser.get_arg_n<ARG_LOG_SYM_SOURCE_FILES>().exist())
		log_sym_source_files(elf_hdr);

	if (g_arg_parser.get_arg_n<ARG_LOG_SYM_DATA>().exist())
		log_sym_data(elf_hdr);

	if (g_arg_parser.get_arg_n<ARG_LOG_SYM_FUNCTIONS>().exist())
		log_sym_funcs(elf_hdr);

	CPrintf::log("Logging endded\n");

	return true;
}

std::filesystem::path CElfLogger::prepare_curr_log_path(const std::filesystem::path& initial_proc_dir, const std::filesystem::path& curr_proc_file)
{
	const auto curr_path = std::filesystem::current_path();

	std::filesystem::path log_path = curr_path.string() + "\\logs\\";

	if (!std::filesystem::exists(log_path))
		std::filesystem::create_directory(log_path);

	if (initial_proc_dir != curr_proc_file.parent_path())
	{
		std::deque<std::string> vec_path_difference;

		auto curr_path = curr_proc_file.parent_path();

		// We get the path difference from the initial path but inverted
		while (curr_path != initial_proc_dir)
		{
			vec_path_difference.push_front(curr_path.filename().string());

			curr_path = curr_path.parent_path();
		}

		for (const auto& p : vec_path_difference)
		{
			log_path += p + '\\';

			// We have to create this new directory since it's more likely thats it doesn't exist yet.
			if (!std::filesystem::exists(log_path))
			{
				// This is kinda dumb, but use parent_path here because the ending character is \.
				std::filesystem::create_directory(log_path.parent_path());
			}
		}
	}

	std::filesystem::path log_path_full = log_path.string() + curr_proc_file.filename().string();

	log_path_full.replace_extension(".log");

	// Overwrite the file. The filehandle will be closed at the end of this scope.
	std::ofstream ofs(log_path_full);

	ofs << "Log file generated by elf-parser" << std::endl;
	ofs << "Input file: " << curr_proc_file.string() << std::endl << std::endl;

	return log_path_full;
}

void CElfLogger::log_all(const CElfHeader& elf_hdr)
{
	log_sym_source_files(elf_hdr);
	log_sym_data(elf_hdr);
	log_sym_funcs(elf_hdr);
}

void CElfLogger::log_sym_source_files(const CElfHeader& elf_hdr)
{
	log_sym_by_type(elf_hdr, STT_FILE, "source file data");
}

void CElfLogger::log_sym_data(const CElfHeader& elf_hdr)
{
	log_sym_by_type(elf_hdr, STT_OBJECT, "symbol object data");
}

void CElfLogger::log_sym_funcs(const CElfHeader& elf_hdr)
{
	log_sym_by_type(elf_hdr, STT_FUNC, "symbol source function data");
}

void CElfLogger::log_sym_by_type(const CElfHeader& elf_hdr, uint8_t type, const char* sz_thing)
{
	CLogger::log({ "--- A list of %s ---\n", sz_thing });
	CLogger::newline();

	uint32_t total = 0;
	for (const auto& sym : elf_hdr.m_symbol_table)
	{
		if (sym.get_type().get_first() != type)
			continue;

		CLogger::log({ "%s\n", sym.get_name().c_str() });

		total++;
	}

	CLogger::newline();
	CLogger::log("--- end ---\n");
	CLogger::newline();

	CLogger::log({ "Total amount of %d %s\n", total, sz_thing });
	CLogger::separator();
}
