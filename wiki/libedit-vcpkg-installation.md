# wdb Notes

## libedit

To intall the [libedit](https://vcpkg.io/en/package/libedit) on vcpkg you need to follow these commands below.

### vcpkg Requirements

Move to the `$VCPKG_ROOT` directory

```bash
cd $VCPKG_ROOT
```

Install `libncurses-dev`

```bash
sudo apt-get install -y libncurses-dev
```

Install `libedit` in **vcpkg**

```bash
vcpkg install libedit
```

### Create new application

Create the `vcpkg.json` and `vcpkg-configuration.json` files

```bash
cd [wdb ROOT DIRETORY]
vcpkg --new application
```

Configure `vcpkg` project with the required libraries

```bash
vcpkg add port libedit
```

```bash
vcpkg add port libedit
```


```bash
```


```bash
```


```bash
```

