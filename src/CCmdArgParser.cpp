#include "inc.h"

CArg CCmdArgParser::m_arg_list[ARG_COUNT] =
{
	{ ARG_TYPE_SINGLE, "-help", "<>", "Displayes command list" },

	// Input
	{ ARG_TYPE_DOUBLE, "-file", "<\"path to the file\">", "Specifies the input file" },

	// Dumping
	{ ARG_TYPE_SINGLE, "-dump_all", "<>", "Dumps out all the information about elf file" },
	{ ARG_TYPE_SINGLE, "-dump_pheaders", "<>", "Dumps out program headers information" },
	{ ARG_TYPE_SINGLE, "-dump_sections", "<>", "Dumps out section headers information" },
	{ ARG_TYPE_SINGLE, "-dump_symbols", "<>", "Dumps out all symbols information" },
	{ ARG_TYPE_SINGLE, "-dump_sym_source_files", "<>", "Dumps out all symbols that are tagged as source files" },
	{ ARG_TYPE_SINGLE, "-dump_sym_data", "<>", "Dumps out all symbols that are tagged as data" },
	{ ARG_TYPE_SINGLE, "-dump_sym_funcs", "<>", "Dumps out all symbols that are tagged as functions" },
	{ ARG_TYPE_SINGLE, "-dump_notes", "<>", "Dumps out all notes" },

	// Logging
	{ ARG_TYPE_SINGLE, "-log_sym_all", "<>", "Logs out all symbols" },
	{ ARG_TYPE_SINGLE, "-log_sym_source_files", "<>", "Logs out all symbols that are tagged as source files" },
	{ ARG_TYPE_SINGLE, "-log_sym_data", "<>", "Logs out all symbols that are tagged as object data" },
	{ ARG_TYPE_SINGLE, "-log_sym_funcs", "<>", "Logs out all symbols that are tagged as functions" },
};

void CCmdArgParser::parse(int32_t argc, char** argv)
{
	for (int32_t i = 0; i < argc; i++)
	{
		for (uint32_t k = 0; k < ARG_COUNT; k++)
		{
			auto arg = &m_arg_list[k];

			if (arg->m_name != argv[i])
				continue;

			arg->m_exists = false;

			switch (arg->get_type())
			{
				case ARG_TYPE_SINGLE:
					arg->m_exists = true;
					m_args.emplace_back(arg);
					break;

				case ARG_TYPE_DOUBLE:
					if (argv[i + 1])
					{
						arg->m_exists = true;
						arg->m_value = argv[i + 1];
						m_args.emplace_back(arg);
					}
					break;
				case ARG_TYPE_TRIPLE:
					if (argv[i + 1] && argv[i + 2])
					{
						arg->m_exists = true;
						arg->m_value = argv[i + 1];
						arg->m_value1 = argv[i + 2];
						m_args.emplace_back(arg);
					}
					break;
				default:
					break;
			}
		}
	}
}