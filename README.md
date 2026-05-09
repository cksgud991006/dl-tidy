![DL-Tidy Logo](./apps/assets/dl-tidy.ico)
# dl-tidy
keep your download folder always empty

---

## Features

- Watches a Downloads folder for changes  
- Automatically deletes incomplete or temporary files (e.g. `.tmp`, `.crdownload`) in Downloads folder
- Runs quietly in the system tray  
- Optional autostart on Windows login  

---

## Installation

1. **Download** the installer:  
   [`DL-Tidy-Setup-1.0.0.exe`](#) (from the official release page)

2. **Run** the installer and follow the on-screen instructions.  
   By default, DL-Tidy installs to:

---
## Usage

<img width="143" height="76" alt="Untitled" src="https://github.com/user-attachments/assets/80b4c880-8edc-4dcb-9fd2-b4e6e691e7f6" />

- **Right-click** the tray icon to open the menu.  
- *Clean* → ㅡManually clean up the download folder  
- *Exit* → Closes the app



---
## Build

Developer mode requires building the program through cmake commands.

```C++
cmake -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Debug -B build
cmake --build build   
```
