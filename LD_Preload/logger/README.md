# LD_Preload Logger

This project uses the `LD_PRELOAD` technique to log executed commands. It captures and logs details such as the time of execution, the current working directory, and the binary executed.

## Features

- Logs the time of command execution
- Logs the current working directory
- Logs the executed binary
- Automatically logs all commands executed on the system

## Installation

To install the logger, follow these steps:

1. Clone the repository:
    ```sh
    git clone https://github.com/0xMrNiko/RootKit.git
    ```

2. Navigate to the logger directory:
    ```sh
    cd LD_Preload/logger
    ```

3. Compile the logger:
    ```sh
    make
    ```

4. Copy the compiled shared object to the library directory:
    ```sh
    cp logger.so /lib/logger.so
    ```

5. Preload the logger shared object:
    ```sh
    echo "/lib/logger.so" > /etc/ld.so.preload
    ```

## Usage

Once installed, the logger will automatically log commands executed on the system. The log entries will be saved in `/var/log/logger.log`.

## Example Log Entries

```bash
Mr.Niko root [TIME: 01:03:46] | [TTY: N/A] | [CWD: /] | [BIN: /usr/bin/clear] clear 
Mr.Niko root [TIME: 01:03:46] | [TTY: N/A] | [CWD: /] | [BIN: /bin/ls] ls 
Mr.Niko root [TIME: 01:03:48] | [TTY: N/A] | [CWD: /] | [BIN: /usr/bin/clear] clear 
Mr.Niko root [TIME: 01:03:50] | [TTY: N/A] | [CWD: /var/log] | [BIN: /bin/ls] ls 
Mr.Niko root [TIME: 01:03:55] | [TTY: N/A] | [CWD: /var/log] | [BIN: /bin/cat] cat logger.log
```

## Log File

The log file is located at:

```
/var/log/logger.log
```

## Uninstallation

To remove the logger, follow these steps:

1. Remove the preload configuration:
    ```sh
    rm -rf /etc/ld.so.preload
    ```

2. Remove the logger shared object:
    ```sh
    rm -rf /lib/logger.so
    ```

## License

This project is licensed under the MIT License.