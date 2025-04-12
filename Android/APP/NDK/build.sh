export ANDROID_SDK_ROOT=$PWD/android-sdk
export PATH=$PATH:$ANDROID_SDK_ROOT/cmdline-tools/latest/bin
export PATH=$PATH:$ANDROID_SDK_ROOT/platform-tools
export PATH=$PATH:$ANDROID_SDK_ROOT/ndk-bundle

TOOLS_URL=https://dl.google.com/android/repository/commandlinetools-win-10406996_latest.zip

if [ ! -d "$ANDROID_SDK_ROOT/cmdline-tools" ]; then
    mkdir -p $ANDROID_SDK_ROOT/cmdline-tools

    wget -c $TOOLS_URL -O cmdline-tools-latest.zip

    unzip cmdline-tools-latest.zip -d $ANDROID_SDK_ROOT/cmdline-tools
    mv $ANDROID_SDK_ROOT/cmdline-tools/cmdline-tools $ANDROID_SDK_ROOT/cmdline-tools/latest

    # 下载平台工具和 NDK
    sdkmanager.bat platform-tools ndk-bundle
fi
