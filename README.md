# Project Ananta

> [!WARNING]
> Work in progress!

**Distributed Fault-Tolerant Robotic System**

Project Ananta is the first research and engineering project under [Navikarana Labs](https://navikarana.io), exploring distributed embedded control, fault detection, diagnosis, and safe recovery.

A tracked robotic rover will serve as the initial test platform.

### Links

- [Project Page](https://navikarana.io/projects/001-ananta/)
- [Introduction & Development Blog](https://loke.sh/blog/navikarana-project-ananta/introduction/)

### License

Licensed under the [Apache License 2.0](LICENSE).

## Development

Project Ananta uses [Zephyr RTOS](https://www.zephyrproject.org/) for its embedded firmware.

The Zephyr workspace and toolchain are maintained separately from the Ananta repository. Firmware build artifacts are stored locally under `build/`.

### Prerequisites

Install Git, Python, CMake, Ninja, Devicetree Compiler, and [uv](https://docs.astral.sh/uv/).

Refer to the [Zephyr Getting Started Guide](https://docs.zephyrproject.org/latest/develop/getting_started/index.html) for platform-specific dependencies.

### Zephyr Setup

Create a separate Zephyr workspace:

```shell
mkdir -p ~/zephyrproject
cd ~/zephyrproject

uv venv --python 3.13 .venv
```

Activate the Python environment.

**Bash / Zsh:**

```shell
source .venv/bin/activate
```

**Fish:**

```fish
source .venv/bin/activate.fish
```

Install `west` and initialize Zephyr v4.4.0:

```shell
uv pip install west

west init -m https://github.com/zephyrproject-rtos/zephyr --mr v4.4.0 .
west update

west zephyr-export
uv pip install -r zephyr/scripts/requirements.txt
```

Install the Zephyr SDK with only the ARM toolchain:

```shell
cd ~/zephyrproject/zephyr

west sdk install --toolchains arm-zephyr-eabi
```

Verify the SDK installation:

```shell
west sdk list
```

The SDK provides the cross-compiler and supporting tools required to build Zephyr firmware for ARM targets.

See the [Zephyr SDK documentation](https://docs.zephyrproject.org/latest/develop/toolchains/zephyr_sdk.html) for more information.

### Environment Configuration

Set `ZEPHYR_BASE` to allow building applications outside the Zephyr workspace.

**Bash / Zsh** (`~/.bashrc` or `~/.zshrc`):

```shell
export ZEPHYR_BASE="$HOME/zephyrproject/zephyr"
```

**Fish** (`~/.config/fish/config.fish`):

```fish
set -gx ZEPHYR_BASE $HOME/zephyrproject/zephyr
```

Restart your shell or reload its configuration.

Activate the Zephyr Python environment whenever working with its tools.

### Clone & Build

Clone Ananta into any directory:

```shell
git clone https://github.com/navikarana-labs/ananta.git
cd ananta
```

Activate the Zephyr virtual environment if necessary.

Build an application from the repository root:

```shell
west build -p always \
    -b BOARD_NAME \
    -s firmware/APP_NAME \
    -d build/APP_NAME \
    -- -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
```

Replace `BOARD_NAME` and `APP_NAME` with the appropriate board target and firmware application.

**Example: Motion Controller (nRF52840 DK)**

```shell
west build -p always \
    -b nrf52840dk/nrf52840 \
    -s firmware/motion-controller \
    -d build/motion-nrf52840 \
    -- -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
```

The resulting firmware is located at:

```text
build/motion-nrf52840/zephyr/zephyr.hex
```

The build also generates `compile_commands.json` for editor tooling such as `clangd`.

**Subsequent builds:**

```shell
west build -d build/motion-nrf52840
```

### Flash

Connect the nRF52840 DK and flash the compiled firmware:

```shell
west flash -d build/motion-nrf52840
```

A supported flashing runner and the necessary device permissions must be configured.

Flashing replaces the firmware currently installed on the target board.

### Clean

Clean compiled artifacts from an existing, configured build directory:

```shell
west build -t clean -d build/motion-nrf52840
```

For a complete rebuild, including a fresh CMake configuration:

```shell
west build -p always \
    -b nrf52840dk/nrf52840 \
    -s firmware/motion-controller \
    -d build/motion-nrf52840 \
    -- -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
```

> Build artifacts are stored under `build/` inside the Ananta repository. Ensure this directory is excluded from version control. Build, flash, and clean commands must reference the appropriate build directory.
