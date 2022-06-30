#include "inc.h"

CCmdArgParser g_arg_parser;

class CProcessResult
{
public:
	inline double get_process_time_us() const { return m_total_process_time_us; }
	
	inline uint32_t get_total_files() const { return m_total_files; }
	inline uint32_t get_files_failed() const { return m_files_failed; }
	inline uint32_t get_files_succeeded() const { return m_total_files - m_files_failed; }

public:
	double m_total_process_time_us;

	uint32_t m_total_files;
	uint32_t m_files_failed;
};

void sleep_till_input()
{
	CPrintf::log("\nPress any key to continue...\n");

	std::cin.get();
}

bool process_elf(const std::filesystem::path& initial_proc_dir, const std::filesystem::path& full_path, CElfParser& elf_parser, CProcessResult* process_result)
{
	CPrintf::log({ "Input file: %s\n", full_path.filename().string().c_str() });

	auto start = std::chrono::high_resolution_clock::now();

	// Initialize
	if (!elf_parser.init(full_path))
	{
		CPrintf::err("Couldn't initialize elf parser\n");

		process_result->m_files_failed++;
		return false;
	}

	// Parse
	if (!elf_parser.parse())
	{
		CPrintf::err("Couldn't parse elf\n");

		process_result->m_files_failed++;
		return false;
	}

#if 0
	CDwarfParser dwarf_parser;

	// Parse DWARF
	if (!dwarf_parser.parse(elf_parser))
	{
		CPrintf::err("Couldn't parse DWARF\n");

		process_result->m_files_failed++;
		return false;
	}
#endif

	CPrintf::log("Processed elf file\n");

	CPrintf::separator();

	double time_dt = std::chrono::duration_cast<std::chrono::microseconds>(
		std::chrono::high_resolution_clock::now() - start).count();

	process_result->m_total_process_time_us += time_dt;

	// Dumping
	CElfDumper::cmdline_param_decide(elf_parser.get_elf_hdr());

	// Logging
	if (!CElfLogger::cmdline_param_decide(elf_parser.get_elf_hdr(), initial_proc_dir, elf_parser.get_filepath()))
	{
		CPrintf::err("Couldn't log file data\n");
		return false;
	}

	// We can clear the elf data now
	elf_parser.clear();

	CPrintf::separator();

	CPrintf::log({ "Took %0.3f seconds to process\n", time_dt / (1000.0 * 1000.0) });

	process_result->m_total_files++;
	return true;
}

int main(int argc, char** argv)
{
	CPrintf::log("Start!\n");

	g_arg_parser.parse(argc, argv);

	if (g_arg_parser.is_arg_present<ARG_HELP>())
	{
		CPrintf::raw("--- List of available commands ---\n");
		CPrintf::raw({ "%-30s %-30s %s\n", "Name", "Usage", "Description" });

		for (const auto& arg : g_arg_parser.get_total_arg_list())
			CPrintf::raw({ "%-30s %-30s %s\n", arg.get_name().c_str(), arg.get_usage().c_str(), arg.get_desc().c_str() });

		sleep_till_input();
		return true;
	}

	if (!g_arg_parser.is_arg_present<ARG_FILE>())
	{
		CPrintf::err("You have to enter input file by using -file <file>\n");

		sleep_till_input();
		return false;
	}

	const std::filesystem::path full_path = g_arg_parser.get_arg_n<ARG_FILE>().get_value();
	
	bool is_directory = std::filesystem::is_directory(full_path);

	CProcessResult process_result = {};

	std::vector<CElfParser> parsed_elfs;

	if (is_directory)
	{
		CPrintf::log("The input is a directory, processing all elf files...\n");

		for (const auto& file : std::filesystem::recursive_directory_iterator(full_path))
		{
			if (file.path().extension() != ".so")
				continue;

			CElfParser parser;

			// The top-most parent path
			std::filesystem::path initial_proc_dir = full_path;

			if (!is_directory)
				initial_proc_dir = full_path.parent_path();

			if (!process_elf(initial_proc_dir, file, parser, &process_result))
				CPrintf::err({ "Couldn't process %s\n", file.path().filename().string().c_str() });

			parsed_elfs.emplace_back(parser);
		}

		if (parsed_elfs.empty())
		{
			CPrintf::err("Failed to parse any elf files\n");
			sleep_till_input();
			return false;
		}
	}
	else
	{
		CPrintf::log("The input is a file\n");

		CElfParser parser;

		if (!process_elf(full_path.parent_path(), full_path, parser, &process_result))
			CPrintf::err({ "Couldn't process %s\n", full_path.filename().string().c_str() });

		parsed_elfs.emplace_back(parser);
	}

	CPrintf::separator();
	
	CPrintf::log({ "%-32s %-12s %-12s %-12s\n", "Name", "Size", "Succeeded", "Time"});

	for (auto& file : parsed_elfs)
	{
		CPrintf::log({ "%-32s %-12s %-12s %0.3f\n",
					 file.get_filepath().filename().string().c_str(),
					 CElfUtils::large_number_format(std::filesystem::file_size(file.get_filepath())).c_str(),
					 file.did_succeed() ? "yes" : "no", 
					 file.get_process_time_us() / (1000.0 * 1000.0) });
	}

	CPrintf::separator();
	CPrintf::log({ "Parsed %d elf files (%d succeeded)\n", process_result.get_total_files(), process_result.get_files_succeeded() });
	CPrintf::log({ "Took %0.3f seconds to process\n", process_result.get_process_time_us() / (1000.0 * 1000.0) });
	CPrintf::separator();

	CPrintf::newline();

	CPrintf::log("End!\n");

	sleep_till_input();
	return true;
}
