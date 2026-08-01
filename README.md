<h1 align="center"> mynix version 2 </h1>
<h3 align="center"> Second Version of the mynix operating system </h3>

<h4>Information:</h4>
Created by JM-Pilot.

This operating system using the limine boot protocol and its own kernel

<h4>Compiling and Building</h4>
To compile you must have the required tools:

```sh
x86_64-elf-gcc
nasm

# optional tools
xorriso
wget
git
qemu
```

after getting all those tools, you can simply run:

```sh
$ make kernel   # to compile the kernel
$ make iso      # to make the iso
```

or if you want to run it via qemu:

```sh
$ make run      # for bios
$ make run-efi 	# for UEFI
```

type `$ make help` for more information about building.


<h4>Check out other projects</h4>

[mynix v1](https://github.com/JM-Pilot/mynix)

[C-kernel-OS](https://github.com/orca-pet3910YT/C-kernel-OS)