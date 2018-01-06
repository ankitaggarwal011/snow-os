# SNOW OS: 64-bit cooperative multi-tasking OS
Snow OS is a 64-bit cooperative multi-tasking operating system implemented in C. In short, it supports memory management, paging, context switching, terminal, shell and file management for tarfs.

The project was completed as a part of the graduate course [CSE 506 Operating Systems](https://compas.cs.stonybrook.edu/courses/cse506-operating-systems-fall-17/) by Prof. Mike Ferdman at Stony Brook University in Fall '17.


## OS Functionality
- Memory Subsystem (page descriptors, free list, virtual memory, page tables, kmalloc)  
- Process Subsystem (kernel threads, context switch)  
- User-level Subsystem (VMAs/vm_map_entrys, switch to ring 3, page faults)  
- COW fork(), auto-growing stack, dynamic heap allocation, graceful SEGV  
- I/O subsystem (syscalls, terminals, VFS, tarfs file access, elf loading)  
- tarfs: open, read, close, opendir, readdir, closedir  
- Terminal: read(stdin), write(stdout), write(stderr)  
- Virtual File System  
- kprintf, IDT, keyboard, timer interrupts  
- Shell with PATH, PS1, cd, and &  
- Script handling with shebang (#!)  
- bin/init user process to launch shell using /etc/rc  
- Reaper/Idle process to cleanup zombie processes  
- Reused PIDs  
- Binaries implemented: init, echo, sleep, cat, ls, kill -9, ps  
- Background processes  
- Runs its own Shell  

## Shell support
```
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
```

## Build
```
make clean  
make  
qemu-system-x86_64 -curses -drive id=boot,format=raw,file=$USER.img,if=none -drive id=data,format=raw,file=$USER-data.img,if=none -device ahci,id=ahci -device ide-drive,drive=boot,bus=ahci.0 -device ide-drive,drive=data,bus=ahci.1 -gdb tcp::9999  
```

## Examples
```
ls  
cat /etc/test.txt  
.//etc/test.sbush  
sbush /etc/test.sbush  
ls /etc  
ps  
echo hello  
export PS1=SHELL  
```

## Example files
Under the rootfs/etc/ folder, the following test files can be found:  

- test: test directory for cd  
- test.txt: simple text file for cat  
- test.sbush: simple sbush script  
- stress_test.sbush: stress test sbush script with 60+ commands to spawn 60+ processes, system remains stable  

## Syscalls Implemented
```
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
```

## License
[MIT License](https://github.com/ankitaggarwal011/snow-os/blob/master/LICENSE)

## Authors
Ankit Aggarwal  
Saraj Munjal  
