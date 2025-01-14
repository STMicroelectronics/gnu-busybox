# Busybox for STM32

This repository contains sources and build scripts of Busybox included into [STM32CubeIDE](https://www.st.com/en/development-tools/stm32cubeide.html) advanced development platform and part of the [STM32Cube](https://www.st.com/en/ecosystems/stm32cube.html) software ecosystem. It is based on [Busybox](https://www.busybox.net/) sources, with patches improving embedded systems development.

## License

See [LICENSE.md](LICENSE.md)

## Host Platforms

* Windows

## Communication and support

For communication and support, please refer to:

- [ST Support Center](https://my.st.com/ols#/ols/) for any defect
- ST Community [MCUs](https://community.st.com/t5/stm32cubeide-mcus/bd-p/stm32-mcu-cubeide-forum) or [MPUs](https://community.st.com/t5/stm32cubeide-mpus/bd-p/stm32-mpu-cubeide-forum) forums

## Patches

Patch                                                                | Description |
---------------------------------------------------------------------|--------------- |
Fix for long path issues on Windows                                  | Windows has a limit of the number of characters in paths to files. This fix allows up to 248 characters in paths to GNU Make binary and up to 4096 characters for all files processed by the tool. Without the patch the latter limit is about 150 characters. |
