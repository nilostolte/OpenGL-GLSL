# OpenGL and GLSL shaders

Examples (OpenGL and GLSL shaders) and  complete OpenGL installation with w64devkit development kit, glad, glfw3 binaries, and Ninja building tool.

The objective of this repository is to simplify the development of OpenGL applications in C and C++. The advantage
of using w64devkit is that it's a development kit that is portable, that is, that can be used just copying
the files to any device in a Windows PC (an SD card, for example). It does not require installation and bypasses
the registry.

The development kit is used in normal command windows. To avoid typing and to simplify the overall development
process the programs are built using [Ninja](https://ninja-build.org/) building tool. Ninja is a very simple
buiding tool that substitute makefiles with several advantages, particularly of being very efficient.

In this implementation GLFW and Glad are used, and all [exemples](https://github.com/nilostolte/OpenGL/tree/main/glfw-3.3.8/examples) are to be built with C compiler using this configuration. The examples are located inside [glfw-3.3.8](https://github.com/nilostolte/OpenGL/tree/main/glfw-3.3.8) directory, because they use GLFW library. The first two examples were dowloaded from the web and modified for the installed configuration. The third example is a C++ example rewritten in C, with objective to simplify GLSL shader programming. For that a vertex shader with two triangles forming a rectangle (quad) is implemented to be used to display fragment shaders inside. 

In [shaders](https://github.com/nilostolte/OpenGL/tree/main/glfw-3.3.8/examples/shaders) directory, the examples use a modified version of the third example. For example, the rectangle (quad) is expanded to the size of the viewport in order to show the fragment shaders in full window. The objective here is to implement in C, fragment shaders previously developed using web shader editors.

The fragment shaders developed on [Shadertoy](https://www.shadertoy.com/) online shader editor can be used here with only few modifications.

## How to Start

Download this repository, for example, in the zip format and copy its content into an Explorer window located
at the place chose to store the development kit as well as the other tools to be used. Keep this Explorer window
opened for further use below. This repository contain some empty directories that are placeholders for the software 
to be dowloaded as follows.

Alternatively, place the development kit somewhere else. For that, after copying this repository somewhere in the hard drive, just delete [w64devkit](https://github.com/nilostolte/OpenGL/tree/main/w64devkit) directory, and set the environment path to the complete path of "w64devkit/bin." 

## Installing w64devkit

This is the development kit allowing to develop C/C++ projects in Windows 64, without needing to install any Windows
tools. The advantage of this kit is its simplicity, allowing to perfectly understand the main dependencies involved
without the need of setting paths to libraries or include files.

One can download it 
[here](https://github.com/skeeto/w64devkit/releases). Choose a release (here it's v1.18.0) and click on the link:

```
    w64devkit-1.18.0.zip
```
Delete the directory `w64devkit`, download the version desired and copy `w64devkit` directory at its previous positon,
**or** in some other place of choice.

For the sake of simplification, one should set the path in order to allow the kit to be visible in a command shell
and accessible via Ninja building tool.

For that, one needs to go to `Control Panel > System > About > Advanced System Settings`, select `Environment Variables` 
on `System Properties` window. In `User variables for Administrator` click on `Path` and `Edit...`. In the new window 
`Edit environment variable` click `New`. In the Explorer window where `w64devkit` was copied click twice on `w64devkit`, 
and click twice on `bin`. Now copy the path shown on the top of the Explorer window and paste it in the window 
`Edit environment variable`, by clicking on `New` again and pasting. Press `OK` in this window, in `Environment Variables` window, as well as in `System Properties`.

The development kit is now installed and accessible in command shell windows, such as in a Windows Powershell

## Installing Ninja

Find the latest release of Ninja [here](https://github.com/ninja-build/ninja/releases) and click on:

```
    ninja-win.zip
```

Copy `ninja.exe` from the zip file to the `tools` directory

Now set a new path to the `tools` directory. 
Go to `Control Panel > System > About > Advanced System Settings`, select `Environment Variables` 
on `System Properties` window. In `User variables for Administrator` click on `Path` and `Edit...`. In the new window 
`Edit environment variable` click `New`. In the Explorer window where `ninja.exe` was copied copy the path shown on the 
top of the Explorer window and paste it in the window `Edit environment variable`, by clicking on `New` again and pasting. 
Press `OK` in this window, in `Environment Variables` window, as well as in `System Properties`.

Now Ninja is visible to any command shell window

## Installing glfw-3.3.8

This library is already installed in this repository for convenience. The whole package can be downloaded
[here](https://www.glfw.org/download.html) and clicking on `64-bit Windows binaries`. If a new version of
glfw is required than the whole package should be reinstalled.

For simplicity, the installation of glfw-3.3.8 as provided in this repository is explained as follows.

### Installing Glad

Glad is an OpenGL library loader and it's used here as well as in other open source programs.

Glad is already provided in this repository. All we need is to compile it. For that, go to `glad` subdirectory and
in a command shell opened in this location, just type:

```
    ninja
```

This will read the file `build.ninja` which gives the details on how to compile Glad. It will generate
the file `glad.o` that will be needed to be linked to any OpenGL program using the approach shown here.

### Compiling the Examples

Returning to the `glfw-3.3.8` directory, one should now be able to compile the examples. For that, open
a command shell in this location, and just type:

```
    ninja
```

Example 2 is a modified version of the source code shown [here](https://www.glfw.org/docs/latest/quick_guide.html). It
uses the library `linmath.h`, which is all contained in that include file (functions are `#define` macros). This include 
file is located [here](https://github.com/nilostolte/OpenGL/tree/main/glfw-3.3.8/include).

### Executing the Examples

One can now execute the examples by typing in the command shell:

```
    ./example1
```
Or by typing 

```
    ./example
```

Followed by the example number, as many as there are available.

And this tests the installation.

In addition to these examples, Ninja will build additional examples found in [shaders](https://github.com/nilostolte/OpenGL/tree/main/glfw-3.3.8/examples/shaders) directory. These examples have more specific names as can be seen from the executable filenames. These are called in a similar way as explained above.

These additional examples are for GLSL shader development, as a way to help implementing in C language, GLSL fragment shaders previously developed in online GLSL shader editors such as [Shadertoy](https://www.shadertoy.com/).

## Changing the Installation or Installing a New Version of glfw

As explained above the whole glfw package can be downloaded [here](https://www.glfw.org/download.html) and clicking on 
`64-bit Windows binaries`. One can copy the new version in a new directory at the level of the directories `w64devkit`
and `tools`. 

The directory `glfw-3.3.8` should be conserved at least for conserving a clean copy until the new installation is
working fine. Once the new glfw directory is copied, one should then copy all necessary files from `glfw-3.3.8` to
this new directory. These are the example source files, `build.ninja` and the entire `glad` directory. For example2
one should also copy [linmath.h](https://github.com/nilostolte/OpenGL/tree/main/glfw-3.3.8/include).

In principle the `build.ninja` doen't need to be modified, since there are no instances of `glfw-3.3.8` in it.

If one needs to change the locations of the different parts of this installation, then the `build.ninja` and
the different paths might be required to be changed, accordingly.

As can be seen, `build.ninja` files are quite simple to understand and modified.

## Changing Glad

Glad will also be required to change in certain cases. This can be done by [generating the file](https://gen.glad.sh/)
based on the official specs. It's set to C/C++ language by default. The `gl` version used here was 4.2 and we should 
make sure to also choose `core` compatibility. The Option `Generate a loader` also has to be checked. By clicking on 
`Generate` one is presented with a zip file containing the directoties to be copied to `glad` directory.

