# NeuzPE -- a PoC packet editor for FlyFF
NOTE: sigscans are currently broken. Additionally, the send packet hook needs to be seperated from the code to update the GUI, as it is stalling the game upon large packet bursts.


# Download & Building
1. Clone the repo with submodules: `git clone --recurse-submodules https://github.com/Andoryuuta/NeuzPE`
2. Configure and build nana:
	1. Open the VS2017 `nana.sln` project in `NeuzPE\nana_src\build\vc2017`.
	2. Configure release build: 
		1. Go to `Project -> Properties` and select `Configuration: Release` and `Platform: Win32`.
		2. Under `C/C++ -> Code Generation` select `Runtime Library` and change it to `/MD`.
	3. (Optional) Configure debug build: 
		1. Go to `Project -> Properties` and select `Configuration: Debug` and `Platform: Win32`.
		2. Under `C/C++ -> Code Generation` select `Runtime Library` and change it to `/MDd`.
	4. Go to `Build -> Batch build` and build for x86 (both Debug and Release).
3. Build NeuzPE:
	1. Open the `NeuzPE.sln` in the root directory of the repo.
	2. Select configuration `Release` or `Debug`, and platform `x86`, then build.
