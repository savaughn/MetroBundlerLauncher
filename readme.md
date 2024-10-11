# Metro Bundler Launcher

Metro Bundler Launcher is a GTK4-based application designed to manage the Metro Bundler for React Native projects. It provides a graphical interface to start, terminate, and restart the Metro Bundler.

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
