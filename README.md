# TIAD - Tetris in a Day
Tetris clone made in a day (about 12hrs).

The platform and rendering code I copied from another project, so those don't count.

# How to build?
The project is way too simple for fancy build systems. It's build from the command line. 

Here is how to do it:

 - Make sure you have MSVC installed and you're in luck as it comes with Visual Studio.

 - Open the cp.bat in your favorite text editor. Replace the path in there with where every our "vcvars64.bat" is located. It's likely the same, but it's a good idea to check. "vcvars64.bat" Enables running the compiler/linker from any directory.

 - Save and close cp.bat, then run it! It will open a command prompt (with access to the compiler) in the project directory.

 - Then in that command prompt input the command "build" or "optbuild" for some o2 fun.

 - If you want to open it in visual studio in then run the Debugger.bat (you need to build before this works)
