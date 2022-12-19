cd ..
git clone https://github.com/Microsoft/vcpkg.git
.\vcpkg\bootstrap-vcpkg.bat
.\vcpkg\vcpkg install install cpprestsdk[default-features, websockets]:x64-windows
.\vcpkg\vcpkg install install boost:x64-windows
.\vcpkg\vcpkg integrate install