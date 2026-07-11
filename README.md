# Burnout Paradise Remastered Monitoring Tools

![](https://img.shields.io/badge/Windows-0078D6?style=for-the-badge&logo=windows&logoColor=white)
![](https://img.shields.io/badge/Visual%20Studio-5C2D91?style=for-the-badge&logo=visual-studio&logoColor=white)
![](https://img.shields.io/badge/C%2B%2B-00599C?style=for-the-badge&logo=c%2B%2B&logoColor=white)

A collection of tools for monitoring various stuff in Burnout Paradise Remastered.


## Setup

1. Clone this repository recursively (with submodules)
1. Open the solution in Visual Studio
1. Set the platform to `x86`
1. Build the solution
1. Inject the built DLL(s) into the game


## Monitoring tools

### Network Traffic

Creates a dummy local server that allows the game's network traffic to be inspected without TLS encryption.
- Local server: `tcp://127.0.0.1:40123`

### Game Events

Monitors game events from `BrnGameState::GameStateModuleIO` and prints them to the console.
- Allocates a console
- Exported game event ID filter: `bool g_ExcludedGameEventIDs[500]`

### Game Actions

Monitors game actions from `BrnGameState::GameStateModuleIO` and prints them to the console.
- Allocates a console
- Exported game action ID filter: `bool g_ExcludedGameActionIDs[500]`
