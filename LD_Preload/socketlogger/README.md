# SocketLogger

SocketLogger is a tool that intercepts and logs data received over network sockets. It logs the data to a specified log file for further analysis.

## Installation

1. Compile the `socketlogger` shared library:
    ```sh
    make
    ```

2. Copy the compiled shared library to `/lib/`:
    ```sh
    sudo cp socketlogger.so /lib/
    ```

3. Preload the shared library by adding it to `/etc/ld.so.preload`:
    ```sh
    echo /lib/socketlogger.so | sudo tee -a /etc/ld.so.preload
    ```

## Log File

The intercepted data is logged to `/var/log/net.log`. Ensure that the application has the necessary permissions to write to this file.

### Demo Log

Here is an example of what the log file might look like:

```
HTTP/1.0 200 OK
Server: SimpleHTTP/0.6 Python/3.12.8
Date: Fri, 17 Jan 2025 05:35:54 GMT
Content-type: text/x-csrc
Content-Length: 2000
Last-Modified: Fri, 17 Jan 2025 04:47:39 GMT

<file content>
```

### Triggering the Log

You can use the following `curl` command to trigger the log file:

```sh
curl http://example.com/script.sh
```

## How It Works

SocketLogger uses the `LD_PRELOAD` technique to intercept the `recv` function call. It buffers the received data and logs it once the full response is received.

