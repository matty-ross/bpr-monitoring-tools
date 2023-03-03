# Burnout Paradise Remastered monitoring tools

<img align="left" src="https://img.shields.io/badge/c++-%2300599C.svg?style=for-the-badge&logo=c%2B%2B&logoColor=white" />
<img align="left" src="https://img.shields.io/badge/Visual%20Studio-5C2D91.svg?style=for-the-badge&logo=visual-studio&logoColor=white" />
<img src="https://img.shields.io/badge/Windows-0078D6?style=for-the-badge&logo=windows&logoColor=white" />

A collection of tools for monitoring various stuff in Burnout Paradise Remastered.


# Building
1. Open the solution in Visual Studio
2. Compile for **x86** platform as a **dll** file
3. Inject the dll(s) into the game


# Libraries
## Common
A library with components which are commonly used in multiple tools.


# Tools
## Network Traffic
A tool that creates a dummy local server and simulates the network traffic between the game and the game server without the encryption.
- Default port: 16976
## Game Actions
A tool that monitors the game actions and prints their contents into the console.
- Assumes that the console has already been allocated
