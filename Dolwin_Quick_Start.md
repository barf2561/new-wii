# Overview

After restarting the development, the architecture of the emulator has changed a bit.

Now all the main components (like processor, debugger, etc.) use multithreading.

Also, all components are divided into Visual Studio projects, which are a dependency for the main Dolwin.exe project.

Visual Studio solution upgraded to VS2019. There should not be a problem with the build.

Both X86 and X64 targets are supported.

## Dolwin Architecture

Key components are shown in the diagram:

![Dolwin_Architecture](https://github.com/ogamespec/dolwin-docs/blob/master/EMU/DolwinArchitecture.png?raw=true)

## Getting Started

To get started, you can build the Doxygen documentation (Doxyfile is in the root of the project).
To become familiar with the basic components of the emulator is enough to study the Doxygen documentation, there you can find all the basic information on the emulator internals.

If you have additional questions, you can ask them on the Discord server on the #dolwin-dev channel.

## Trends

- Emulator development has become more Agile. Basic milestones can be viewed on Github (https://github.com/ogamespec/dolwin/milestones)
- The source code began to contain more c++, but without fanaticism (limited to namespaces and simple container classes)

## Solution structure

All solution projects are independent components (or try to be like that).

Currently, work is underway to encapsulate components in their namespaces, but the legacy code from version 0.10 does not make it so simple.

Currently, the following namespaces are quietly formed:

- Debug: for debugging functionality
- Gekko: for the core of the Gekko CPU
- Flipper: for various internal Flipper hardware modules (AI, VI, EXI, etc.)
- DSP: for GameCube DSP
- DVD: for a DVD unit (now the functionality is limited to reading images of ISO discs)
- GX: for the Flipper GPU

The user interface will most likely be rewritten as a Managed C# application. In the meantime, it's just like a piece of code from version 0.10.

## Docs

All documentation is now stored in a separate repository: https://github.com/ogamespec/dolwin-docs

## RnD

For experimental and outdated code, https://github.com/ogamespec/dolwin-rnd repository was created.
