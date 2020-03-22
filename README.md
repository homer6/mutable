
Build Status: [![CircleCI](https://circleci.com/gh/homer6/mutable.svg?style=svg)](https://circleci.com/gh/homer6/mutable)

Overview
--------

Mutable allows you to make schema migrations (schema mutations), data mutations (data migrations), and other mutations in a clean and modern way.

Mutable is designed to work with kubernetes and works with postgres, mongodb, redis, and can support other datastores.



Build Ubuntu 18
---------------

```
apt install gcc g++ cmake pkg-config libssl-dev libpqxx-dev
cmake .
make
```


Running 
-------

```
export POSTGRES_CONNECTION="postgresql://username@localhost/dbname?connect_timeout=10&application_name=myapp&ssl=true"
./build/mutable
```
