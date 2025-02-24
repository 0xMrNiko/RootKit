<div align="center">
  <img src="space.png" width="90%"><br><br>
  <h2>Linux Rootkit's</h2>
</div><br>

## Description

This repository contains **Loadable Kernel Modules (LKM)** and **LD_PRELOAD-based modules** designed for penetration testing, red teaming, and security research. These tools are intended to help researchers understand and experiment with advanced techniques like syscall hooking, runtime manipulation, and stealth persistence.

---

## Modules Overview

| **No.** | **LKM Modules**                             | **LD_PRELOAD Modules**                        |
|---------|---------------------------------------------|-----------------------------------------------|
| 1       | [Logger](https://github.com/0xMrNiko/RootKit/tree/main/lkm/logger) | [Logger](https://github.com/0xMrNiko/RootKit/tree/main/LD_Preload/logger)|
| 2       | [hide_lkm](https://github.com/0xMrNiko/RootKit/tree/main/lkm/hide_lkm) | [SocketLogger](https://github.com/0xMrNiko/RootKit/tree/main/LD_Preload/socketlogger)|
| 3       |  | [LibProchide](https://github.com/0xMrNiko/RootKit/tree/main/LD_Preload/libprochide)|

---

## Disclaimer

**This repository is for educational and research purposes only.**  
We are **not responsible for any damages** caused by the use of these tools.  
Use is strictly limited to **controlled environments** such as virtual machines (VMs) with proper authorization and in compliance with applicable laws.

---

### **Note**
Always ensure ethical use of these tools. Unauthorized use on systems without explicit permission is illegal and unethical.
