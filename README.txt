SBUNIX: CSE 506 Project
-----------------------
Cooperative multi-tasking w/ all functional requirements (50 points)

Team:
-----
Ankit Aggarwal (anaaggarwal) (SBU ID: 111485578)
Saraj Munjal (smunjal) (SBU ID: 111497962)


OS Functionality:
-----------------
Memory Subsystem (page descriptors, free list, page tables, kmalloc)
Process Subsystem (kernel threads, context switch)
User-level Subsystem (VMAs/vm_map_entrys, switch to ring 3, page faults)
I/O subsystem (syscalls, terminals, VFS, tarfs file access, elf loading)
kprintf, IDT, keyboard, timer
Auto growing stack and dynamic heap allocation
Shell with PATH, PS1, cd, and &
Script handling with shebang (#!)


Shell support:
--------------
cd: change current working directory
echo <input>: print the given input
export PATH=<_path>: update path variable with _path
export PS1=<_ps1>: update PS1 variable with _ps1
ls: list the current directory contents
ps: list all process in the OS
sleep: sleep for given input time in seconds
kill -9: kills the process with given pid
cat <file>: prints the given file content
./<binary_file>: execution of binary file with name binary_file
<command> &: execution of background process


Build:
------
make clean
make
qemu-system-x86_64 -curses -drive id=boot,format=raw,file=$USER.img,if=none -drive id=data,format=raw,file=$USER-data.img,if=none -device ahci,id=ahci -device ide-drive,drive=boot,bus=ahci.0 -device ide-drive,drive=data,bus=ahci.1 -gdb tcp::9999


Examples:
---------
ls
cat etc/test.txt
./etc/test.sbush
sbush etc/test.sbush
ls etc
ps
echo hello
export PS1=SHELL


Syscalls Implemented:
---------------------
chdir
close
closedir
execvpe
exit
fork
free
getcwd
getpid
getppid
kill
malloc
open
opendir
ps
read
readdir
sleep
wait
waitpid
write
yield
-----
gets
printf
putchar
puts


References:
-----------
OSDev Wiki (http://wiki.osdev.org/Main_Page)
Stackoverflow (https://stackoverflow.com)
AskUbuntu (https://askubuntu.com)
JamesM's kernel development tutorials (http://www.jamesmolloy.co.uk/tutorial_html)


Known Limitations
----------------------
1. .. and . is not handled in chdir syscall
2. Add init process (optional)
