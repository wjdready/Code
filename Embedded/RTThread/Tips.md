

### Env 功能


cons --help

```
SCons Options:
  -b, -d, -e, -m, -S, -t, -w, --environment-overrides, --no-keep-going,   
  --no-print-directory, --print-directory, --stop, --touch
                              Ignored for compatibility.
  -c, --clean, --remove       Remove specified targets and dependencies.  
  -C DIR, --directory=DIR     Change to DIR before doing anything.        
  --cache-debug=FILE          Print CacheDir debug info to FILE.
  --cache-disable, --no-cache
                              Do not retrieve built targets from CacheDir.
  --cache-force, --cache-populate
                              Copy already-built targets into the CacheDir.
  --cache-readonly            Do not update CacheDir with built targets.
  --cache-show                Print build actions for files from CacheDir.
  --config=MODE               Controls Configure subsystem: auto, force,
                                cache.
  -D                          Search up directory tree for SConstruct,
                                build all Default() targets.
  --debug=TYPE                Print various types of debugging information:
                                count, duplicate, explain, findlibs, includes,
                                memoizer, memory, objects, pdb, prepare,
                                presub, stacktrace, time, action-timestamps.
  --diskcheck=TYPE            Enable specific on-disk checks.
  --duplicate=DUPLICATE       Set the preferred duplication methods. Must be
                                one of hard-soft-copy, soft-hard-copy,
                                hard-copy, soft-copy, copy
  --enable-virtualenv         Import certain virtualenv variables to SCons
  -f FILE, --file=FILE, --makefile=FILE, --sconstruct=FILE
                              Read FILE as the top-level SConstruct file.
  -h, --help                  Print defined help message, or this one.
  -H, --help-options          Print this message and exit.
  -i, --ignore-errors         Ignore errors from build actions.
  -I DIR, --include-dir=DIR   Search DIR for imported Python modules.
  --ignore-virtualenv         Do not import virtualenv variables to SCons
  --implicit-cache            Cache implicit dependencies
  --implicit-deps-changed     Ignore cached implicit dependencies.
  --implicit-deps-unchanged   Ignore changes in implicit dependencies.
  --interact, --interactive   Run in interactive mode.
  -j N, --jobs=N              Allow N jobs at once.
  -k, --keep-going            Keep going when a target can't be made.
  --max-drift=N               Set maximum system clock drift to N seconds.
  --md5-chunksize=N           Set chunk-size for MD5 signature computation to
                                N kilobytes.
  -n, --no-exec, --just-print, --dry-run, --recon
                              Don't build; just print commands.
  --no-site-dir               Don't search or use the usual site_scons dir.
  --profile=FILE              Profile SCons and put results in FILE.
  -q, --question              Don't build; exit status says if up to date.
  -Q                          Suppress "Reading/Building" progress messages.
  --random                    Build dependencies in random order.
  -s, --silent, --quiet       Don't print commands.
  --site-dir=DIR              Use DIR instead of the usual site_scons dir.
  --stack-size=N              Set the stack size of the threads used to run
                                jobs to N kilobytes.
  --taskmastertrace=FILE      Trace Node evaluation to FILE.
  --tree=OPTIONS              Print a dependency tree in various formats: all,
                                derived, prune, status.
  -u, --up, --search-up       Search up directory tree for SConstruct,
                                build targets at or below current directory.
  -U                          Search up directory tree for SConstruct,
                                build Default() targets from local SConscript.
  -v, --version               Print the SCons version number and exit.
  --warn=WARNING-SPEC, --warning=WARNING-SPEC
                              Enable or disable warnings.
  -Y REPOSITORY, --repository=REPOSITORY, --srcdir=REPOSITORY
                              Search REPOSITORY for source and target files.

Local Options:
  --dist                      make distribution
  --dist-strip                make distribution and strip useless files
  --dist-ide                  make distribution for RT-Thread Studio IDE
  --project-path=PROJECT-PATH
                              set dist-ide project output path
  --project-name=PROJECT-NAME
                              set project name
  --reset-project-config      reset the project configurations to default
  --cscope                    Build Cscope cross reference database. Requires
                                cscope installed.
  --clang-analyzer            Perform static analyze with Clang-analyzer.
                                Requires Clang installed. It is recommended to
                                use with scan-build like this: `scan-build
                                scons --clang-analyzer` If things goes well,
                                scan-build will instruct you to invoke
                                scan-view.
  --buildlib=BUILDLIB         building library of a component
  --cleanlib                  clean up the library by --buildlib
  --target=TARGET             set target project: mdk/mdk4/mdk5/iar/vs/vsc/ua/
                                cdk/ses/makefile/eclipse/codelite/cmake
  --stackanalysis             thread stack static analysis
  --genconfig                 Generate .config from rtconfig.h
  --useconfig=USECONFIG       make rtconfig.h from config file.
  --verbose                   print verbose information during build
  --pyconfig                  Python GUI menuconfig for RT-Thread BSP
  --pyconfig-silent           Don`t show pyconfig window
```

##### 1. [+++] scons --pyconfig

打开 Python 实现的, 带 GUI 的 menuconfig, 非常实用

##### 2. [+++] scons --pyconfig

打开 Python 实现的, 带 GUI 的 menuconfig, 非常实用

##### 3. [+++] scons --pyconfig

打开 Python 实现的, 带 GUI 的 menuconfig, 非常实用

