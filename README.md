# OpenGL Exercises
This repository contains the code for all the [OpenGL exercises](https://learnopengl.com/) required for the masters degree's graphics programming course at UniVr.

# ENVIRONMENT
All of the tutorials where developed on Ubuntu 22.04.2 LTS

# PACKAGES
All used packages (GLAD, ASSIMP, GLM and STBI_IMAGE) are contained in the lib folder. Each one of them is compiled from source code as shared library via CMake.

# DOWNLOAD THE REPOSITORY
```bash
$ sudo apt install git
$ git clone 
```
# DEPENDENCIES
In a fresh installed Ubuntu 22.04.2 distributions, you will need to install:
```bash
$ sudo apt install git
$ git clone git
```
then clone the repository
# HOW TO RUN EACH PROJECT
Navigate to the project's ```main.cpp```:
```bash
$ cd ~/OpenGLExercises/12-LigthCasters/1-DirectionalLigth
```
create build folder and navigate to it:
```bash
$ mkdir build
$ cd build
```
run cmake:
```bash
$ cmake ..
```
compile with ```-j $(nproc)``` option to speed up the compilation process (otherwise just use ```make``` and you're fine):
```bash
$ make -j $(nproc)
```
and finally execute:
```bash
$ ./ligthCasters
```
