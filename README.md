# :clipboard: General description
An **Executable and linkable format (ELF)** is a file format used to store exetutable files or object files. It is being used among different UNIX systems such as linux. It's structure is similiar to the Windows's **Portable Executable (PE)** file format. 

For more information about the elf file format, you can read the **Executable and Linkable Format (ELF)** document: _https://www.cs.cmu.edu/afs/cs/academic/class/15213-f00/docs/elf.pdf_

# :question: Why does this exist?
I've personally developed this project as an utility for another project, that is eventually a reverse-engineering one. The ability to dump out or log various information stored inside and elf file such as symbols (function names, data or source files), sections and more, is crucial for projects like this, because it allows me to see everything that is stored inside the binary file in a somewhat organized way and thus allows me to see my progress in reverse-engineering it.

# :hourglass_flowing_sand: Test & proof

As a test I've included a log of the GoldSource engine's linux binary files. They're included under _logs\\_ directory. This information is totally free and you can use it however you want.

# :bulb: Features

Since there's no GUI, the only thing we can do is either display the information or log it into a file. There a two modes - dumping and logging. Each mode has it's own way of representing things.

## :computer: Dumping

The information is written to the console. Dumping is a temporary way of inspecting contents of the file, rather than saving it permanently using logging.

## :floppy_disk: Logging

The information is logged into a system of _.log_ files that lies inside the _workingdir\logs_ directory. If the input is not a file but rather recursive directory, the program creates a set of subdirectories inside the _logs\\_ folder following the same pattern as original.
 
# :hammer: Usage

This program is a command-line controlled program, so it's controlled by a bunch of cmd-line parameters.

## Example input for a file

```
 elf-parser -file "file.so" -dump_all -log_sym_all
```
## Example input for a directory

```
 elf-parser -file "first_directory\another_dir" -dump_all -log_sym_all
```

| Name                     | Usage                         | Description                                                                   |
| :----------------------: | :---------------------------: | ----------------------------------------------------------------------------- |
| `-help`                  | `<>`                          | Displayes a list of available commands.                                       |
| `-file`                  | `<path to file or directory>` | Processes specific .so file or multiple files recursively inside a directory. |
| `-dump_all`              | `<>`                          | Dumps out all the information about elf file.                                 |
| `-dump_pheaders`         | `<>`                          | Dumps out program headers information.                                        |
| `-dump_sections`         | `<>`                          | Dumps out section headers information.                                        |
| `-dump_symbols`          | `<>`                          | Dumps out all symbols information.                                            |
| `-dump_sym_source_files` | `<>`                          | Dumps out all symbols that are tagged as source files.                        |
| `-dump_sym_data`         | `<>`                          | Dumps out all symbols that are tagged as data.                                |
| `-dump_sym_funcs`        | `<>`                          | Dumps out all symbols that are tagged as functions.                           |
| `-dump_notes`            | `<>`                          | Dumps out all notes.                                                          |
| `-log_sym_all`           | `<>`                          | Logs out all symbols.                                                         |
| `-log_sym_source_files`  | `<>`                          | Logs out all symbols that are tagged as source files.                         |
| `-log_sym_data`          | `<>`                          | Logs out all symbols that are tagged as object data.                          |
| `-log_sym_funcs`         | `<>`                          | Logs out all symbols that are tagged as functions.                            |

# :notebook_with_decorative_cover: To-Do

- I would really like to dive into the **DWARF** stuff, however, so far I didn't quite get the chance to follow it's standard since the documentation is poor and looking into binutils is very hard and frustrating, since the code is complicated. So, in the future.
- Implement a GCC demangler that would demangle the function names.