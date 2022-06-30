#ifndef INC_H
#define INC_H

#define _CRT_SECURE_NO_WARNINGS

#define FMT_ADDR "0x%08X"

#include <filesystem>
#include <vector>
#include <array>
#include <map>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <locale>
#include <deque>
#include <windows.h>

#include <elf.h>
#include <elf-em.h>

#include <dwarf.h>
#include <dwarf2.h>

#include "CPrintf.h"
#include "CElfParser.h"
#include "CCmdArgParser.h"
#include "CDwarfParser.h"

// This is shared globally
extern CCmdArgParser g_arg_parser;

#endif