# Diglot Reader - Learn and Explore with Language-Infused Texts

[![CMake](https://github.com/theinformationretrievers/cosc345/actions/workflows/windows.yml/badge.svg)](https://github.com/theinformationretrievers/cosc345/actions/workflows/windows.yml)
[![codecov](https://codecov.io/gh/theinformationretrievers/cosc345/branch/main/graph/badge.svg)](https://app.codecov.io/gh/theinformationretrievers/cosc345)
[![Codacy Badge](https://app.codacy.com/project/badge/Grade/8e10b54517ff492bb5c0649f71236a66)](https://app.codacy.com/gh/theinformationretrievers/cosc345/dashboard?utm_source=gh&utm_medium=referral&utm_content=&utm_campaign=Badge_grade)
[![Documentation](https://codedocs.xyz/theinformationretrievers/cosc345.svg)](https://codedocs.xyz/theinformationretrievers/cosc345/)

---

Welcome to **Diglot Reader**, a text reading app designed to enhance language learning through technqiues such as space repitition and the Diglot Weave method. Our app combines the power of a text reader with an integrated Te Reo Māori vocabulary training component, helping users learn Te Reo Māori though general day-to-day reading.

### Key Features

- **Extensive eBook Library:**
- \*\*Aids in increasing use of Maori in everyday life by gradually switching out common English words and Maori
- **Part of speech tagging** Increases accuracy of words

### Feedback Form (under ethics review)

https://forms.gle/isLtkvW6b8zuNcN77

### Installation

The code is build with cmake. The current build does work on both windows and linux, though the linux version is less stable.

## To install

```
git clone git@github.com:theinformationretrievers/cosc345.git
cd cosc345
open in visual studio
run Diglot.exe once it has compiled

```

In order for the app to work on Linux, install `libgtk-3-dev`, additionally g++ version must be > 13 e.g. for ubuntu:

```
sudo apt-get install libgtk-3-dev
sudo add-apt-repository ppa:ubuntu-toolchain-r/test
sudo apt-get update
sudo apt-get install gcc-13 g++-13
Then to run specify compiler by 
mdkir && cd build
cmake -DCMAKE_TOOLCHAIN_FILE=gcc-toolchain.cmake ..

```

### License
