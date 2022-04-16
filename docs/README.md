# des
[![GitHub license](https://img.shields.io/badge/license-GNU-green?style=flat)](https://github.com/CastellaniDavide/des/blob/main/docs/LICENSE)
![Author](https://img.shields.io/badge/author-Castellani%20Davide-green?style=flat)
![sys.platform supported](https://img.shields.io/badge/OS%20platform%20supported-all-blue?style=flat) 

##  Description 
Implementation of DES algorithm
![CastellaniDavide/des](https://opengraph.githubassets.com/2195f3d7fb190e44dd0a4b3b7a3a6d448bc5af19b492cbf1e88964cdb8fdef22/CastellaniDavide/des)
 Changelog 
## How to install/ update/ remove
To install this application you need Ubuntu or Debian OS.

If you have Windows you can use WSL: [ufficial guide](https://docs.microsoft.com/en-us/windows/wsl/)

If you have MacOS you can use Homebrew: [official guide](https://brew.sh/)

### Ubuntu
#### Install
First you need to add ppa to your system:


Then you can install the application:


#### Update
To update the application you can use:


#### Remove
To uninstall the application you can use:

### Debian
#### Install / Update
To install the application you need to dowload the package from [releases](https://github.com/CastellaniDavide/des/releases).

Then you can install the application:


#### Remove
To uninstall the application you can use:


##  Directory structure 

```
../repo
├── Makefile
├── bin
│   ├── des.c
│   └── lib
│       ├── base64.h
│       ├── binary.h
│       ├── des.h
│       ├── input.h
│       ├── key.h
│       ├── logo.h
│       └── stopwatch.h
├── debian # debian control code
│   ├── changelog
│   ├── compat
│   ├── control
│   ├── copyright
│   ├── des.dirs
│   ├── rules
│   └── source
│       └── format
├── des.1
├── des.in
└── docs # documentation
    ├── CODE_OF_CONDUCT.md
    ├── LICENSE
    └── README.md

5 directories, 21 files
```
## Changelog
des (02.03) focal; urgency=medium

  * fixed a bug

 -- Castellani Davide <help@castellanidavide.it>  Sat, 16 Apr 2022 14:45:00 +1000

des (02.02) focal; urgency=medium

  * Upgrade documentation
  * fixed a bug

 -- Castellani Davide <help@castellanidavide.it>  Sat, 16 Apr 2022 11:00:00 +1000

des (02.01) focal; urgency=medium

  * Fixed base64 bug (now it's possible encrypt and decrypt correctly messages as "castellani==" in base64)
  * Added encryption and decryption of text messages
  * Added encryption and decryption of files (up to 16MB)
  * Optimized the prints
  * Now it always prints the logo :)
  * Optimized logo
  * Added --version option

 -- Castellani Davide <help@castellanidavide.it>  Fri, 15 Apr 2022 17:00:00 +1000

des (01.02) focal; urgency=medium

  * fixed a bug

 -- Castellani Davide <help@castellanidavide.it>  Tue, 12 Apr 2022 23:15:00 +1000

des (01.01) focal; urgency=medium

  * DES algorithm completed

 -- Castellani Davide <help@castellanidavide.it>  Tue, 12 Apr 2022 23:00:00 +1000

des (00.02) focal; urgency=medium

  * Fixed a bug

 -- Castellani Davide <help@castellanidavide.it>  Fri, 8 Apr 2022 21:30:00 +1000

des (00.01) focal; urgency=medium

  * Initial version

 -- Castellani Davide <help@castellanidavide.it>  Fri, 8 Apr 2022 21:15:00 +1000

---
Made w/ :heart: by Castellani Davide

If you want to contribute you can start with:
- [Discussion](https://github.com/CastellaniDavide/des/discussions)
- [Issue](https://github.com/CastellaniDavide/des/issues/new)
