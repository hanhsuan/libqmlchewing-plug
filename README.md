# Bridge between QML and [libchewing](https://codeberg.org/chewing/libchewing)
A very simple bridge between QML and libchewing that allows developers to create a QML keyboard for libchewing and is not limited to Sailfish OS. If you need a more complex version, the [Maliit Keyboard plugin](https://github.com/maliit/keyboard/tree/master/plugins/chewing) for QML is a good example.

# How to build

## Build without Sailfish OS SDK

### Install dependencies

```bash
DEBIAN_FRONTEND=noninteractive apt install qtbase5-dev qtdeclarative5-dev make g++ libchewing3-dev rpm dpkg-dev git
```

### Clone source code

```bash
git clone https://github.com/hanhsuan/libqmlchewing-plug.git
```

### Build with qmake only

```bash
cd libqmlchewing-plug

# Remove the CONFIG+=unittest will skip the unit test
qmake CONFIG+=unittest

make

# Executing the unittest
QT_QPA_PLATFORM=offscreen LD_LIBRARY_PATH=qmlchewing:$LD_LIBRARY_PATH ./tests/tests
```

### Build with rpm only

This example shows how to build rpm package on the ubuntu/debain.
* --nodeps: The package manager isn’t the same; skip the dependencies check.
* --define: Replace the defalut directories to the right place.
* --with unittest: Build unittest

```bash
cd libqmlchewing-plug

rpmbuild --nodeps --define "_topdir $PWD" --define "_sourcedir $PWD/qmlchewing" --define "_builddir $PWD"  --define "qmake5 $(which qmake)" --with unittest -bb rpm/qmlchewing.spec

# Executing the unittest
QT_QPA_PLATFORM=offscreen LD_LIBRARY_PATH=qmlchewing:$LD_LIBRARY_PATH ./tests/tests
```

## Build with Sailfish OS SDK 3.13.5

Install the libchewing package in a development environment, built from [source code](https://github.com/hanhsuan/SailfishOS-libchewing) by yourself, or use the prebuilt [one](https://github.com/hanhsuan/SailfishOS-libchewing/releases). The architecture should be the same as your environment.

### container

* Enter the container with root permission

```bash
sfdk build-shell --maintain
```

* Install the rpm package (Choose the architecture that works best for you)

```bash
zypper in libchewing-devel-0.8.5-0.aarch64.rpm
```

* Back to the host and build

```bash
git clone https://github.com/hanhsuan/libqmlchewing-plug.git

cd libqmlchewing-plug

# Remove the -- --with unittest will skip the unit test
sfdk build -- --with unittest
```

## How to install the rpm in the Sailfish OS VM

* Start the VM first

* Copy rpm pakcage to the VM

```bash
scp -p 2223 -i ~/SailfishOS/vmshare/ssh/private_keys/sdk libchewing-0.8.5-0.i486.rpm root@localhost:/home/defaultuser/
```

* ssh into the VM to install the rpm package

```bash
ssh -p 2223 -i ~/SailfishOS/vmshare/ssh/private_keys/sdk root@localhost

pkcon install-local libchewing-0.8.5-0.i486.rpm
```

## Thanks
Thank [Arvid](https://github.com/ecryth/libanthy-qml-plugin) for providing a good example that helped me understand how to do it back in 2016.
