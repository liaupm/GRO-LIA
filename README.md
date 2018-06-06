# README #

This version of GRO was developed at the LIA-UPM lab atop the original version, created by Eric Klavins at University of Washington.

### What is this repository for? ###

* This repository holds the latest developer's version of GRO at LIA-UPM. There is an executable version for Windows, and the source code can be compiled on MacOS, Linux or Windows.
* The current version is 1.0


### How do I get set up? ###

[//]: # (* Summary of set up)
* Setup - Download and decompress or clone the repository. If you want to use the Windows executable version, work from the WIN-Executable subfolder and follow the instructions in the Readme in that folder. Otherwise, if you want to compile, for all OSs make sure the directory structure is created and available. There is a script that creates this structure and that can be executed: `create_dirs.sh` (MacOS and Linux). This script also works under Windows, but should be renamed to `create_dirs.bat`.   

[//]: # (* Configuration)
* Dependencies - Under any OS, the latest version of QT Creator (https://www.qt.io/ide/) should be installed. Flex (https://github.com/westes/flex) and Bison (https://www.gnu.org/software/bison/) should also be installed. Windows versions may be found at https://sourceforge.net/projects/winflexbison/. A C/C++ compiler needs to be installed. This version has been tested with clang/clang++ in MacOS, gcc/g++ in Linux (CentOS 7.0) and MinGW in Windows. 

[//]: # (* Database configuration)
[//]: # (* How to run tests)
[//]: # (* Deployment instructions)
* Instructions for compiling - 1. Run `create_dirs.sh` (from the project directory) 2. From the same project directory run the script for compiling all modules: `modules.sh <mode>` (MacOS and Linux) or `modules-win.bat <mode>` (Windows). `<mode>` is either `debug` or `release`. 3. Open QT Creator and load both projects `ccl-master` and `gro-master`. Set the correct output of the projects to `ccl_Debug`, `ccl_Release`, `gro_Debug` and `gro_Release` respectively when setting up the projects in QT Creator. 4. Build `ccl-master`. 5. Build `gro-master`. 6. Upon execution of gro, check the bottom line of the window for the Working Directory (a guide is shown in the Readme in the WIN-Executable folder). Copy `standard.gro` and `gro.gro` from the `gro-master/include` folder to the Working Directory.

### Contribution guidelines ###

[//]: # (* Writing tests)
[//]: # (* Code review)
[//]: # (* Other guidelines)
* If you find any bugs or wish to contribute, please contact martin.gutierrez@gmail.com

### Who do I talk to? ###

[//]: # (* Repo owner or admin)
[//]: # (* Other community or team contact)
* For more information, please direct your questions to martin.gutierrez@gmail.com or www.lia.upm.es.
