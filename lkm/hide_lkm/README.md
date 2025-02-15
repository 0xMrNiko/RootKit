# Hide LKM RootKit

This is a simple Linux Kernel Module (LKM) rootkit for educational purposes. It demonstrates how to hide a module from `lsmod`, `/sys/module`, restrict access to `/dev/kmsg`, and suppress `dmesg` logging.

## Features

- Hide module from `lsmod`
- Hide module from `/sys/module`
- Restrict access to `/dev/kmsg`
- Suppress `dmesg` logging

## Compilation

To compile the module, run the following command in the directory containing the `Makefile`:

```sh
make
```

## Loading the Module

To load the compiled module into the kernel, use the `insmod` command:

```sh
sudo insmod hide_lkm.ko
```

## Disclaimer

This rootkit is for educational purposes only. Use it responsibly and only in environments where you have explicit permission to do so. Unauthorized use of this software may violate laws and regulations.
