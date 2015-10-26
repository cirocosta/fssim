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

## LICENSE

MPLv2. See `LICENSE`.

