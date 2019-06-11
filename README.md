
# NeuzPE -- a PoC packet editor for FlyFF
NeuzPE is a semi-working, proof-of-concept, packet editor for the game FlyFF. ("Neuz" being the original name of the game). This was made mainly to see if I could bypass GameGuard via hooking virutal functions, as well as messing around with the [Nana](https://github.com/cnjinhao/nana) C++ GUI library (and [Nana Creator](https://github.com/besh81/nana-creator) for quick layout).

I originally intended to come back and finish/polish this project more (and clean up the awful code), but I've lost interest in it and decided to un-private the repo in a semi-working state as of June/10/2019.

![screenshot](https://github.com/Andoryuuta/NeuzPE/blob/master/screenshot/ss1.png)

# Download & Building
1. Clone the repo with submodules: `git clone --recurse-submodules https://github.com/Andoryuuta/NeuzPE`
2. Configure and build [Nana](https://github.com/cnjinhao/nana):
	1. Open the VS2017 `nana.sln` project in `NeuzPE\nana_src\build\vc2017`.
	2. Configure release build: 
		1. Go to `Project -> Properties` and select `Configuration: Release` and `Platform: Win32`.
		2. Under `C/C++ -> Code Generation` select `Runtime Library` and change it to `/MD`.
	3. Go to `Build -> Batch build` and build for x86 Release.
3. Build NeuzPE:
	1. Open the `NeuzPE.sln` in the root directory of the repo.
	2. Select configuration `Release`, and platform `x86`, then build.


# Usage
The built `.dll` simply needs to be injected into the game process with whatever injector you desire, however it must be injected before the game fully loads GameGuard, unless you have a way to inject it post handle-stripping.

* Note: The GUI will only load after it is able to hook one of the game's DirectPlay socket wrapper classes (for the separate auth/login/world connections). If injected before GG loads, this means the GUI will only appear after you login.

# Other notes

Packets are logged in the format of `[SERVER] [PACKET DIRECTION] ... data here ...`  
E.g. `[WORLD] [SEND] FF FF FF FF 71 00 20 02 0D 00 00 00 48 65 6C 6C 6F 2C 20 77 6F 72 6C 64 21`

Packet bytes can be replaced with with `??` which will fill in a random byte before sending.  
E.g. `[WORLD] [SEND] FF FF FF FF 71 00 20 02 04 00 00 00 ?? ?? ?? ??`

`Recv` logging/injection was never implemented, but could be implemented by hooking and calling the `Net::CDPMng` virtual functions: `SysMessageHandler` and `UserMessageHandler`.

The `Send` packet hook blocks while adding entries into the Nana gui, easily stalling the game when there is a large amount of packets being sent (read: anytime you move ingame). I meant to fix this with `moodycamel::ConcurrentQueue` or something, but I never got around to it.

If you are trying to use this in the future and the DirectPlay object byte sig scan isn't working, I've included the full disassembly of the function that the sigscan looks for at the bottom of `Net.cpp`, which includes a couple of strings you should be able to xref in IDA/Ghidra/Binja/Radare/Hopper/REDasm/whatever floats your static disassembly boat.
