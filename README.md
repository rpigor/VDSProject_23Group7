<div id="top"></div>

<!-- PROJECT SHIELDS -->
[![Contributors][contributors-shield]][contributors-url]
[![Stargazers][stars-shield]][stars-url]
[![Issues][issues-shield]][issues-url]
<!--
[![MIT License][license-shield]][license-url]
[![LinkedIn][linkedin-shield]][linkedin-url]
-->

<!-- PROJECT LOGO -->
<br />
<div align="center">
  <a href="https://github.com/rpigor/VDSProject_23Group7">
    <img src="doc/figures/RPTU-Logo-RGB.png" alt="Logo" width="400" height="200">
  </a>
  <h3 align="center">
  VDS Class Project
  <br />
  Group #7
  <br />
  Winter Semester 2023/2024
  </h3>

  <p align="center">
    GitHub repository for the Verification of Digital Systems Class Project
    <br />
    <br />
    <a href="https://github.com/rpigor/VDSProject_23Group7/issues">Report Bug</a>
  </p>
</div>

<br />
<br />

<!-- TABLE OF CONTENTS -->
<details>
  <summary>Table of Contents</summary>
  <ol>
    <li>
      <a href="#about-the-project">About The Project</a>
    </li>
    <li>
      <a href="#getting-started">Getting Started</a>
      <ul>
        <li><a href="#dependencies">Dependencies</a></li>
        <li><a href="#installation">Installation</a></li>
      </ul>
    </li>
    <li><a href="#roadmap">Roadmap</a></li>
    <li><a href="#contact">Contact</a></li>
    <li><a href="#acknowledgments">Acknowledgments</a></li>
  </ol>
</details>

<!-- ABOUT THE PROJECT -->
## About The Project
In the course of this Lab assignment, students are going to implement a minimal BDD package in C++. This package implements the fundamental manipulation methods for ROBDDs as they were introduced in the lecture *Verification of Digital Systems* by Prof. Kunz. The package will be implemented using the Test Driven Development (TDD) paradigm presented by Dr. Wedler.

The project is split into three parts.
<p align="right">(<a href="#top">back to top</a>)</p>

#### Part 1:
Implementation of the basic functionality of the BDD package using the TDD methodology. This is the biggest part of the project.

Tasks in this part:
* Set up and maintain a Git repository
* Use CMake as a build system of the project
* Verify the code using GTest
* Set up a Continuous Integration (CI) pipeline with GitHub Actions **(Mandatory)**
* Use Doxygen (or a similar tool) to generate documentation for the API. _(Optional)_
<p align="right">(<a href="#top">back to top</a>)</p>

#### Part 2:
Improvement of the performance of the implementation via provided benchmarks.

Tasks in this part:
* Learning how to identify performance bottlenecks within the code.
* Analyzing benchmark results.
* How to overcome bottlenecks.
<p align="right">(<a href="#top">back to top</a>)</p>

#### Part 3:
Extending the existing implementation by a practical application of BDD. Using BDDs, it is possible to symbolically represent a state-space. This representation allows to check quickly, whether a specific state is within the reachable state space or not.

<p align="right">(<a href="#top">back to top</a>)</p>

<!-- GETTING STARTED -->
## Getting Started
Fork this repository and follow the instructions given in `doc` to complete the project.

### Dependencies
List of dependencies required to compile the project:

* Boost 1.81 or higher (`libboost-all-dev`)
* Graphviz (`graphviz-dev`)
* GoogleTest (`libgtest-dev`)
* Build tools (`build-essential` and `cmake`)

Be aware that this project uses `boost::unordered_flat_map` and thus requires Boost 1.81 or higher, which is not currently available in Ubuntu's official repository. If you don't have access to this version of the library, you can use `boost::unordered_map` with minor adjustments.

<p align="right">(<a href="#top">back to top</a>)</p>

### Installation
1. Install the dependencies

2. Clone the repo
   ```sh
   git clone https://github.com/rpigor/VDSProject_23Group7
   ```
**Installation with CLion**

CLion comes with CMake.

3. Open CLion and open VDSProject_23Group7/CMakeLists.txt as a project

4. Select your target and build the project

**Installation without CLion**

3. Navigate to the project folder
    ```sh
    cd VDSProject_23Group7
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

<!-- ROADMAP
## Roadmap
#### Part-1
- [ ] TODO
- [X] DONE
<p align="right">(<a href="#top">back to top</a>)</p>

#### Part-2
- [ ] TODO
- [X] DONE
<p align="right">(<a href="#top">back to top</a>)</p>

#### Part-3
- [ ] TODO
- [X] DONE
<p align="right">(<a href="#top">back to top</a>)</p>
-->

<!-- CONTACT -->
## Contact

<!-- Your Name - [@your_twitter](https://twitter.com/your_username) - email@example.com -->
Lucas Deutschmann & Philipp Schmitz - eit-vds-cp@rptu.de

Igor Reis - xaj60vom@rptu.de

<p align="right">(<a href="#top">back to top</a>)</p>

<!-- ACKNOWLEDGMENTS -->
## Acknowledgments
Thank you Veli Durmuşcan, Shreya Vithal Kulhalli and Osama Omar Youssif Ayoub for the work on this README.md.

You might find helpful links below.

### Project Related Resources

#### Given Task Links
* [WSL](https://docs.microsoft.com/en-us/windows/wsl/install)
* [Google Test](https://github.com/google/googletest)
* [GitHub CI Interactive Tutorial](https://lab.github.com/githubtraining/github-actions:-hello-world)
* [Documenting C++ Code](https://developer.lsst.io/cpp/api-docs.html)
* [Doxygen Examples](https://www.doxygen.nl/results.html)
* [WSL Integration for CLion](https://www.jetbrains.com/help/clion/how-to-use-wsl-development-environment-in-product.html#wsl-tooclhain)
* [How to install Boost library in Ubuntu](https://stackoverflow.com/questions/12578499/how-to-install-boost-on-ubuntu)

<p align="right">(<a href="#top">back to top</a>)</p>

#### Additional Materials

* [README Template](https://github.com/othneildrew/Best-README-Template)
* [Travis CI Examples](https://github.com/deftio/travis-ci-cpp-example)

<p align="right">(<a href="#top">back to top</a>)</p>

<!-- MARKDOWN LINKS & IMAGES -->
<!-- https://www.markdownguide.org/basic-syntax/#reference-style-links -->
[contributors-shield]: https://img.shields.io/github/contributors/rpigor/VDSProject_23Group7.svg?style=for-the-badge
[contributors-url]: https://github.com/rpigor/VDSProject_23Group7/graphs/contributors
[forks-shield]: https://img.shields.io/github/forks/rpigor/VDSProject_23Group7.svg?style=for-the-badge
[forks-url]: https://github.com/rpigor/VDSProject_23Group7/network/members
[stars-shield]: https://img.shields.io/github/stars/rpigor/VDSProject_23Group7.svg?style=for-the-badge
[stars-url]: https://github.com/rpigor/VDSProject_23Group7/stargazers
[issues-shield]: https://img.shields.io/github/issues/rpigor/VDSProject_23Group7.svg?style=for-the-badge
[issues-url]: https://github.com/rpigor/VDSProject_23Group7/issues
<!--
[license-shield]: https://img.shields.io/github/license/othneildrew/Best-README-Template.svg?style=for-the-badge
[license-url]: https://github.com/othneildrew/Best-README-Template/blob/master/LICENSE.txt
[linkedin-shield]: https://img.shields.io/badge/-LinkedIn-black.svg?style=for-the-badge&logo=linkedin&colorB=555
[linkedin-url]: https://linkedin.com/in/othneildrew
[product-screenshot]: images/screenshot.png
-->
