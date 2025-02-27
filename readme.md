<div align="center">
   
# RPCSX
*An experimental emulator for PS4 (PlayStation 4) for Linux written in C++*

![](https://img.shields.io/github/actions/workflow/status/RPCSX/rpcsx/format.yml?style=for-the-badge&label=Formatting)
[![](https://img.shields.io/discord/252023769500090368?style=for-the-badge)](https://discord.gg/t6dzA4wUdG)

</div>

> **Warning** <br/>
> It's NOT possible to run any games yet, and there is no ETA for when this will change


## Contributing

If you want to contribute as a developer, please contact us in the [Discord](https://discord.gg/t6dzA4wUdG).

## Building

1. Install dependencies for Debian-like distributions
   
   `sudo apt install build-essential cmake libunwind-dev libglfw3-dev libvulkan-dev vulkan-validationlayers-dev spirv-tools glslang-tools`
2. Building
   
   `mkdir -p build && cd build && cmake .. && cmake --build .`

4. Run
   
   See usage message of rpcsx-os (-h argument), or join the [Discord](https://discord.gg/t6dzA4wUdG) for help.

## License

RPCSX is licensed under GPLv2 license except directories containing their own LICENSE file, or files containing their own license.
Thus, orbis-kernel is licensed under the MIT license.
