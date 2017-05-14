# Configuration Instructions

1. [Requirements Installation](#1-requirements-installation)
1. [Code Compilation](#2-code-compilation)
1. [Game Run](#3-game-run)

## 1. Requirements Installation

To install the game requirements, you can choose one of these methods:

1. [Vagrant installation](#11-vagrant-installation)
1. [Manual installation](#12-manual-installation)

### 1.1. Vagrant Installation

#### Step 1: Install Vagrant

Access the following link to select proper package for your operating system and architecture. The are packages available to Windows, Debian-based systems, CentOS and MAC OS X:

    https://www.vagrantup.com/downloads.html

#### Step 2: Clone repository

After completing the installation of Vagrant, you can clone the project with the **clone** command:

```$ git clone https://github.com/LManaslu/Wenova.git```

#### Step 3: Going up Virtual Machine

Access the path of the cloned repository:

```$ cd Wenova```

At this path, has a Vagrantfile, the file with all configurations for Virtual Machine that can run the project.

To start the VM you need to do the **up** command:

```$ vagrant up```

 The first time will take several minutes.

#### Step 4: Access Virtual Machine

```$ vagrant ssh```

In the VM, the path that you clone project is synced with ```/home/vagrant/Wenova```

And when you need to go out from VM you can do this with **logout** command:

```$ logout```

In the next time, to run the VM, you'll need to do the steps **3** and **4** again.

### 1.2. Manual Installation

If you prefer, you can install the game requirements manually, using bash commands.

#### Step 1: Clone repository

```$ git clone https://github.com/LManaslu/Wenova.git```

####  Step 2: Install C++ compiler

```$ sudo apt-get install g++```

#### Step 3: Install SDL libraries

```$ sudo apt-get install libsdl2-2.0-0 libsdl2-dev libsdl2-image-2.0-0 libsdl2-image-dev libsdl2-ttf-2.0-0 libsdl2-ttf-dev```

## 2. Code Compilation

To compile game code, inside game folder, you can use Makefile available in the repository, there are several **make** commands available:

1. [Compile](#21-compile)
1. [Clean object directory](#22-clean-object-directory)
1. [Clean object directory and delete game executable](#23-clean-object-directory-and-delete-game-executable)

### 2.1. Compile

To compile all sources files (**.cpp**) from **src/** folder and headers files (**.h**) from **include/** folder, use **make** command:

```$ make```

Optionally, for a faster compilation, you can use the **-j** flag:

```$ make -j```

### 2.2. Clean object directory

To remove all **.d** and **.o** files from **obj/** folder, use **clean** command:

```$ make clean```

### 2.3. Clean object directory and delete game executable

To remove all **.d** and **.o** files from **obj/** folder and remove the binary executable from **bin/** folder, use **dist-clean** command:

```$ make dist-clean```


## 3. Game Execution

To run the compiled game, you can use the run command:

```$ make run```
