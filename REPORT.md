## Feature 2 – Long Listing (-l)

**Q1. What is the difference between `stat()` and `lstat()`?**  
`stat()` returns information about the target file, while `lstat()` returns information about the link itself if the file is a symbolic link.  
`ls -l` uses `lstat()` so that symbolic links are listed correctly.

**Q2. Explain the field `st_mode` and how bitwise operators are used.**  
`st_mode` stores file type and permission bits.  
Macros such as `S_IFDIR`, `S_IFREG`, `S_IRUSR`, `S_IWUSR`, etc., are used with bitwise AND (`&`) to test each bit.  
Example:  
```c
if (info.st_mode & S_IRUSR) printf("r");

