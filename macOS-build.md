# Building on macOS (Apple Silicon)

Notes for building this project on a MacBook Pro (arm64, macOS 26.6), bash / `~/.bash_profile`.

The project is a **standard devkitPro 3DS template build**. Nothing in the engine or
game sources is Windows-specific — all asset paths use `romfs:/`, and the `.t3x`
graphics are pre-built and committed. The only thing missing on a fresh Mac is
devkitPro itself.

> **No Homebrew route exists.** No formula, no cask, and the old
> `devkitpro/homebrew-devkitpro` tap is gone (404). The `.pkg` installer is the
> only official macOS path.

> **Rosetta 2 is NOT required.** The installer is dual-architecture
> (`devkitpro-pacman.arm64.pkg` + `devkitpro-pacman.x86_64.pkg`, selected by the
> Distribution script), and devkitPro publishes native arm64 macOS host binaries
> at `pkg.devkitpro.org/packages/osx/arm64` (repo DB last updated 2026-06-10).
> Any guide claiming macOS is x86_64-only is outdated.

## 1. Xcode command line tools

```bash
xcode-select --install
```

## 2. Install devkitPro pacman

**Download with `curl`, not a browser.** The installer is unsigned
(`pkgutil --check-signature` → "no signature"), so a browser-downloaded copy gets
`com.apple.quarantine` and Gatekeeper moves it to the Trash with a malware
warning — this is an open bug ([pacman#65](https://github.com/devkitPro/pacman/issues/65)).
curl-downloaded files don't get the quarantine attribute.

```bash
curl -L -o ~/Downloads/devkitpro-pacman-installer.pkg \
  https://github.com/devkitPro/pacman/releases/latest/download/devkitpro-pacman-installer.pkg
sudo installer -pkg ~/Downloads/devkitpro-pacman-installer.pkg -target /
```

If you already downloaded it via a browser:
`sudo xattr -dr com.apple.quarantine ~/Downloads/devkitpro-pacman-installer.pkg`

Current release is v6.0.2, but the asset was **re-uploaded 2025-09-21** to fix
Apple Silicon. Older copies were a bootstrap that silently installed nothing on
arm64 ([#60](https://github.com/devkitPro/pacman/issues/60),
[#49](https://github.com/devkitPro/pacman/issues/49)). The current asset is ~65 MB
and self-contained — if you have an old download, re-fetch it. Install log:
`/private/tmp/devkitpro.log`.

## 3. Install the 3DS toolchain

```bash
sudo dkp-pacman -S 3ds-dev
```

The `3ds-dev` group includes `devkitARM` (r68), `libctru` 2.7.0, `citro3d` 1.7.1,
`citro2d` 1.7.0, `3ds-cmake`, `devkitarm-rules` and `3ds-examples` — verified from
the PKGBUILDs. That covers all three libs the `Makefile` links
(`-lcitro2d -lcitro3d -lctru -lm`). No separate citro2d/citro3d install needed.

Optional: `sudo dkp-pacman -S 3ds-portlibs` (SDL, zlib, freetype, libpng, …).

### If this fails with a 403

**This is currently the most likely failure.** devkitPro's Cloudflare WAF is
misidentifying pacman traffic as scrapers and blocking real users — open issues
[#62](https://github.com/devkitPro/pacman/issues/62),
[#63](https://github.com/devkitPro/pacman/issues/63),
[#64](https://github.com/devkitPro/pacman/issues/64). (I hit this myself while
researching these notes — every request to `pkg.devkitpro.org` from this network
returned 403.) The maintainer's workaround is to set a user-agent via
`XferCommand`, using wget rather than curl:

```bash
brew install wget
sudo nano /opt/devkitpro/pacman/etc/pacman.conf
```

Uncomment/replace the `XferCommand` line with (note the Homebrew path — macOS has
no `/usr/bin/wget`):

```
XferCommand = /opt/homebrew/bin/wget -U "dkp-pacman" --passive-ftp -c -O %o %u
```

Then:

```bash
sudo rm -f /opt/devkitpro/pacman/var/lib/pacman/sync/dkp-*
sudo dkp-pacman -Syy
```

**Do not loop retries** — repeated hammering gets your IP firewalled. If banned,
mail support@devkitpro.org with your IP. There is currently no mirror.

Keyring errors (unknown trust, `call to execv failed`) — the postinstall already
does this, but to redo it manually:

```bash
sudo dkp-pacman-key --init
sudo dkp-pacman-key --populate devkitpro
```

### Checking for updates later

```bash
# What's installed right now
dkp-pacman -Q | grep -E 'devkitARM|devkitarm|libctru|citro|3dstools|tex3ds'

# Refresh the package database, then list what has updates available
sudo dkp-pacman -Sy
dkp-pacman -Qu            # prints nothing if everything is current

# Apply all updates
sudo dkp-pacman -Syu
```

Notes:

- **Always finish with `-Syu`, never `-Sy` followed by `-S <pkg>`.** Refreshing the
  DB and then installing a single package gives you a partial upgrade, which is
  the classic way to end up with mismatched libctru/citro3d versions.
- The Cloudflare 403 issue (above) applies to every one of these — they all hit
  `pkg.devkitpro.org`. Same workaround.
- **After a `libctru`/`citro2d`/`citro3d` bump, rebuild from scratch:**
  ```bash
  make clean-all
  make GAME=bomberman
  ```
  The build's `.d` dependency files don't track the toolchain's own headers, so a
  stale `build/` can link old objects against new headers.
- `dkp-pacman` itself (the `.pkg`) updates separately — re-run the installer from
  `releases/latest` if a new one ships.

Known-good versions on this machine as of 2026-08-03, for comparison if an update
ever breaks something:

| Package | Version |
|---|---|
| `devkitARM` | r68-1 |
| `devkitarm-gcc` | 16.1.0-1 |
| `devkitarm-binutils` | 2.46.0-1 |
| `devkitarm-newlib` | 4.6.0.20260123-5 |
| `devkitarm-rules` | 1.6.0-4 |
| `libctru` | 2.7.0-1 |
| `citro3d` | 1.7.1-2 |
| `citro2d` | 1.7.0-1 |
| `3dstools` | 1.3.1-3 |
| `tex3ds` | 2.3.0-4 |
| `picasso` | 2.7.2-3 |
| `3dslink` | 0.6.3-1 |

Roll back a bad update with `sudo dkp-pacman -U /opt/devkitpro/pacman/var/cache/pacman/pkg/<pkg>-<oldver>-*.pkg.tar.xz`
(cached packages from previous installs live there).

## 4. Set the environment

The `Makefile` hard-errors without `DEVKITARM`:

```
ifeq ($(strip $(DEVKITARM)),)
    $(error "Please set DEVKITARM in your environment: ...")
endif
```

**The macOS installer does not write a profile script.** (The wiki's
`/etc/profile.d/devkit-env.sh` is Linux-only; there's no `/etc/profile.d` on
macOS.) Instead, it installs `/Library/LaunchAgents/devkit.env.plist` which runs
`launchctl setenv` for `DEVKITPRO`/`DEVKITARM`/`DEVKITPPC`, plus
`/etc/paths.d/devkitpro-tools` for `/opt/devkitpro/tools/bin`.

Two consequences:

1. **You must reboot (or log out and back in)** for the LaunchAgent to run — the
   vars will not appear in already-open terminals.
2. `launchctl setenv` vars **don't reach SSH sessions or `sudo` environments**.

So set them explicitly anyway. Add to `~/.bash_profile`:

```bash
export DEVKITPRO=/opt/devkitpro
export DEVKITARM=${DEVKITPRO}/devkitARM
export DEVKITPPC=${DEVKITPRO}/devkitPPC
export PATH=${DEVKITPRO}/tools/bin:$PATH
```

Reload and verify:

```bash
source ~/.bash_profile
echo $DEVKITARM                          # -> /opt/devkitpro/devkitARM
3dsxtool --help                          # in tools/bin
$DEVKITARM/bin/arm-none-eabi-gcc --version   # note the full path
```

> **`arm-none-eabi-gcc: command not found` is expected, not an error.** The
> compiler lives in `/opt/devkitpro/devkitARM/bin`, which the `PATH` line above
> deliberately does *not* add — only `tools/bin` (`3dsxtool`, `smdhtool`,
> `tex3ds`, `3dslink`) goes on your interactive PATH, matching what devkitPro's
> own `/etc/paths.d/devkitpro-tools` does. `make` adds the compiler directory
> itself; `/opt/devkitpro/devkitARM/base_tools` line 10 reads:
>
> ```make
> export PATH := $(DEVKITPATH)/tools/bin:$(DEVKITPATH)/devkitARM/bin:$(PATH)
> ```
>
> So call it by full path to check it, or just go to step 5 — a successful build
> proves the compiler works. If you *want* it on your PATH for direct use, add
> `export PATH=${DEVKITARM}/bin:$PATH` to `~/.bash_profile`; it's optional and
> not needed for building.

**Never add `/opt/devkitpro/pacman/bin` to PATH** — it clashes with system tools.
The installer puts `dkp-pacman`, `dkp-makepkg` etc. wrappers in `/usr/local/bin`,
which is already first in `/etc/paths`. If you get `dkp-pacman: command not found`,
check `ls -l /usr/local/bin/dkp-pacman` and that `/usr/local/bin` is in `$PATH`
(open issue [#67](https://github.com/devkitPro/pacman/issues/67)).

## 5. Build

**Run this from the project root** — `make` looks for the makefile in the current
directory:
```
make GAME=bomberman     # or: platformer, blank
```

Output lands at `build/<game>/<game>.3dsx`.

> `make: *** No targets specified and no makefile found.  Stop.` means you're in
> the wrong directory — `make` never found the makefile. (If you were in the right
> place but missing `DEVKITARM`, you'd get the Makefile's own
> `"Please set DEVKITARM in your environment"` error instead. The two are easy to
> confuse.)

Other targets:

```bash
make info GAME=bomberman     # print resolved paths / file lists
make clean GAME=bomberman
make clean-all
```

macOS `/usr/bin/make` is **GNU Make 3.81**, not BSD make, and `devkitarm-rules`
uses no GNU Make 4.x-only constructs — so this should work as-is.
`brew install make` + `gmake` is only a fallback.

## 6. Running it — use Azahar, not Citra

**The README's Citra recommendation is dead.** citra-emu.org serves only the
discontinuation notice and the GitHub repo 404s. Lime3DS is archived and merged
into Azahar.

**[Azahar](https://azahar-emu.org/)** ([GitHub](https://github.com/azahar-emu/azahar))
is the actively maintained successor. Native arm64 build, requires macOS 13.4+,
and loads `.3dsx` directly (`AppLoader_THREEDSX` in `src/core/loader/loader.cpp`).

```bash
curl -L -o ~/Downloads/azahar.zip \
  https://github.com/azahar-emu/azahar/releases/download/2125.1.3/azahar-macos-arm64-2125.1.3.zip
unzip ~/Downloads/azahar.zip -d ~/Downloads/azahar
cp -R ~/Downloads/azahar/azahar-macos-arm64-2125.1.3/Azahar.app /Applications/
xattr -dr com.apple.quarantine /Applications/Azahar.app
open -a Azahar build/bomberman/bomberman.3dsx
```

The `xattr` line is needed — Azahar is ad-hoc signed and not notarized, so it's
blocked on first launch.

**On real hardware:** `3dslink` ships with the toolchain —
`3dslink -a <3ds-ip> build/bomberman/bomberman.3dsx` sends it to the Homebrew
Launcher.

[Panda3DS](https://github.com/wheremyfoodat/Panda3DS) is a less mature HLE
alternative; fine as a secondary check, not the primary target.

## 7. Repo notes (macOS-specific)

- **Assets need no rebuild.** The `.t3x` files are already committed under each
  game's `romfs/gfx/`. `games/bomberman/gfx/build_gfx.sh` is bash and works on
  Mac, but needs ImageMagick (`brew install imagemagick`) for `magick` if you ever
  regenerate them.
- **`tools/sprite_sheet_slicer.bat` is Windows-only.** Use `python3 tools/slice.py`
  directly. Needs Pillow: `pip3 install Pillow`.
- **`CMakeLists.txt` used to hardcode `C:/devkitPro/...` include paths** — fixed
  2026-08-03, now `$ENV{DEVKITPRO}/libctru/include` and
  `$ENV{DEVKITPRO}/devkitARM/arm-none-eabi/include`. This file is for IDE
  indexing only; the real build is the `Makefile`. Note that CLion must be
  launched with `DEVKITPRO` in its environment for this to resolve — if you start
  it from Spotlight rather than a shell, it may not inherit your
  `~/.bash_profile`. Launching via `open -a CLion` from a terminal works.
- **Stale CMake caches** in `build/` and `cmake-build-debug/` from earlier local
  runs. They don't affect `make` (which builds into `build/<game>/`), but delete
  them if CMake/CLion acts up.

## 8. README changes — applied 2026-08-03

- Replaced the `[Citra](https://citra-emu.org/)` link with Azahar — the old link
  pointed at a takedown notice.
- Replaced the third-party GBAtemp guide with
  <https://devkitpro.org/wiki/Getting_Started>, plus a `dkp-pacman -S 3ds-dev`
  step and a link to this file.
- Documented the `DEVKITPRO`/`DEVKITARM` exports — the Makefile hard-errors
  without `DEVKITARM`.
- Fenced the `make GAME=<name>` command. It was unfenced, so `<name>` was parsed
  as an HTML tag and rendered as `make GAME=` on GitHub.

## 9. Verification status

**The whole path — install, build, run — is confirmed working on Apple Silicon
macOS (2026-08-03).**

- **Steps 1–4 (install):** completed on two separate Macs.
- **Step 5 (build): all three games build clean**, with no warnings despite
  `-Wall`. GNU Make 3.81 handled it with no `gmake` fallback needed. `file`
  identifies each output as "Nintendo 3DS Homebrew Application (3DSX)":

  | Game | Sources | Output |
  |---|---|---|
  | `bomberman` | 24 | `build/bomberman/bomberman.3dsx` — 224K |
  | `platformer` | 23 | `build/platformer/platformer.3dsx` — 219K |
  | `blank` | 21 | `build/blank/blank.3dsx` — 108K |

- **Step 6 (run):** the `bomberman` build **runs in Azahar** on Apple Silicon.

Also confirmed by the build: the host tools (`3dsxtool`, `smdhtool`, `tex3ds`,
`picasso`, `3dslink`) **are** included in `3ds-dev` — all present in
`/opt/devkitpro/tools/bin`, alongside `libcitro2d.a`, `libcitro3d.a` and
`libctru.a` in `/opt/devkitpro/libctru/lib`.

Still unverified:

1. **The 403 workaround in §3** — not needed on this network in the end, so the
   `XferCommand` fix is from the maintainer's issue thread rather than tested
   here. Keep it in mind if a future `dkp-pacman` call fails.
2. **Real hardware via `3dslink`** (§6) — untested; only the emulator path has
   been exercised.
3. **Running `platformer` and `blank`** — both compile and link cleanly, but only
   `bomberman` has actually been launched. A clean build proves the toolchain
   works, not that the games do.

## 10. Don't "fix" `-ffunction-sections` — a false alarm

Reading the `Makefile` on its own suggests a missed optimisation:

```make
CFLAGS  := -Wall -O2 -ffunction-sections ...
LDFLAGS  = -specs=3dsx.specs -g $(ARCH) -Wl,-Map,$(notdir $*.map)
```

`-ffunction-sections` gives each function its own section so the linker *can*
discard unused ones — but only when told to with `--gc-sections`, which appears
nowhere in `LDFLAGS`. It looks like the flag is doing nothing.

**It isn't. Dead-code stripping is already on.** devkitPro enables it inside the
specs file that `LDFLAGS` pulls in via `-specs=3dsx.specs`
(`/opt/devkitpro/devkitARM/arm-none-eabi/lib/3dsx.specs`):

```
*link:
+ -T 3dsx.ld%s -d --emit-relocs --use-blx --gc-sections
```

Measured, not assumed — rebuilding `bomberman` with an explicit
`-Wl,--gc-sections` produces a **byte-identical** binary (229,460 bytes; text
198,976 / data 7,576 / bss 20,904), because the flag was already in effect. A
control run with a deliberately invalid `-Wl,--bogus-flag-xyz` failed at the
linker, confirming the override really did reach it.

Adding `-fdata-sections` (genuinely absent) was also measured: it saves **24
bytes** of a 229 KB binary. Not worth the churn.

**Conclusion: the Makefile's optimisation flags are correct as they stand.**
Recorded here because the Makefile reads like a bug in isolation, and the specs
file is easy to miss — the check costs one `cat` and saves a pointless MR.
