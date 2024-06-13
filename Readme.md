CR3Converter is a GUI application that converts CR3 files into other raw and jpeg files.

### Compiling and Running

To compile and run the application, you need to have the following dependencies installed:
```bash
sudo apt install libglfw3-dev
sudo apt install libglew-dev
sudo apt install libraw-dev
```

Then create a build directory and run cmake:
```bash
mkdir build
cd build
cmake ..
make
```

To run the application, you can use the following command:
```bash
.build/bin/CR3Converter
```