#include "inc.h"

CFmtBuf::CFmtBuf(const char* fmt, ...)
{
	va_list va;

	va_start(va, fmt);
	_vsnprintf(m_buffer, sizeof(m_buffer) - 1, fmt, va);
	va_end(va);
}

void CPrintf::log(const CFmtBuf& buf)
{
	char temp[1024];
	sprintf(temp, "[Log] %s", buf.get_buf());
	printf(temp);
}

void CPrintf::err(const CFmtBuf& buf)
{
	char temp[1024];
	sprintf(temp, "[Err] %s", buf.get_buf());
	printf(temp);
}

void CPrintf::wrn(const CFmtBuf& buf)
{
	char temp[1024];
	sprintf(temp, "[Wrn] %s", buf.get_buf());
	printf(temp);
}

void CPrintf::raw(const CFmtBuf& buf)
{
	printf(buf.get_buf());
}

void CPrintf::exit(const CFmtBuf& buf)
{
	char temp[1024];
	sprintf(temp, "Exiting application due to following reason:\n\n %s", buf.get_buf());
	printf(temp);
}

void CPrintf::newline()
{
	log("\n");
}

void CPrintf::separator()
{
	log("----------------------------------\n");
}

bool CLogger::prepare_new_file(const std::filesystem::path& path)
{
	m_working_file = path;

	if (!std::filesystem::exists(m_working_file))
	{
		CPrintf::err({ "Failed to prepare new working file: %s\n", path.string().c_str() });
		return false;
	}

	return true;
}

void CLogger::log(const CFmtBuf& buf)
{
	log_internal(buf);
}

void CLogger::newline()
{
	log_internal("\n");
}

void CLogger::separator()
{
	log_internal("----------------------------------\n");
}

void CLogger::log_internal(const CFmtBuf& buf)
{
	std::ofstream ofs(m_working_file, std::ios_base::out | std::ios_base::app);

	if (ofs.bad())
	{
		CPrintf::err({ "Couldn't log to file %s\n", m_working_file.filename().string().c_str() });
		return;
	}

	ofs << buf.get_buf();

	ofs.close();
}
