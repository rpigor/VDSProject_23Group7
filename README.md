<div id="top"></div>

<!-- PROJECT SHIELDS -->
[![Contributors][contributors-shield]][contributors-url]
[![Stargazers][stars-shield]][stars-url]
[![Issues][issues-shield]][issues-url]

<!-- PROJECT LOGO -->
<br />
<div align="center">
  <h3 align="center">
  ROBDD library
  </h3>

  <p align="center">
    Verification of Digital Systems class project
    <br />
    Winter Semester 2023/2024
    <br />
    Group #7
  </p>
</div>

<br />
<br />

<!-- TABLE OF CONTENTS -->
<details>
  <summary>Table of Contents</summary>
  <ol>
    <li>
      <a href="#about-the-project">About the project</a>
    </li>
    <li>
      <a href="#getting-started">Getting started</a>
      <ul>
        <li><a href="#dependencies">Dependencies</a></li>
        <li><a href="#installation">Installation</a></li>
      </ul>
    </li>
    <li><a href="#acknowledgments">Acknowledgments</a></li>
  </ol>
</details>

<!-- ABOUT THE PROJECT -->
## About the project
This is a minimal Reduced Ordered Binary Decision Diagram (ROBDD) package written in C++. This package implements the fundamental manipulation methods for BDDs as they were introduced in the lecture Verification of Digital Systems by Prof. Kunz. The package was implemented using the Test Driven Development paradigm presented by Dr. Wedler.

The project is split into three parts.

- Implementation of the basic functionality of the BDD package using the TDD methodology. This is the biggest part of the project.

- Improvement of the performance of the implementation via provided benchmarks.

- Extending the existing implementation by a practical application of BDD. Using BDDs, it is possible to symbolically represent a state-space and perform a reachability analysis.

<p align="right">(<a href="#top">back to top</a>)</p>

<!-- GETTING STARTED -->
## Getting started

### Dependencies
List of dependencies required to compile the project:

* Boost 1.81 or higher (`libboost-all-dev`)
* Graphviz (`graphviz-dev`)
* GoogleTest (`libgtest-dev`)

Be aware that this project uses `boost::unordered_flat_map` and thus requires Boost 1.81 or higher, which is not currently available in Ubuntu's official repository. If you don't have access to this version of the library, you can use `boost::unordered_map` with minor adjustments.

### Installation
1. Install the dependencies

2. Clone the repo
   ```sh
   git clone https://github.com/rpigor/librobdd
   ```
**Installation with CLion**

CLion comes with CMake.

3. Open CLion and open librobdd/CMakeLists.txt as a project

4. Select your target and build the project

**Installation without CLion**

3. Navigate to the project folder
    ```sh
    cd librobdd
    ```
4. Invoke CMake to create the build directory and generate makefiles
    ```sh
    cmake -S . -B build
    ```
5. Invoke CMake to build the project
    ```sh
    cmake --build build
    ```

<p align="right">(<a href="#top">back to top</a>)</p>

<!-- ACKNOWLEDGMENTS -->
## Acknowledgments
Thank you Veli Durmuşcan, Shreya Vithal Kulhalli and Osama Omar Youssif Ayoub for the work on this README.md.

<p align="right">(<a href="#top">back to top</a>)</p>

<!-- MARKDOWN LINKS & IMAGES -->
<!-- https://www.markdownguide.org/basic-syntax/#reference-style-links -->
[contributors-shield]: https://img.shields.io/github/contributors/rpigor/librobdd.svg?style=for-the-badge
[contributors-url]: https://github.com/rpigor/librobdd/graphs/contributors
[stars-shield]: https://img.shields.io/github/stars/rpigor/librobdd.svg?style=for-the-badge
[stars-url]: https://github.com/rpigor/librobdd/stargazers
[issues-shield]: https://img.shields.io/github/issues/rpigor/librobdd.svg?style=for-the-badge
[issues-url]: https://github.com/rpigor/librobdd/issues
