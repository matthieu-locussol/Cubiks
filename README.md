# Cubiks

[![Linux](https://img.shields.io/github/workflow/status/matthieu-locussol/Cubiks/Continuous%20integration?label=Linux&logo=linux&logoColor=white)](https://github.com/matthieu-locussol/Cubiks/actions/workflows/ci.yml)
[![MacOS](https://img.shields.io/github/workflow/status/matthieu-locussol/Cubiks/Continuous%20integration?label=MacOS&logo=apple&logoColor=white)](https://github.com/matthieu-locussol/Cubiks/actions/workflows/ci.yml)
[![Windows](https://img.shields.io/github/workflow/status/matthieu-locussol/Cubiks/Continuous%20integration?label=Windows&logo=windows&logoColor=white)](https://github.com/matthieu-locussol/Cubiks/actions/workflows/ci.yml)
[![Documentation](https://img.shields.io/static/v1?label=Documentation&message=online&color=success&logo=github&logoColor=white&style=flat)](https://matthieu-locussol.github.io/Cubiks/)
[![Codacy](https://img.shields.io/codacy/grade/88a1fd1898984ab58058be450b6e94d9?label=Code%20quality&logo=codacy&logoColor=white)](https://www.codacy.com/gh/matthieu-locussol/Cubiks/dashboard?utm_source=github.com&utm_medium=referral&utm_content=matthieu-locussol/Cubiks&utm_campaign=Badge_Grade)
[![Release](https://img.shields.io/github/v/release/matthieu-locussol/Cubiks?label=Release)](https://github.com/matthieu-locussol/Cubiks/releases)

## Author

Matthieu LOCUSSOL - Main developer (<[dev.matthieu.locussol@gmail.com](mailto:dev.matthieu.locussol@gmail.com)>)

## Installation

Create a `build/` directory and `cd` to it:

```bash
mkdir -p build
cd build
```

You can then build the library using CMake:

```bash
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build . --config "Release"
```
