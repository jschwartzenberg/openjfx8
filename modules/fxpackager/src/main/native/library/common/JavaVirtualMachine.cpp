/*
 * Copyright (c) 2014, 2015, Oracle and/or its affiliates.
 * All rights reserved. Use is subject to license terms.
 *
 * This file is available and licensed under the following license:
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 *  - Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  - Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the distribution.
 *  - Neither the name of Oracle Corporation nor the names of its
 *    contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */


#include "JavaVirtualMachine.h"
#include "Platform.h"
#include "PlatformString.h"
#include "FilePath.h"
#include "Package.h"
#include "Java.h"
#include "Helpers.h"
#include "Messages.h"
#include "Macros.h"
#include "PlatformThread.h"

#include "jni.h"

#include <map>
#include <list>


// Private typedef for function pointer casting
#ifndef USE_JLI_LAUNCH
#define LAUNCH_FUNC "JNI_CreateJavaVM"
typedef jint (JNICALL *JVM_CREATE)(JavaVM ** jvm, JNIEnv ** env, void *);
#else
#define LAUNCH_FUNC "JLI_Launch"
typedef int (JNICALL *JVM_CREATE)(int argc, char ** argv,
                                    int jargc, const char** jargv,
                                    int appclassc, const char** appclassv,
                                    const char* fullversion,
                                    const char* dotversion,
                                    const char* pname,
                                    const char* lname,
                                    jboolean javaargs,
                                    jboolean cpwildcard,
                                    jboolean javaw,
                                    jint ergo);
#endif //USE_JLI_LAUNCH

class JavaLibrary : public Library {
    JVM_CREATE FCreateProc;

    JavaLibrary(const TString &FileName);

public:
    JavaLibrary() : Library() {
        FCreateProc = NULL;
    }

#ifndef USE_JLI_LAUNCH
bool JavaVMCreate(JavaVM** jvm, JNIEnv** env, void* jvmArgs) {
        bool result = true;

        if (FCreateProc == NULL) {
            FCreateProc = (JVM_CREATE)GetProcAddress(LAUNCH_FUNC);
        }

        if (FCreateProc == NULL) {
            Platform& platform = Platform::GetInstance();
            Messages& messages = Messages::GetInstance();
            platform.ShowMessage(messages.GetMessage(FAILED_LOCATING_JVM_ENTRY_POINT));
            return false;
        }

        if ((*FCreateProc)(jvm, env, jvmArgs) < 0) {
            Platform& platform = Platform::GetInstance();
            Messages& messages = Messages::GetInstance();
            platform.ShowMessage(messages.GetMessage(FAILED_CREATING_JVM));
            return false;
        }

        return result;
    }
#else
    bool JavaVMCreate(size_t argc, char *argv[]) {
        if (FCreateProc == NULL) {
            FCreateProc = (JVM_CREATE)GetProcAddress(LAUNCH_FUNC);
        }

        if (FCreateProc == NULL) {
            Platform& platform = Platform::GetInstance();
            Messages& messages = Messages::GetInstance();
            platform.ShowMessage(messages.GetMessage(FAILED_LOCATING_JVM_ENTRY_POINT));
            return false;
        }

        return FCreateProc((int)argc, argv,
            0, NULL,
            0, NULL,
            "",
            "",
            "java",
            "java",
            false,
            false,
            false,
            0) == 0;
    }
#endif //USE_JLI_LAUNCH
};

#ifndef USE_JLI_LAUNCH
//debug hook to print JVM messages into console.
static jint JNICALL vfprintfHook(FILE *fp, const char *format, va_list args) {
#ifdef WINDOWS
   char buffer[20480];
   int len;
   HANDLE hConsole;
   DWORD wasWritten;

   len = _vsnprintf_s(buffer, sizeof(buffer), sizeof(buffer), format, args);

   if (len <= 0) {
        return len;
   }

   hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

   if (hConsole == INVALID_HANDLE_VALUE) {
        return false;
   }

   //JVM will always pass us ASCII
   WriteConsoleA(hConsole, buffer, strlen(buffer), &wasWritten, NULL);

   return (jint) len;
#endif //WINDOWS
#ifdef LINUX
   return 0;
#endif //LINUX
}
#endif //USE_JLI_LAUNCH

//--------------------------------------------------------------------------------------------------

struct JavaOptionItem {
    TString name;
    TString value;
    void* extraInfo;
};


class JavaOptions {
private:
    std::list<JavaOptionItem> FItems;
    JavaVMOption* FOptions;

public:
    JavaOptions() {
        FOptions = NULL;

#ifndef USE_JLI_LAUNCH
#ifdef DEBUG
        Platform& platform = Platform::GetInstance();

        if (platform.GetDebugState() == dsNative) {
            AppendValue(_T("vfprintf"), _T(""), (void*)vfprintfHook);
        }
#endif //DEBUG
#endif //USE_JLI_LAUNCH
    }

    ~JavaOptions() {
        if (FOptions != NULL) {
            for (unsigned int index = 0; index < GetCount(); index++) {
                delete[] FOptions[index].optionString;
            }

            delete[] FOptions;
        }
    }

    void AppendValue(const TString Key, TString Value) {
        AppendValue(Key, Value, NULL);
    }

    void AppendValue(const TString Key, TString Value, void* Extra) {
        JavaOptionItem item;
        item.name = Key;
        item.value = Value;
        item.extraInfo = Extra;
        FItems.push_back(item);
    }

    void AppendValues(TOrderedMap Values) {
        std::list<TString> orderedKeys = Helpers::GetOrderedKeysFromMap(Values);
        
        for (std::list<TString>::const_iterator iterator = orderedKeys.begin(); iterator != orderedKeys.end(); iterator++) {
            TString name = *iterator;
            TValueIndex value = Values[name];
            AppendValue(name, value.value);
        }
    }
    
    void ReplaceValue(const TString Key, TString Value) {
        for (std::list<JavaOptionItem>::iterator iterator = FItems.begin();
             iterator != FItems.end(); iterator++) {
            
            TString lkey = iterator->name;
            
            if (lkey == Key) {
                JavaOptionItem item = *iterator;
                item.value = Value;
                iterator = FItems.erase(iterator);
                FItems.insert(iterator, item);
                break;
            }
        }
    }

#ifndef USE_JLI_LAUNCH
    JavaVMOption* ToJavaOptions() {
        FOptions = new JavaVMOption[FItems.size()];
        memset(FOptions, 0, sizeof(JavaVMOption) * FItems.size());
        Macros& macros = Macros::GetInstance();
        unsigned int index = 0;
        
        for (std::list<JavaOptionItem>::const_iterator iterator = FItems.begin();
             iterator != FItems.end(); iterator++) {
            TString key = iterator->name;
            TString value = iterator->value;
            TString option = Helpers::NameValueToString(key, value);
            option = macros.ExpandMacros(option);
#ifdef DEBUG
            printf("%s\n", PlatformString(option).c_str());
#endif //DEBUG
            FOptions[index].optionString = PlatformString::duplicate(PlatformString(option).c_str());
            FOptions[index].extraInfo = iterator->extraInfo;
            index++;
        }

        return FOptions;
    }
#else
    std::list<TString> ToList() {
        std::list<TString> result;
        Macros& macros = Macros::GetInstance();
        
        for (std::list<JavaOptionItem>::const_iterator iterator = FItems.begin();
             iterator != FItems.end(); iterator++) {
            TString key = iterator->name;
            TString value = iterator->value;
            TString option = Helpers::NameValueToString(key, value);
            option = macros.ExpandMacros(option);
            result.push_back(option);
        }

        return result;
    }
#endif //USE_JLI_LAUNCH

    size_t GetCount() {
        return FItems.size();
    }
};

// jvmuserargs can have a trailing equals in the key. This needs to be removed to use
// other parts of the launcher.
TOrderedMap RemoveTrailingEquals(TOrderedMap Map) {
    TOrderedMap result;

    for (TOrderedMap::const_iterator iterator = Map.begin(); iterator != Map.end(); iterator++) {
        TString name = iterator->first;
        TValueIndex value = iterator->second;

        // If the last character of the key is an equals, then remove it. If there is no
        // equals then combine the two as a key.
        TString::iterator i = name.end();
        i--;

        if (*i == '=') {
            name = name.substr(0, name.size() - 1);
        }
        else {
            i = value.value.begin();
            
            if (*i == '=') {
                value.value = value.value.substr(1, value.value.size() - 1);
            }
            else {
                name = name + value.value;
                value.value = _T("");
            }
        }

        result.insert(TOrderedMap::value_type(name, value));
    }

    return result;
}

//--------------------------------------------------------------------------------------------------

JavaVirtualMachine::JavaVirtualMachine() {
#ifndef USE_JLI_LAUNCH
    FEnv = NULL;
    FJvm = NULL;
#endif //USE_JLI_LAUNCH
}

JavaVirtualMachine::~JavaVirtualMachine(void) {
}

bool JavaVirtualMachine::StartJVM() {
    Platform& platform = Platform::GetInstance();
    Package& package = Package::GetInstance();

    TString classpath = package.GetClassPath();

    JavaOptions options;
    options.AppendValue(_T("-Djava.class.path"), classpath);
    options.AppendValue(_T("-Djava.library.path"), package.GetPackageAppDirectory() + FilePath::PathSeparator() + package.GetPackageLauncherDirectory());
    options.AppendValue(_T("-Djava.launcher.path"), package.GetPackageLauncherDirectory());
    options.AppendValue(_T("-Dapp.preferences.id"), package.GetAppID());
    options.AppendValues(package.GetJVMArgs());
    options.AppendValues(RemoveTrailingEquals(package.GetJVMUserArgs()));
 
#ifdef DEBUG
    if (package.Debugging() == dsJava) {
        options.AppendValue(_T("-Xdebug"), _T(""));
        options.AppendValue(_T("-Xrunjdwp:transport=dt_socket,server=y,suspend=y,address=localhost:8000"), _T(""));
        platform.ShowMessage(_T("localhost:8000"));
    }
#endif //DEBUG

    TString maxHeapSizeOption;
    TString minHeapSizeOption;


    if (package.GetMemoryState() == PackageBootFields::msAuto) {
        TPlatformNumber memorySize = package.GetMemorySize();
        TString memory = PlatformString((size_t)memorySize).toString() + _T("m");
        maxHeapSizeOption = TString(_T("-Xmx")) + memory;
        options.AppendValue(maxHeapSizeOption, _T(""));

        if (memorySize > 256)
            minHeapSizeOption = _T("-Xms256m");
        else
            minHeapSizeOption = _T("-Xms") + memory;

        options.AppendValue(minHeapSizeOption, _T(""));
    }

    TString mainClassName = package.GetMainClassName();

    if (mainClassName.empty() == true) {
        Messages& messages = Messages::GetInstance();
        platform.ShowMessage(messages.GetMessage(NO_MAIN_CLASS_SPECIFIED));
        return false;
    }

    JavaLibrary javaLibrary;
    javaLibrary.AddDependencies(platform.FilterOutRuntimeDependenciesForPlatform(platform.GetLibraryImports(package.GetJVMLibraryFileName())));
    javaLibrary.Load(package.GetJVMLibraryFileName());
    
#ifndef USE_JLI_LAUNCH
    if (package.HasSplashScreen() == true) {
        options.AppendValue(TString(_T("-splash:")) + package.GetSplashScreenFileName(), _T(""));
    }

    // Set up the VM init args
    JavaVMInitArgs jvmArgs;
    memset(&jvmArgs, 0, sizeof(JavaVMInitArgs));
    jvmArgs.version = JNI_VERSION_1_6;
    jvmArgs.options = options.ToJavaOptions();
    jvmArgs.nOptions = (jint)options.GetCount();
    jvmArgs.ignoreUnrecognized = JNI_TRUE;

    if (javaLibrary.JavaVMCreate(&FJvm, &FEnv, &jvmArgs) == true) {
        try {
            JavaClass mainClass(FEnv, Helpers::ConvertIdToJavaPath(mainClassName));
            JavaStaticMethod mainMethod = mainClass.GetStaticMethod(_T("main"), _T("([Ljava/lang/String;)V"));
            std::list<TString> appargs = package.GetArgs();
            JavaStringArray largs(FEnv, appargs);
            
            package.FreeBootFields();
            
            mainMethod.CallVoidMethod(1, largs.GetData());
            return true;
        }
        catch (JavaException& exception) {
            platform.ShowMessage(PlatformString(exception.what()).toString());
            return false;
        }
    }

    return false;
}
#else
    // Initialize the arguments to JLI_Launch()
    //
    // On Mac OS X JLI_Launch spawns a new thread that actually starts the JVM. This
    // new thread simply re-runs main(argc, argv). Therefore we do not want
    // to add new args if we are still in the original main thread so we
    // will treat them as command line args provided by the user ...
    // Only propagate original set of args first time.

    options.AppendValue(Helpers::ConvertPathToId(mainClassName), _T(""));

    std::list<TString> vmargs;
    vmargs.push_back(package.GetCommandName());

    // Mac adds a ProcessSerialNumber to args when launched from .app
    // filter out the psn since they it's not expected in the app
    if (platform.IsMainThread() == false) {
        //TODO shows a splash screen, does not work on Windows, and it does not go away and
        // it hangs the process.
        if (package.HasSplashScreen() == true) {
            options.AppendValue(TString(_T("-splash:")) + package.GetSplashScreenFileName(), _T(""));
        }

        std::list<TString> loptions = options.ToList();
        vmargs.splice(vmargs.end(), loptions, loptions.begin(), loptions.end());
    }

    std::list<TString> largs = package.GetArgs();
    vmargs.splice(vmargs.end(), largs, largs.begin(), largs.end());
    size_t argc = vmargs.size();
    DynamicBuffer<char*> argv(argc+1);
    unsigned int index = 0;

    for (std::list<TString>::const_iterator iterator = vmargs.begin();
         iterator != vmargs.end(); iterator++) {
        TString item = *iterator;
        std::string arg = PlatformString(item).toStdString();
#ifdef DEBUG
        printf("%i %s\n", index, arg.c_str());
#endif //DEBUG
        argv[index] = PlatformString::duplicate(arg.c_str());
        index++;
    }

    argv[argc] = NULL;

    // On Mac we can only free the boot fields if the calling thread is not the main thread.
#ifdef MAC
    if (platform.IsMainThread() == false) {
        package.FreeBootFields();
    }
#endif //MAC

    if (javaLibrary.JavaVMCreate(argc, argv.GetData()) == true) {
        return true;
    }

    for (index = 0; index < argc; index++) {
        if (argv[index] != NULL) {
            delete[] argv[index];
        }
    }

    return false;
}
#endif //USE_JLI_LAUNCH

void JavaVirtualMachine::ShutdownJVM() {
#ifndef USE_JLI_LAUNCH
    if (FJvm != NULL) {
        // If application main() exits quickly but application is run on some other thread
        //  (e.g. Swing app performs invokeLater() in main and exits)
        // then if we return execution to tWinMain it will exit.
        // This will cause process to exit and application will not actually run.
        //
        // To avoid this we are trying to detach jvm from current thread (java.exe does the same)
        // Because we are doing this on the main JVM thread (i.e. one that was used to create JVM)
        // this call will spawn "Destroy Java VM" java thread that will shut JVM once there are
        // no non-daemon threads running, and then return control here.
        // I.e. this will happen when EDT and other app thread will exit.
        if (FJvm->DetachCurrentThread() != JNI_OK) {
            Platform& platform = Platform::GetInstance();
            platform.ShowMessage(_T("Detach failed."));
        }

        FJvm->DestroyJavaVM();
    }
#endif //USE_JLI_LAUNCH
}
