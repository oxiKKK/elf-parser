#ifndef C_PRINTF_H
#define C_PRINTF_H

class CFmtBuf
{
public:
	CFmtBuf(const char* fmt, ...);
	CFmtBuf() = delete;

public:
	inline const char* get_buf() const { return m_buffer; }

private:
	char m_buffer[1024];
};

class CPrintf
{
public:
	static void log(const CFmtBuf& buf);
	static void err(const CFmtBuf& buf);
	static void wrn(const CFmtBuf& buf);
	static void raw(const CFmtBuf& buf);

	static void exit(const CFmtBuf& buf);

	static void newline();
	static void separator();
};

class CLogger
{
public:
	// This has to be called BEFORE you start logging. If the directory
	// to the new file doesn't exist, the function will fail.
	static bool prepare_new_file(const std::filesystem::path& path);

	static void log(const CFmtBuf& buf);

	static void newline();
	static void separator();

	static inline const auto& get_working_file() { return m_working_file; };

private:
	static void log_internal(const CFmtBuf& buf);

private:
	static inline std::filesystem::path m_working_file;
};

#endif