# OpenGL

Complete Windows C/C++ development environment with OpenGL installation using w64devkit, glad and glfw3 binaries.

The objective of this repository is to simplify the development of OpenGL applications in C and C++. The advantage
of using w64devkit is that it's a development kit that is portable, that is, that can be used just copying
the files to any device in a Windows PC (an SD card, for example). It does not require installation and bypasses
the registry.

The development kit is used in normal command windows. To avoid typing and to simplify the overall development
process the programs are built using [Ninja](https://ninja-build.org/) building tool. Ninja is a very simple
buiding tool that substitute makefiles with several advantages, particularly of being very efficient.

## Installing w64devkit

This is the development kit allowing to develop projects in C/C++.

One can download it 
[here](https://github.com/skeeto/w64devkit/releases). Choose a release (here it's v1.18.0) and click on the link:

```
    w64devkit-1.18.0.zip
```

This installation is only necessary if one wants to use a more up to date kit version. In this case, delete the directory
`w64devkit`, download the version desired and copy `w64devkit` directory at the desired root directory.

If the version 1.18.0 is satisfactory, it's already included dowloading this directory.


