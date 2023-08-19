## This project is based on Visual Studio 2019, glfw-3.3.2 and glew-2.1.0-win32 , to run this program, first, make sure you have GLFW and GLEW installed and then:

1.In vs2019, use the "x64" solution platform and "Release" to build the project, which is next to the "Debug" button at the top of the screen.
2.In program->property, in the top right position, select the "platform:" to be "All platforms".
3.In program->property->VC++ directories, on the right side, under "General", select the "include Directories" to be the following path: project folder->Libraries->include.
4.Still, in program->property->VC++ directories, under the previous one, select the "library Directories"to be the path: project folder->Libraries->lib.
5.In program->property->Linker->input, in the "Additional Dependencies" option, edit it, and type in the following:

opengl32.lib
glew32.lib
glew32s.lib
glfw3.lib
glfw3dll.lib

6.In program->property->C/C++ ->running library, select"MultiThread DLL(/MD)".
7.then click ok.
8.If the error "cannot find file glew32.dll" is pumped up, go to project foder->Libraries->bin, copy the file "glew32.dll" into this folder: C:\Windows\System32, if that doesn't work, try copy into this folder: C:\Windows\SysWOW64
## This is what it looks like:
![RayTracer](https://github.com/AlexWeiZH/Computer-Graphics-21Fall/assets/98062338/7a682f2f-1915-4209-a167-340e4ada6e13)

## (I know it looked bad, but consider its my first CG program, it is actually acceptable :) )
