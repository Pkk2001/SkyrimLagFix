# Third-Party Notices

Skyrim Lag Fix uses third-party libraries and build tools. Their respective copyright notices and license terms are reproduced below.

The MIT License in the project's root `LICENSE` file applies to the original Skyrim Lag Fix project code. It does not replace or override the licenses of third-party components.

## 1. Third-party components

The following versions are specified by the project's vcpkg dependency configuration:

| Component | Version | License |
|---|---:|---|
| CommonLibSSE-NG | 3.6.0 | MIT |
| fmt | 9.1.0 | MIT |
| rapidcsv | 8.64 | BSD-3-Clause |
| spdlog | 1.11.0 | MIT |
| xbyak | 6.60 | BSD-3-Clause |
| vcpkg-cmake-config | 2022-02-06 | MIT |

The first five components are project dependencies. `vcpkg-cmake-config` is a build-time dependency.

## 2. MIT-licensed components

### CommonLibSSE-NG

Copyright (c) 2018 Ryan-rsm-McKenzie

License: MIT

### fmt

Copyright (c) 2012 - present, Victor Zverovich

License: MIT

**Optional exception to the fmt license**

As an exception, if, as a result of your compiling your source code, portions of this Software are embedded into a machine-executable object form of such source code, you may redistribute such embedded portions in such object form without including the above copyright and permission notices.

### spdlog

Copyright (c) 2016 Gabi Melman.

License: MIT

spdlog depends on the fmt library, which is separately acknowledged above.

### vcpkg-cmake-config

Copyright (c) Microsoft Corporation

All rights reserved.

License: MIT

### MIT License Terms

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

## 3. BSD-3-Clause-licensed components

### rapidcsv

Copyright (c) 2017, Kristofer Berggren

All rights reserved.

### xbyak

Copyright (c) 2007 MITSUNARI Shigeo

All rights reserved.

### BSD 3-Clause License Terms

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

## 4. Dependency management

Dependencies are managed through [Microsoft vcpkg](https://github.com/microsoft/vcpkg) and the [vcpkg-colorglass registry](https://gitlab.com/colorglass/vcpkg-colorglass).

- CommonLibSSE-NG is obtained through the vcpkg-colorglass registry.
- fmt, rapidcsv, spdlog, and xbyak are obtained through the configured vcpkg registry.
- vcpkg-cmake-config is used as a build-time dependency.

Please refer to the respective upstream projects for further information:

- [CommonLibSSE-NG](https://github.com/CharmedBaryon/CommonLibSSE)
- [fmt](https://github.com/fmtlib/fmt)
- [rapidcsv](https://github.com/d99kris/rapidcsv)
- [spdlog](https://github.com/gabime/spdlog)
- [xbyak](https://github.com/herumi/xbyak)
- [vcpkg](https://github.com/microsoft/vcpkg)
