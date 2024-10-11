# Metro Bundler Launcher
[![license](https://img.shields.io/badge/license-MIT-blue.svg)](https://github.com/ncorgan/metro_launcher/blob/master/LICENSE.txt) [![GitHub release (latest by date)](https://img.shields.io/github/v/release/savaughn/metro_launcher)](https://github.com/savaughn/metro_launcher/releases) [![GitHub Workflow Status](https://img.shields.io/github/actions/workflow/status/savaughn/metro_launcher/macos.yml?branch=master)](https://github.com/savaughn/metro_launcher/actions) [![GitHub issues](https://img.shields.io/github/issues/savaughn/metro_launcher)](https://github.com/savaughn/metro_launcher/issues)[![GitHub all releases](https://img.shields.io/github/downloads/savaughn/metro_launcher/total)](https://github.com/savaughn/metro_launcher/releases)

Metro Bundler Launcher is a GTK4-based application designed to manage the Metro Bundler for React Native projects. It provides a graphical interface to start, terminate, and restart the Metro Bundler.

<img width="532" alt="image" src="https://github.com/user-attachments/assets/d1615ac8-3945-48d2-9bbf-13f9fa8f9083">

## Contributing

### Prerequisites

- GTK4
- GCC
- macOS (for setup script)

### Setup for MacOS

1. **Clone the repository**:
    ```sh
    git clone https://github.com/savaughn/metro_launcher.git 
    cd metro_launcher
    ```

2. **Install dependencies**:
    ```sh
    ./setup_macos.sh
    ```

3. **Build the project**:
    ```sh
    make
    ```

4. **Run the application**:
    ```sh
    make run
    ```

### Makefile

The `Makefile` includes targets for building and running the project:
- `make`: Builds the project.
- `make run`: Runs the built executable.
- `make clean`: Cleans up the build artifacts.

## Contributing
 - Fork and PR, please.

## License

This project is licensed under the MIT License.
