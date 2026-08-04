# Building on macOS (Apple Silicon)

This installation guide provides the steps to install **devkitPro** on macOS.

## 1. Xcode command line tools

```bash
xcode-select --install
```

## 2. Install devkitPro pacman

```bash
curl -L -o ~/Downloads/devkitpro-pacman-installer.pkg \
  https://github.com/devkitPro/pacman/releases/latest/download/devkitpro-pacman-installer.pkg
sudo installer -pkg ~/Downloads/devkitpro-pacman-installer.pkg -target /
```

Use curl instead of a browser to avoid Gatekeeper-related problems.

## 3. Install the 3DS toolchain

```bash
sudo dkp-pacman -S 3ds-dev
```

Optional: `sudo dkp-pacman -S 3ds-portlibs` (SDL, zlib, freetype, libpng, …).

### Checking for updates later

Install all updates:

```bash
sudo dkp-pacman -Syu
```

Rebuild:

```bash
  make clean-all
  make GAME=bomberman
```

Roll back a bad update:

```
sudo dkp-pacman -U /opt/devkitpro/pacman/var/cache/pacman/pkg/<pkg>-<oldver>-*.pkg.tar.xz
```

## 4. Set the environment

Add the following env variables, e.g. to `~/.bash_profile`:

```bash
export DEVKITPRO=/opt/devkitpro
export DEVKITARM=${DEVKITPRO}/devkitARM
export DEVKITPPC=${DEVKITPRO}/devkitPPC
export PATH=${DEVKITPRO}/tools/bin:$PATH
```

Verify:

```bash
source ~/.bash_profile
echo $DEVKITARM
3dsxtool --help
$DEVKITARM/bin/arm-none-eabi-gcc --version
```

## 5. Build targets

Build bomberman:

```
make clean-all
make GAME=bomberman
make info GAME=bomberman
```

Replace 'bomberman' by other games.

## 6. Azahar emulator

Check `https://github.com/azahar-emu/azahar/releases/download` for the latest version
and download the binary appropriate for the CPU architecture your mac.
For apple silicon the architecture is `macos-arm64`. Change the file name below accordingly.

```bash
curl -L -o ~/Downloads/azahar.zip \
  https://github.com/azahar-emu/azahar/releases/download/2125.1.3/azahar-macos-arm64-2125.1.3.zip
unzip ~/Downloads/azahar.zip -d ~/Downloads/azahar
cp -R ~/Downloads/azahar/azahar-macos-arm64-2125.1.3/Azahar.app /Applications/
xattr -dr com.apple.quarantine /Applications/Azahar.app
```

The `xattr` line is needed since Azahar ad-hoc signed and not notarised, so it's blocked
on the first launch by Gatekeeper. (In macOS system settings: search for Gatekeeper.)

Run bomberman on Azahar:

```
open -a Azahar build/bomberman/bomberman.3dsx
```

