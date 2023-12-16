# How to use PortScanTool

Environment : Visual Studio 2022

1. Download the folder.
2. Unzip the downloaded folder.
3. Run the visual studio and build with x64 releases.
4. In the CMD, enter the command.

--------------------------------------------------------

# Command Options
**Required Options**
  - `PortScanner.exe` : Run a program 
  - `-targetIp [TargetServerIp]` : Enter the IP of the target server that you want to scan 
  - `-port [start] [end]` : Port range you want to check. //Default value : 1 65535

**Selection Options**
  - `-save [FileName.txt]` : If you enter a file name and save the extension as txt, it is saved as a text file for the open port.

**Examples of Commands**
  - `PortScanner.exe -targetIp 127.0.0.1 -port 1 5000 -save check.txt`
    
