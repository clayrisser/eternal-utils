# eternal-utils

[![GitHub stars](https://img.shields.io/github/stars/codejamninja/eternal-utils.svg?style=social&label=Stars)](https://github.com/codejamninja/eternal-utils)

> Eternally persist exported environment variables and aliases

Please ★ this repo if you found it useful ★ ★ ★


## Features

* Persist environment varaibles eternally
* Persist aliases eternally
* Remove persisted environment variables
* Remove persisted aliases
* Compatible with bash
* Compatible with zsh


## Installation

### Mac OS X

```sh
brew install glib make
curl -LO \
  https://github.com/codejamninja/eternal-utils/releases/download/0.1.0/eternal-utils_0.1.0.tar.gz
tar -xzvf eternal-utils_0.1.0.tar.gz
cd eternal-utils
./configure
make
sudo make install
```

### Ubuntu Linux

```sh
sudo apt-get install -y libglib2.0-dev build-essential make
curl -LO \
  https://github.com/codejamninja/eternal-utils/releases/download/0.1.0/eternal-utils_0.1.0.tar.gz
tar -xzvf eternal-utils_0.1.0.tar.gz
cd eternal-utils
./configure
make
sudo make install
```


## Usage

### Eternal export

```sh
eternal-export HELLO=world
```

### Eternal unset

```sh
eternal-unset HELLO
```

### Eternal alias

```sh
eternal-alias hello="echo world"
```

### Eternal unalias

```sh
eternal-unalias hello
```

### Eternal source

```sh
eternal-source ~/.hello.sh
```

### Eternal unsource

```sh
eternal-unsource ~/.hello.sh
```


## Support

Submit an [issue](https://github.com/codejamninja/eternal-utils/issues/new)


## Contributing

Review the [guidelines for contributing](https://github.com/codejamninja/eternal-utils/blob/master/CONTRIBUTING.md)


## License

[MIT License](https://github.com/codejamninja/eternal-utils/blob/master/LICENSE)

[Jam Risser](https://codejam.ninja) © 2018


## Changelog

Review the [changelog](https://github.com/codejamninja/eternal-utils/blob/master/CHANGELOG.md)


## Credits

* [Jam Risser](https://codejam.ninja) - Author


## Support on Liberapay

A ridiculous amount of coffee ☕ ☕ ☕ was consumed in the process of building this project.

[Add some fuel](https://liberapay.com/codejamninja/donate) if you'd like to keep me going!

[![Liberapay receiving](https://img.shields.io/liberapay/receives/codejamninja.svg?style=flat-square)](https://liberapay.com/codejamninja/donate)
[![Liberapay patrons](https://img.shields.io/liberapay/patrons/codejamninja.svg?style=flat-square)](https://liberapay.com/codejamninja/donate)
