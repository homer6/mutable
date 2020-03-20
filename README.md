
Build Status: [![CircleCI](https://circleci.com/gh/homer6/mutable.svg?style=svg)](https://circleci.com/gh/homer6/mutable)

Overview
--------

Mutable allows you to make schema migrations (schema mutations), data mutations (data migrations), and other mutations in a clean and modern way.

Mutable is designed to work with kubernetes and works with postgres, mongodb, redis, and can support other datastores.



Build MacOS
-----------

```
brew install openssl
brew unlink openssl && brew link openssl --force
cmake .
make
./build/mutable
```


Build Ubuntu
------------

```
apt install gcc g++ cmake pkg-config libssl-dev
cmake .
make
./build/mutable
```
