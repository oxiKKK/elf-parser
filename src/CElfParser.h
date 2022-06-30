#ifndef C_ELF_PARSER_H
#define C_ELF_PARSER_H

#pragma once

#define ELF_NONE_STR		"<n/a>"
#define ELF_UNKNOWN_STR		"<unknown>"

// Used to store an type and an equivalent in a string from describing the type
template<typename T>
class CStringPair
{
public:
	CStringPair(T type, const std::string& str) :
		m_type(type), 
		m_str(str)
	{}
	CStringPair() : 
		m_type(0),
		m_str(ELF_NONE_STR)
	{}

	inline const auto& get_first() const { return m_type; }
	inline const auto& get_second() const { return m_str; }

	inline const char* get_str() const { return m_str.c_str(); }

private:
	T m_type;
	std::string m_str;
};

// A static class used to store string pairs
class CElfDictionary
{
public:
	// From elf header
	static const CStringPair<uint32_t> m_elf_hdr_type[];
	static const CStringPair<uint16_t> m_elf_hdr_machine[];
	static const CStringPair<uint32_t> m_elf_hdr_version[];

	// From elf ph header
	static const CStringPair<uint32_t> m_elf_phdr_type[];
	static const CStringPair<uint32_t> m_elf_phdr_flags[];

	// From elf sh header
	static const CStringPair<uint32_t> m_elf_shdr_type[];

	// From elf symbol
	static const CStringPair<uint8_t> m_elf_symbol_type[];
	static const CStringPair<uint8_t> m_elf_symbol_binding[];
	static const CStringPair<uint8_t> m_elf_symbol_other[];

	// From section index
	static const CStringPair<uint32_t> m_elf_section_idx[];

	template<typename T, typename U>
	static inline const char* get_str(const T& which, U elem) 
	{
		for (const auto& el : which)
			if (el.get_first() == elem)
				return el.get_str();

		return ELF_NONE_STR;
	}
};

class CElfHeaderIdentification
{
public:
	explicit CElfHeaderIdentification(const uint8_t* ident);
	CElfHeaderIdentification()
	{}

public:
	inline const uint8_t* get_ident() const { return m_szident; }

	inline const uint8_t get_magic_num() const { return m_szident[EI_MAG0]; }
	inline const uint8_t get_class() const { return m_szident[EI_CLASS]; }
	inline const uint8_t get_data() const { return m_szident[EI_DATA]; }
	inline const uint8_t get_version() const { return m_szident[EI_VERSION]; }

	void dump() const;

	bool is_valid() const;

private:
	uint8_t m_szident[EI_NIDENT];
};

class CElfProgramHeader
{
public:
	CElfProgramHeader() :
		m_type(0, ELF_NONE_STR), m_flags(0, ELF_NONE_STR), 
		m_offset(0), 
		m_filesz(0), m_memsz(0),
		m_align(0), 
		m_index(0)
	{}

public:
	inline const auto& get_type() const { return m_type; }
	inline const auto& get_flags() const { return m_flags; }

	inline uint32_t get_offset() const { return m_offset; }

	inline uint32_t get_file_sz() const { return m_filesz; }
	inline uint32_t get_mem_sz() const { return m_memsz; }

	inline uint16_t get_align() const { return m_align; }

	void dump() const;

public:
	CStringPair<uint32_t> m_type, m_flags;

	uint32_t m_offset;

	uint32_t m_filesz, m_memsz;

	uint32_t m_align;

	// Program header index - this isn't really needed
	uint32_t m_index;
};

class CElfSectionHeader
{
public:
	CElfSectionHeader() :
		m_name(ELF_NONE_STR),
		m_type(0, ELF_NONE_STR), 
		m_flags(0),
		m_offset(0), m_size(0), 
		m_entry_size(0),
		m_link(0),
		m_index(0)
	{}

public:
	inline const auto& get_name() const { return m_name; }

	inline const auto& get_type() const { return m_type; }
	inline const auto& get_flags() const { return m_flags; }

	inline uint32_t get_offset() const { return m_offset; }
	inline uint32_t get_size() const { return m_size; }

	inline uint32_t get_entry_size() const { return m_entry_size; }

	inline uint32_t get_link() const { return m_link; }

	inline uint32_t get_index() const { return m_index; }

	inline std::string get_flags_str() const 
	{
		std::string str;
		if (m_flags & SHF_WRITE)
			str.push_back('W');

		if (m_flags & SHF_ALLOC)
			str.push_back('A');

		if (m_flags & SHF_EXECINSTR)
			str.push_back('E');

		if (m_flags & SHF_MASKPROC)
			str.push_back('M');

		if (str.empty())
			str = ELF_NONE_STR;

		return str;
	}

	template<typename T>
	inline T* get_data_at_offset(uint8_t* base) { return reinterpret_cast<T*>(base + m_offset); }

	void dump() const;

public:
	std::string m_name;

	CStringPair<uint32_t> m_type;
	uint32_t m_flags;

	uint32_t m_offset, m_size;

	// Some sections hold a table of fixed-sized entries, such as
	// a symbol table.  For such a section, this member gives the
	// size in bytes for each entry.  This member contains zero
	// if the section does not hold a table of fixed-size
	// entries.
	uint32_t m_entry_size;

	// This member holds a section header table index link, whose
	// interpretation depends on the section type.
	uint32_t m_link;

	// Section index - this really doesn't have any other purpose than
	// comparing this afterwards to for example e_shstrndx..
	uint32_t m_index;
};

class CElfSymbol
{
public:
	CElfSymbol() :
		m_name(ELF_UNKNOWN_STR), 
		m_value(0), 
		m_size(0), 
		m_type(0, ELF_NONE_STR), m_binding(0, ELF_NONE_STR),
		m_relevant_sec_idx(0, ELF_NONE_STR),
		m_index(0)
	{}

public:
	inline const auto& get_name() const { return m_name; }

	inline uint32_t get_value() const { return m_value; }
	inline uint32_t get_size() const { return m_size; }

	inline const auto& get_relevant_sec_idx() const { return m_relevant_sec_idx; }
	
	inline const auto& get_type() const { return m_type; }
	inline const auto& get_binding() const { return m_binding; }
	inline const auto& get_other() const { return m_other; }

	void dump() const;

public:
	std::string m_name;

	uint32_t m_value;

	uint32_t m_size;

	CStringPair<uint8_t> m_type, m_binding;

	CStringPair<uint8_t> m_other;

	// Every symbol table entry is "defined" in relation to some
	// section.This member holds the relevant section header
	// table index.
	CStringPair<uint16_t> m_relevant_sec_idx;

	uint32_t m_index;
};

class CElfNote
{
public:
	CElfNote() :
		m_name(ELF_UNKNOWN_STR), 
		m_desc(ELF_UNKNOWN_STR),
		m_name_len(0), 
		m_desc_len(0),
		m_type(0)
	{}

public:
	inline const auto& get_name() const { return m_name; };
	inline const auto& get_desc() const { return m_desc; };

	inline uint32_t get_type() const { return m_type; }

	void dump() const;

public:
	std::string m_name, m_desc;
	uint32_t m_name_len, m_desc_len;

	uint32_t m_type;
};

class CElfHeader
{
public:
	CElfHeader() :
		m_hdr_ident(),
		m_type(0, ELF_NONE_STR), m_machine(0, ELF_NONE_STR),
		m_version(0, ELF_NONE_STR), 
		m_entry_point_addr(0), 
		m_ph_hdr_offset(0), m_sh_hdr_offset(0), 
		m_ph_hdr_size(0), m_sh_hdr_size(0),
		m_num_ph_entries(0), m_num_sh_entries(0), 
		m_section_name_table_index(0), 
		m_header_size(0)
	{}

public:
	bool is_valid() const;

	inline bool has_ph_hdrs() const { return !m_ph_hdrs.empty(); }
	inline bool has_sh_hdrs() const { return !m_sh_hdrs.empty(); }
	inline bool has_syms() const { return !m_symbol_table.empty(); }
	inline bool has_notes() const { return !m_note_table.empty(); }

	bool parse(elf32_hdr* file_elf);
	void clear();

	void dump() const;

	// Returns nullptr on fail
	const CElfSectionHeader* get_sc_hdr_by_name(const std::string& name) const;

private:
	void parse_program_hdrs(const elf32_hdr* file_elf);
	void parse_section_hdrs(const elf32_hdr* file_elf);
	
	void parse_symbol_table(const elf32_hdr* file_elf);

	void parse_note_table(const elf32_hdr* file_elf);

	void parse_strings(const elf32_hdr* file_elf);

public:
	CElfHeaderIdentification m_hdr_ident;

	CStringPair<uint16_t> m_type, m_machine;
	CStringPair<uint32_t> m_version;

	uint32_t m_entry_point_addr;

	uint32_t m_ph_hdr_offset, m_sh_hdr_offset;

	// Size of headers in bytes
	uint32_t m_ph_hdr_size, m_sh_hdr_size;

	uint32_t m_num_ph_entries, m_num_sh_entries;

	// And index into section which holds section data names
	uint32_t m_section_name_table_index;

	// Size of the elf header in bytes
	uint32_t m_header_size;

	// elf headers
	std::vector<CElfProgramHeader> m_ph_hdrs;
	std::vector<CElfSectionHeader> m_sh_hdrs;

	// Contains strings found in headers tagged as SHT_STRTAB
	std::vector<std::string> m_string_table;

	std::vector<CElfSymbol> m_symbol_table;

	// Holds an index into the section table which is a string table for
	// the symbol table. Every individual symbol name is stored there.
	CStringPair<uint32_t> m_sym_strtable_section_index;

	std::vector<CElfNote> m_note_table;
};

class CElfParser
{
public:
	// Allocates the file bufer and sets the filesize
	bool init(const std::filesystem::path& full_path);

	// Parses the elf header information
	bool parse();

	// Cleans up all the stuff and frees memory
	void clear();

	inline const auto& get_elf_hdr() const { return elf_hdr; }
	inline const auto& get_filepath() const { return file_path; }

	inline bool did_succeed() const { return m_succeeded; }

	inline elf32_hdr* get_file_elf() const { return file_elf; }

	inline double get_process_time_us() const { return m_process_time_us; }

private:
	// Copies file buffer into allocated memory
	bool read_file();

	bool mem_alloc();
	void mem_cleanup();

private:
	// File information
	std::filesystem::path file_path;
	uint8_t* file_buf;
	uint32_t file_size;

	double m_process_time_us;

	// Just a cast to file_buf
	elf32_hdr* file_elf;

	// Our elf header 
	CElfHeader elf_hdr;

	bool m_succeeded;
};

using StrCon_t = std::pair<uint32_t, const char*>;

// A static class that provides some utility routines when managing elf files
class CElfUtils
{
public:	
	static elf32_phdr* get_program_header(const elf32_hdr* elf_hdr);
	static elf32_shdr* get_section_header(const elf32_hdr* elf_hdr);
	static elf32_sym* get_symbol_table(const elf32_shdr* elf_shdr);

	static elf32_shdr* find_shdr_names(const elf32_hdr* elf_hdr);
	static uint32_t* find_section_names_table_ptr(const elf32_hdr* elf_hdr);
	static std::string get_section_name_by_index(const elf32_hdr* elf_hdr, uint32_t n);

	static std::string get_symbol_name(uint32_t* string_table_ptr, uint32_t n);

	static bool is_special_sc_idx(uint32_t sc_idx);

	template<typename T, typename U>
	static inline T file_off(U* base, uint32_t off)
	{
		return reinterpret_cast<T>((uint8_t*)base + off);
	}

	// https://stackoverflow.com/questions/7276826/format-number-with-commas-in-c
	template<class T>
	static inline std::string large_number_format(T value)
	{
		std::stringstream ss;
		ss.imbue(std::locale(""));
		ss << std::fixed << value;
		return ss.str();
	}
};

// A static class used to dump useful stuff from the elf header
class CElfDumper
{
public:
	static void cmdline_param_decide(const CElfHeader& elf_hdr);

	static void dump_all(const CElfHeader& elf_hdr);
	static void dump_pheaders(const CElfHeader& elf_hdr);
	static void dump_sections(const CElfHeader& elf_hdr);
	static void dump_symbols(const CElfHeader& elf_hdr);
	static void dump_sym_source_files(const CElfHeader& elf_hdr);
	static void dump_sym_data(const CElfHeader& elf_hdr);
	static void dump_sym_funcs(const CElfHeader& elf_hdr);
	static void dump_sym_by_type(const CElfHeader& elf_hdr, uint8_t type, const char* sz_thing);
	static void dump_notes(const CElfHeader& elf_hdr);
};

// A static class used to dump useful stuff from the elf header into a file
class CElfLogger
{
public:
	static bool cmdline_param_decide(const CElfHeader& elf_hdr, const std::filesystem::path& initial_proc_dir, const std::filesystem::path& curr_proc_file);

	static std::filesystem::path prepare_curr_log_path(const std::filesystem::path& initial_proc_dir, const std::filesystem::path& curr_proc_file);

	static void log_all(const CElfHeader& elf_hdr);
	static void log_sym_source_files(const CElfHeader& elf_hdr);
	static void log_sym_data(const CElfHeader& elf_hdr);
	static void log_sym_funcs(const CElfHeader& elf_hdr);
	static void log_sym_by_type(const CElfHeader& elf_hdr, uint8_t type, const char* sz_thing);

private:
	static bool init(const std::filesystem::path& initial_proc_dir, const std::filesystem::path& curr_proc_file);
};

#endif