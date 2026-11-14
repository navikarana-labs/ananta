# Project Ananta

> [!WARNING]
> Work in progress!

**Share:** [`nvkr.link/ananta`](https://nvkr.link/ananta)

**Distributed Fault-Tolerant Robotic System**

Project Ananta is the first research and engineering project under [Navikarana Labs](https://navikarana.io), exploring distributed embedded control, fault detection, diagnosis, and safe recovery.

A tracked robotic rover will serve as the initial test platform.

**See [PROGRESS.md](PROGRESS.md) for the latest development progress.**

- [Project Page](https://navikarana.io/projects/001-ananta/)
- [Introduction & Development Blog](https://loke.sh/blog/navikarana-project-ananta/introduction/)

## Hardware

Current bring-up uses two WeAct STM32G431 Core Boards, connected individually over USB-C for flashing and console access. FDCAN1 uses PB8 (RX) and PB9 (TX).

## Development

Uses [Zephyr RTOS](https://www.zephyrproject.org/), with the workspace and toolchain kept outside this repository. Follow the [Zephyr setup guide](https://docs.zephyrproject.org/latest/develop/getting_started/index.html), using `uv` for the Python environment.

Tested with Zephyr `v4.5.0-rc2-516-gef27f62aa8f0` (commit `ef27f62aa8f08f0fc74e90ce95949d3530968f95`), SDK 1.0.1, west 1.5.0, and Python 3.13.16. Install `dfu-util` and `minicom`.

Activate your Zephyr Python environment and set `ZEPHYR_BASE` to your Zephyr source directory:

- Bash/Zsh: `export ZEPHYR_BASE="$HOME/zephyrproject/zephyr"`
- Fish: `set -gx ZEPHYR_BASE $HOME/zephyrproject/zephyr`

### Clone & Build

```shell
git clone https://github.com/navikarana-labs/ananta.git
cd ananta

west build -p always \
    -b weact_stm32g431_core/stm32g431xx \
    -s firmware/motion-controller \
    -d build/motion-stm32 \
    -S cdc-acm-console \
    -- -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
```

Subsequent builds: `west build -d build/motion-stm32`.

### Flash & Console

Connect one board over USB-C:

```shell
west flash -d build/motion-stm32 -- --dfuse-modifiers=
```

To enter DFU, hold **BOOT0**, press and release **RESET**, then release **BOOT0**. After flashing, press **RESET alone** to run the application.

```shell
minicom -D /dev/ttyACM0 -b 115200
```

Use the assigned `/dev/ttyACM*` device with flow control disabled.

## License

Licensed under the [Apache License 2.0](LICENSE).
