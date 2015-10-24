# fssim

> A Filesystem simulator in C

<!-- START doctoc generated TOC please keep comment here to allow auto update -->
<!-- DON'T EDIT THIS SECTION, INSTEAD RE-RUN doctoc TO UPDATE -->
**Table of Contents**  *generated with [DocToc](https://github.com/thlorenz/doctoc)*

- [Inner Workings](#inner-workings)
- [Cli](#cli)
- [LICENSE](#license)

<!-- END doctoc generated TOC please keep comment here to allow auto update -->

## Inner Workings

Simulated files or directories contain:
-   name
-   size in bytes (except for directories)
-   creation time
-   modif time
-   last access time
-   data


The filesystem represts a unique partition of a disk without the booting related stuff that a normal partition would have. It supports up to 100MB of both regular files and metadata. Its blocks are of 4KB each.

It's storage is done using FAT, having the root at `/` and using `/` as the hierachy separator character. Free storage management is implemented using bitmap.


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

  mkdir <dir>

  rmdir <dir>

  cat <fname>

  touch <fname>

  rm <fname>

  ls <dir>
  
  find <dir> <fname>

  df

  unmount

  help

  sai


NOTES:
  in the whole simulation all filenames must be specified as 
  absolute paths.

AUTHOR:
  Written by Ciro S. Costa <ciro dot costa at usp dot br>.
  See `https://github.com/cirocosta/fssim`. Licensed with MPLv2.
```

## LICENSE

MPLv2. See `LICENSE`.

