#ifndef _C_ARGUMENT_PARSER_
#define _C_ARGUMENT_PARSER_

#pragma once

enum EArgType : uint32_t
{
	ARG_TYPE_SINGLE,
	ARG_TYPE_DOUBLE,
	ARG_TYPE_TRIPLE,
};

class CArg
{
public:
	CArg(EArgType type, const std::string name, const std::string usage, const std::string& desc) :
		m_type(type),
		m_name(name),
		m_usage(usage), m_desc(desc),
		m_exists(false)
	{}

	inline EArgType get_type() const { return m_type; }

	inline const auto& get_name() const { return m_name; }

	inline const auto& get_value() const { return m_value; }
	inline const auto& get_value1() const { return m_value1; }

	inline const auto& get_usage() const { return m_usage; }
	inline const auto& get_desc() const { return m_desc; }

	inline bool exist() const { return m_exists; }

public:
	EArgType m_type;

	std::string m_name;

	std::string m_value, m_value1;
	std::string m_usage, m_desc;

	bool m_exists;
};

enum EArg : uint32_t
{
	ARG_HELP,
	
	ARG_FILE,

	ARG_DUMP_ALL,
	ARG_DUMP_PHEADERS,
	ARG_DUMP_SECTIONS,
	ARG_DUMP_SYMBOLS,
	ARG_DUMP_SYM_SOURCE_FILES,
	ARG_DUMP_SYM_DATA,
	ARG_DUMP_SYM_FUNCTIONS,
	ARG_DUMP_NOTES,

	ARG_LOG_SYM_ALL,
	ARG_LOG_SYM_SOURCE_FILES,
	ARG_LOG_SYM_DATA,
	ARG_LOG_SYM_FUNCTIONS,

	ARG_COUNT
};

class CCmdArgParser
{
public:
	void parse(int32_t argc, char** argv);

	// Return argument by index
	template<uint32_t N> requires(N >= 0 && N < ARG_COUNT)
	inline const auto& get_arg_n() const { return m_arg_list[N]; }

	template<uint32_t N> requires(N >= 0 && N < ARG_COUNT)
	inline bool is_arg_present() const { return get_arg_n<N>().exist(); }

	inline const auto& get_total_arg_list() const { return m_arg_list; }

private:
	std::vector<CArg*> m_args;

	static CArg m_arg_list[ARG_COUNT];
};

#endif