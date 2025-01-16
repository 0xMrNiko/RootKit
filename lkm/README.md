# Logger LKM

This project is a Logger Loadable Kernel Module (LKM) that logs various system commands executed by users. The logs include details such as the time of execution, the terminal used, the current working directory, and the binary executed.

## Features

- Logs system commands with detailed information.
- Captures the time of execution.
- Records the terminal (TTY) used.
- Logs the current working directory (CWD).
- Identifies the binary executed.

## Installation

To install the Logger LKM, follow these steps:

1. Clone the repository:
    ```bash
    git clone https://github.com/0xMrNiko/RootKit.git
    cd lkm/logger
    ```

2. Build the module:
    ```bash
    make
    ```

3. Insert the module:
    ```bash
    sudo insmod logger.ko
    ```

4. Check the logs:
    ```bash
    dmesg | grep logger
    ```

## Usage

Once the module is loaded, it will automatically start logging commands executed on the system. You can view the logs using the `dmesg` command or by checking the system log files. The logs are saved in the file `/var/log/logger.log`.

## Example Logs

```bash
Mr.Niko [TIME: 00:37:59] | [TTY: /dev/pts/0] | [CWD: /root/rk] | [BIN: /bin/ls] ls 
Mr.Niko [TIME: 00:38:29] | [TTY: /dev/pts/0] | [CWD: /root/rk] | [BIN: /usr/bin/id] id 
Mr.Niko [TIME: 00:38:33] | [TTY: /dev/pts/0] | [CWD: /root/rk] | [BIN: /usr/bin/whoami] whoami 
Mr.Niko [TIME: 00:38:46] | [TTY: /dev/pts/0] | [CWD: /root/rk] | [BIN: /usr/bin/curl] curl localhost 
Mr.Niko [TIME: 00:38:58] | [TTY: /dev/pts/0] | [CWD: /root] | [BIN: /bin/ls] ls
Mr.Niko [TIME: 00:40:40] | [TTY: /dev/pts/0] | [CWD: /root] | [BIN: /usr/bin/clear] clear 
Mr.Niko [TIME: 00:40:43] | [TTY: /dev/pts/0] | [CWD: /root] | [BIN: /usr/bin/tty] tty 
Mr.Niko [TIME: 00:40:49] | [TTY: /dev/pts/0] | [CWD: /root] | [BIN: /sbin/ip] ip a s 
```

## Tested Kernel Version

This module has been tested on kernel version `4.15.0-91`.

## Contributing

Contributions are welcome! Please open an issue or submit a pull request for any improvements or bug fixes.

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.

## Acknowledgements

- Thanks to all the contributors and open-source projects that made this possible.