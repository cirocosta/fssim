# fssim

> A Filesystem simulator in C

<!-- START doctoc generated TOC please keep comment here to allow auto update -->
<!-- DON'T EDIT THIS SECTION, INSTEAD RE-RUN doctoc TO UPDATE -->


- [Running](#running)
- [Inner Workings](#inner-workings)
  - [FS](#fs)
  - [Flow](#flow)
  - [File Attributes](#file-attributes)
- [Cli](#cli)
- [Block device Structure](#block-device-structure)
- [LICENSE](#license)

<!-- END doctoc generated TOC please keep comment here to allow auto update -->

## Running

This project depends on `cmake` for generating Make files and asserting the required libs (currently depends on [gnu readline](https://cnswww.cns.cwru.edu/php/chet/readline/rltop.html) only).

```sh
$ mkdir build && cd $_
$ cmake ..
$ ./fssim
```

If you wish to test the project as well you might be interested on having the verbose output when a test fails. To enable such feature, add the following variable to your environment and run `cmake` w/ `Debug` set up:

```
$ export CTEST_OUTPUT_ON_FAILURE=1
$ cmake -DCMAKE_BUILD_TYPE=Debug ..
```

## Inner Workings

### FS

The filesystem represents a unique partition of a disk without the booting related stuff that a normal partition (in a normal disk) would have. It supports up to 100MB of both regular files and metadata. Its blocks are of 4KB each (being the block the granularity).

It's storage is done using FAT (File Allocation Table), having the root at `/` and using `/` as the hierarchy separator character. Free storage management is implemented using bitmapping. 

Each directory is a list with an entry for each file inside the directory.

### Flow

```
filepath
  |
  |      (dir struct)                     (f_index struct)
  |     .-----------.                    .----------------.
  '---> |           |                    |                |
        |           |                    +----------------+
        |           |              .---> |  &data_blocks  |
        +-----------+              |     +----------------+
        |    FILE   |---(file_no)--'     |                |
        +-----------+                    |                |
        |___________|                    |                |
                                         |________________|


    |  path resolution  |             |   blocks reading    |
    '-------------------'             '---------------------'
      happens at open()                   
           time
  
``` 

- `touch`: creates a file with no data, setting some attributes and adding a reference to it in the containing directory.
- `rm`: frees space, removes entry in the directory as well as fat
- `open`: 

### File Attributes

Simulated files or directories (which are files) contain:
-   name
-   size in bytes (except for directories)
-   creation time
-   modif time
-   last access time
-   data

These attributes are stuck with file entries in directory files.

## Cli

```sh

$ ./fssim -h

USAGE: `$ ./fssim`
  Starts a prompt which accepts the following commands:

COMMANDS:
  mount <fname>         mounts the fs in the given <fname>. In
                        case <fname> already exists, countinues
                        from where it stopped.

  cp <src> <dest>       copies a file from the real system to the
                        simulated filesystem (dest).

  mkdir <dir>           creates a directory named <dir>

  rmdir <dir>           removes the directory <dir> along with the
                        files that were there (ir any)

  cat <fname>           shows (stdout) the contents of <fname>

  touch <fname>         creates a file <fname>. If it already exists,
                        updates the file's last access time.

  rm <fname>            remover a given <fname>

  ls <dir>              lists the files and directories 'below'
                        <dir>, showing $name, $size, $last_mod for
                        files and an indicator for directories.
  
  find <dir> <fname>    taking <dir> as root, recursively searches
                        for a file named <fname>

  df                    shows filesystem info, including number of
                        diferectories, files, freespace and wasted
                        space

  unmount               unmounts the current filesystem

  help                  shows this message

  sai                   exits the simulator


NOTES:
  in the whole simulation all filenames must be specified as 
  absolute paths.

AUTHOR:
  Written by Ciro S. Costa <ciro dot costa at usp dot br>.
  See `https://github.com/cirocosta/fssim`. Licensed with MPLv2.
```

## Block device Structure

### Overview

```
  (size in B)           
------------------                         --
4                |  block size             |
------------------                         | Super Block
4                |  n = number of blocks   |
------------------                         --
                 |                         |
n*4              |  FAT                    |  File Alloc
                 |                         |  Table and
------------------                         |  Free Space
                 |                         |  Management
((n-1)/8|0)+1)   |  BMP                    |
                 |                         |
------------------                         --
                 |                         |
4KB              |  Block 0                | 
                 |                         | 
------------------                         | 
(...)                                      | Files and 
(...)                                      | its 
------------------                         | payload
                 |                         |
4KB              |  Block N                |
                 |                         |
------------------                         --
```

As we're dealing with >1 byte numbers we have to also care about endianess (as computer  do not agree on MSB). Don't forget to use `htonl` and `ntohl` when (de)serializing numbers from the block char (we're always going with uint32_t, which is fine).

## Files

Files are implemented as an unstructured sequence of bytes. Each file is indexed by the location of its first block in the FAT and might have any size up to disk limit and with a 'floor' of 4KB (given that we're using a granularity of `1 Block = 4096 Bytes`).

## Directories

Directory blocks are well structured:

```c
struct directory_entry {
  uint8_t is_dir;   // bool: directory or not
  char fname[11];   // filename
  uint32_t fblock;  // first block in FAT
  uint32_t ctime;   // creation time
  uint32_t mtime;   // last modified
  uint32_t atime;   // last access
};

```

Being serialized into:
```
/--------------- 32 Bytes ------------------------/     

                                                        
   1B       11B      4B       4B      4B      4B        
+-------------------------------------------------+     --
| is_dir | fname | fblock | ctime | mtime | atime |     |    0
+-------------------------------------------------+     --
                                                        |  
                      (...)                             |   (..)
                                                        |
+-------------------------------------------------+     --
| is_dir | fname | fblock | ctime | mtime | atime |     |   (127)
+-------------------------------------------------+     --
``` 

As a design decision, directories must fit in a single block (4KB) only. This limits each layer in the hierarchy tree to support a maximum of 128 entries (files or directories). Nested directories are limited to disk limit.


## LICENSE

MPLv2. See `LICENSE`.

