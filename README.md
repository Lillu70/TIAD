# TIAD - Tetris in a Day
Tetris clone made in a day (about 12hrs).

The platform and rendering code I copied from another project, so those don't count.

# How to build?
The project is way too simple for fancy build systems. No Ninjas required. It uses a single translation unit "Unity Build" and It's build from the command line.

Here is how to do it:

 - Make sure you have MSVC installed and you're in luck as it comes with Visual Studio.

 There are couple of ways to actually get Microsofts compiler to work from any directory:
 - 1) Option A) Simply search for "x64 Native Tools Command Prompt" Your instaled version should popup. Run that.
   Here is MSDN link that explains it the procedure: https://learn.microsoft.com/en-us/cpp/build/how-to-enable-a-64-bit-visual-cpp-toolset-on-the-command-line?view=msvc-170

 - 1) Option B) Open the cp.bat in your favorite text editor. Replace the path in there with where every our "vcvars64.bat" is located. It's likely the same, but it's a good idea to check. "vcvars64.bat" Enables running the compiler/linker from any directory. Save and close cp.bat, then run it! It will open a command prompt (with access to the compiler) in the project directory.

 - 1) Option C) Manually add the required enviroment variables, I only recomend doing this if you enjoy pain.

 - 2) How ever you got the complier to work, navigate your command prompt to the where ever the repo was cloned. If you see Build.bat you're in the right directory.

 - 3) Run said "Build.bat" or "Optbuild.bat" for some o2 fun. This will create a new folder called "bin" that will house the executable.

 - If you want to open it in visual studio in then run the Debugger.bat (you need to build before this works)
