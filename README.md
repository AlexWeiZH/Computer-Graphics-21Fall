
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
![image](https://github.com/AlexWeiZH/Computer-Graphics-21Fall/assets/98062338/ff0214aa-7a61-4925-8e56-903f34ce950d)
![image](https://github.com/AlexWeiZH/Computer-Graphics-21Fall/assets/98062338/24bc0810-7db7-48aa-abce-e46b5c2a51e6)
![image](https://github.com/AlexWeiZH/Computer-Graphics-21Fall/assets/98062338/75dffe77-c284-446c-95ad-f0aaa720557b)
![image](https://github.com/AlexWeiZH/Computer-Graphics-21Fall/assets/98062338/94b4b0e3-56d8-43ea-95fc-dc397ffe06fd)
![image](https://github.com/AlexWeiZH/Computer-Graphics-21Fall/assets/98062338/6ea0b484-b7e2-44f9-985f-00d737a289d2)
![image](https://github.com/AlexWeiZH/Computer-Graphics-21Fall/assets/98062338/a045c068-cd01-4f25-9a06-b94b85800e63)
![image](https://github.com/AlexWeiZH/Computer-Graphics-21Fall/assets/98062338/5b4192e8-5581-42ae-bc85-67c335aa8146)
![image](https://github.com/AlexWeiZH/Computer-Graphics-21Fall/assets/98062338/8643b30d-646d-4ab1-9bb7-6a2d81834682)


