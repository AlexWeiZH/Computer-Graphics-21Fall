
## This project is based on Visual Studio 2019, glfw-3.3.2 and glew-2.1.0-win32 , to run this program, first, make sure you have GLFW and GLEW & GLM installed and then:<br />

1.  In vs2019, use the "x64" solution platform and "Release" to build the project, which is next to the "Debug" button at the top of the screen.<br />
2.  In program->property, in the top right position, select the "platform:" to be "All platforms".<br />
3.  In program->property->VC++ directories, on the right side, under "General", select the "include Directories" to be the following path: project folder->Libraries->include.<br />
4.  Still, in program->property->VC++ directories, under the previous one, select the "library Directories"to be the path: project folder->Libraries->lib.<br />
5.  In program->property->Linker->input, in the "Additional Dependencies" option, edit it, and type in the following:<br />

&nbsp;&nbsp;&nbsp;&nbsp;     opengl32.lib <br />
&nbsp;&nbsp;&nbsp;&nbsp;      glew32.lib  <br />
&nbsp;&nbsp;&nbsp;&nbsp;      glew32s.lib  <br />
&nbsp;&nbsp;&nbsp;&nbsp;      glfw3.lib  <br />
&nbsp;&nbsp;&nbsp;&nbsp;      glfw3dll.lib  <br />

6.  In program->property->C/C++ ->running library, select"MultiThread DLL(/MD)".<br />
7.  lick apply & ok.<br />
8.  If the error "cannot find file glew32.dll" is pumped up, go to project foder->Libraries->bin, copy the file "glew32.dll" into this folder: C:\Windows\System32, if that doesn't work, try copy into this folder: C:\Windows\SysWOW64<br />
### By changing certain codes, this will be what it looks like:
![image](https://github.com/AlexWeiZH/Computer-Graphics-21Fall/assets/98062338/f69bb252-c6bb-415d-bc49-a2b22c2635c2)
