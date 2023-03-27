# OpenGL

Complete Windows 64 C/C++ development environment with OpenGL installation using w64devkit, glad and glfw3 binaries.

The objective of this repository is to simplify the development of OpenGL applications in C and C++. The advantage
of using w64devkit is that it's a development kit that is portable, that is, that can be used just copying
the files to any device in a Windows PC (an SD card, for example). It does not require installation and bypasses
the registry.

The development kit is used in normal command windows. To avoid typing and to simplify the overall development
process the programs are built using [Ninja](https://ninja-build.org/) building tool. Ninja is a very simple
buiding tool that substitute makefiles with several advantages, particularly of being very efficient.

## How to Start

Download this repository, for example, in the zip format and copy its content into an Explorer window located
at the place chose to store the development kit as well as the other tools to be used. Keep this Explorer window
opened for further use below. This repository contain some empty directories that are placeholders for the software 
to be dowloaded as follows.

## Installing w64devkit

This is the development kit allowing to develop C/C++ projects in Windows 64, without needing to install any Windows
tools. The advantage of this kit is it's simplicity, allowing to perfectly understand the main dependencies involved
without the need of setting paths to libraries or include files.

One can download it 
[here](https://github.com/skeeto/w64devkit/releases). Choose a release (here it's v1.18.0) and click on the link:

```
    w64devkit-1.18.0.zip
```
Delete the directory `w64devkit`, download the version desired and copy `w64devkit` directory at its previous positon,
**or** copy the contents of `w64devkit` into the existing `w64devkit` directory.

For the sake of simplification, one should set the path in order to allow the kit to be visible in a command shell
and accessible via Ninja building tool.

For that, one needs to go to `Control Panel > System > About > Advanced System Settings`, select `Environment Variables` 
on `System Properties` window. In `User variables for Administrator` click on `Path` and `Edit...`. In the new window 
`Edit environment variable` click `New`. In the Explorer window where `w64devkit` was copied click twice on `w64devkit`, 
and click twice on `bin`. Now copy the path shown on the top of the Explorer window and paste it in the window 
`Edit environment variable`, by clicking on `New` again and pasting. Press `OK` in this window, in `Environment Variables` 
window, as well as in `System Properties`.

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



