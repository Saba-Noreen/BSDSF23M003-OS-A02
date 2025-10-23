## Feature 2: ls-v1.1.0 – Long Listing Format (-l)

**Q1. Difference between stat() and lstat():**  
- `stat()` retrieves metadata for a file. If the file is a **symbolic link**, it returns the metadata of the **target file** it points to.  
- `lstat()` retrieves metadata for a file as well, but if the file is a **symbolic link**, it returns the metadata of the **link itself**, without following it.  
- In the context of `ls -l`, using `lstat()` is more appropriate when we want to display information about symbolic links instead of following them to their target.

**Q2. Extracting file type and permissions from st_mode:**  
- The `st_mode` field in `struct stat` is an integer containing **both the file type and permission bits**.  
- To determine **file type**, use macros like:
  - `S_ISDIR(st.st_mode)` → directory  
  - `S_ISREG(st.st_mode)` → regular file  
  - `S_ISLNK(st.st_mode)` → symbolic link  
- To determine **permissions**, use **bitwise AND** with predefined constants:
  - Owner: `S_IRUSR` (read), `S_IWUSR` (write), `S_IXUSR` (execute)  
  - Group: `S_IRGRP`, `S_IWGRP`, `S_IXGRP`  
  - Others: `S_IROTH`, `S_IWOTH`, `S_IXOTH`  
- Combine these checks to print `rwx` style permissions for each file.


