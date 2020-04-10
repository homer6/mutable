
Build Status: [![CircleCI](https://circleci.com/gh/homer6/mutable.svg?style=svg)](https://circleci.com/gh/homer6/mutable)

Overview
--------

Mutable allows you to make schema migrations (schema mutations), data mutations (data migrations), and other mutations in a clean and modern way.

Mutable is designed to work with kubernetes and works with postgres, mongodb, redis, and can support other datastores.



Build Ubuntu 18
---------------

```
sudo apt install git gcc g++ cmake pkg-config libssl-dev libpqxx-dev libsasl2-dev python
git clone --recursive https://github.com/homer6/mutable.git
cd mutable

cd lib/libmongoc
git checkout debian/1.15.2-1
python build/calc_release_version.py > VERSION_CURRENT
mkdir cmake-build
cd cmake-build
cmake -DENABLE_AUTOMATIC_INIT_AND_CLEANUP=OFF ..
sudo make -j12 install
cd ../../..

cd lib/mongocxx-driver/build
git checkout debian/3.4.1-1
cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/usr/local
sudo make -j12 install
cd ../../..

cmake .
make -j12
```


Running 
-------

```
export POSTGRES_CONNECTION="postgresql://username@localhost/dbname?connect_timeout=10&application_name=myapp&ssl=true"
export MONGO_CONNECTION="mongodb://localhost/"
./build/mutable
```
