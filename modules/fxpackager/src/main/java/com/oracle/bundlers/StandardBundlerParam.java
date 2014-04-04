/*
 * Copyright (c) 2014, Oracle and/or its affiliates. All rights reserved.
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * This code is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 only, as
 * published by the Free Software Foundation.  Oracle designates this
 * particular file as subject to the "Classpath" exception as provided
 * by Oracle in the LICENSE file that accompanied this code.
 *
 * This code is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 * version 2 for more details (a copy is included in the LICENSE file that
 * accompanied this code).
 *
 * You should have received a copy of the GNU General Public License version
 * 2 along with this work; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 * Please contact Oracle, 500 Oracle Parkway, Redwood Shores, CA 94065 USA
 * or visit www.oracle.com if you need additional information or have any
 * questions.
 */

package com.oracle.bundlers;

import com.sun.javafx.tools.packager.Log;
import com.sun.javafx.tools.packager.PackagerLib;
import com.sun.javafx.tools.packager.bundlers.BundleParams;
import com.sun.javafx.tools.packager.bundlers.RelativeFileSet;

import java.io.File;
import java.io.IOException;
import java.io.StringReader;
import java.nio.file.Files;
import java.text.MessageFormat;
import java.util.*;
import java.util.function.BiFunction;
import java.util.function.Function;
import java.util.jar.Attributes;
import java.util.jar.JarFile;
import java.util.jar.Manifest;

public class StandardBundlerParam<T> extends BundlerParamInfo<T> {

    private static final ResourceBundle I18N =
            ResourceBundle.getBundle("com.oracle.bundlers.StandardBundlerParam");

    public StandardBundlerParam(String name, String description, String id,
                                Class<T> valueType, String[] fallbackIDs,
                                Function<Map<String, ? super Object>, T> defaultValueFunction,
                                boolean requiresUserSetting,
                                BiFunction<String, Map<String, ? super Object>, T> stringConverter) {
        this.name = name;
        this.description = description;
        this.id = id;
        this.valueType = valueType;
        this.fallbackIDs = fallbackIDs;
        this.defaultValueFunction = defaultValueFunction;
        this.requiresUserSetting = requiresUserSetting;
        this.stringConverter = stringConverter;
    }

    public static final StandardBundlerParam<RelativeFileSet> RUNTIME =
            new StandardBundlerParam<>(
                    I18N.getString("param.runtime.name"),
                    I18N.getString("param.runtime.description"),
                    BundleParams.PARAM_RUNTIME,
                    RelativeFileSet.class,
                    null,
                    params -> null,
                    false,
                    (s, p) -> null
            );

    public static final StandardBundlerParam<RelativeFileSet> APP_RESOURCES =
            new StandardBundlerParam<>(
                    I18N.getString("param.app-resources.name"),
                    I18N.getString("param.app-resource.description"),
                    BundleParams.PARAM_APP_RESOURCES,
                    RelativeFileSet.class,
                    null,
                    null, // no default.  Required parameter
                    false,
                    null // no string translation, tool must provide complex type
            );

    public static final StandardBundlerParam<File> ICON =
            new StandardBundlerParam<>(
                    I18N.getString("param.icon-file.name"),
                    I18N.getString("param.icon-file.description"),
                    BundleParams.PARAM_ICON,
                    File.class,
                    null,
                    params -> null,
                    false,
                    (s, p) -> new File(s)
            );


    public static final StandardBundlerParam<String> MAIN_CLASS =
            new StandardBundlerParam<>(
                    I18N.getString("param.main-class.name"),
                    I18N.getString("param.main-class.description"),
                    BundleParams.PARAM_APPLICATION_CLASS,
                    String.class,
                    null,
                    params -> {
                        extractParamsFromAppResources(params);
                        return (String) params.get(BundleParams.PARAM_APPLICATION_CLASS);
                    },
                    false,
                    (s, p) -> s
            );

    public static final StandardBundlerParam<String> APP_NAME =
            new StandardBundlerParam<>(
                    I18N.getString("param.app-name.name"),
                    I18N.getString("param.app-name.description"),
                    BundleParams.PARAM_NAME,
                    String.class,
                    null,
                    params -> {
                        String s = MAIN_CLASS.fetchFrom(params);
                        if (s == null) return null;

                        int idx = s.lastIndexOf(".");
                        if (idx >= 0) {
                            return s.substring(idx+1);
                        }
                        return s;
                    },
                    true,
                    (s, p) -> s
            );

    public static final StandardBundlerParam<String> VENDOR =
            new StandardBundlerParam<>(
                    I18N.getString("param.vendor.name"),
                    I18N.getString("param.vendor.description"),
                    BundleParams.PARAM_VENDOR,
                    String.class,
                    null,
                    params -> I18N.getString("param.vendor.default"),
                    false,
                    (s, p) -> s
            );

    public static final StandardBundlerParam<String> CATEGORY =
            new StandardBundlerParam<>(
                    I18N.getString("param.category.name"),
                    I18N.getString("param.category.description"),
                    BundleParams.PARAM_CATEGORY,
                    String.class,
                    null,
                    params -> I18N.getString("param.category.default"),
                    false,
                    (s, p) -> s
            );

    public static final StandardBundlerParam<String> DESCRIPTION =
            new StandardBundlerParam<>(
                    I18N.getString("param.description.name"),
                    I18N.getString("param.description.description"),
                    BundleParams.PARAM_DESCRIPTION,
                    String.class,
                    new String[] {APP_NAME.getID()},
                    params -> I18N.getString("param.description.default"),
                    false,
                    (s, p) -> s
            );

    public static final StandardBundlerParam<String> COPYRIGHT =
            new StandardBundlerParam<>(
                    I18N.getString("param.copyright.name"),
                    I18N.getString("param.copyright.description"),
                    BundleParams.PARAM_COPYRIGHT,
                    String.class,
                    null,
                    params -> MessageFormat.format(I18N.getString("param.copyright.default"), new Date()),
                    false,
                    (s, p) -> s
            );

    // note that each bundler is likely to replace this one with their own converter
    public static final StandardBundlerParam<RelativeFileSet> MAIN_JAR =
            new StandardBundlerParam<>(
                    I18N.getString("param.main-jar.name"),
                    I18N.getString("param.main-jar.description"),
                    "mainJar",
                    RelativeFileSet.class,
                    null,
                    params -> {
                        extractParamsFromAppResources(params);
                        return (RelativeFileSet) params.get("mainJar");
                    },
                    false,
                    (s, p) -> {
                        File f = new File(s);
                        return new RelativeFileSet(f.getParentFile(), new LinkedHashSet<>(Arrays.asList(f)));
                    }
            );

    public static final StandardBundlerParam<String> MAIN_JAR_CLASSPATH =
            new StandardBundlerParam<>(
                    I18N.getString("param.main-jar-classpath.name"),
                    I18N.getString("param.main-jar-classpath.description"),
                    "classpath",
                    String.class,
                    null,
                    params -> {
                        extractParamsFromAppResources(params);
                        String cp = (String) params.get("classpath");
                        return cp == null ? "" : cp;
                    },
                    false,
                    (s, p) -> s
            );

    public static final StandardBundlerParam<Boolean> USE_FX_PACKAGING =
            new StandardBundlerParam<>(
                    I18N.getString("param.use-javafx-packaging.name"),
                    I18N.getString("param.use-javafx-packaging.description"),
                    "fxPackaging",
                    Boolean.class,
                    null,
                    params -> {
                        extractParamsFromAppResources(params);
                        Boolean result = (Boolean) params.get("fxPackaging");
                        return (result == null) ? Boolean.FALSE : result;
                    },
                    false,
                    (s, p) -> Boolean.valueOf(s)
            );

    @SuppressWarnings("unchecked")
    public static final StandardBundlerParam<List<String>> JVM_OPTIONS =
            new StandardBundlerParam<>(
                    I18N.getString("param.jvm-options.name"),
                    I18N.getString("param.jvm-options.description"),
                    "jvmOptions",
                    (Class<List<String>>) (Object) List.class,
                    null,
                    params -> Collections.emptyList(),
                    false,
                    (s, p) -> Arrays.asList(s.split("\\s+"))
            );

    @SuppressWarnings("unchecked")
    public static final StandardBundlerParam<Map<String, String>> JVM_PROPERTIES =
            new StandardBundlerParam<>(
                    I18N.getString("param.jvm-system-properties.name"),
                    I18N.getString("param.jvm-system-properties.description"),
                    "jvmProperties",
                    (Class<Map<String, String>>) (Object) Map.class,
                    null,
                    params -> Collections.emptyMap(),
                    false,
                    (s, params) -> {
                        Map<String, String> map = new HashMap<>();
                        try {
                            Properties p = new Properties();
                            p.load(new StringReader(s));
                            for (Map.Entry<Object, Object> entry : p.entrySet()) {
                                map.put((String)entry.getKey(), (String)entry.getValue());
                            }
                        } catch (IOException e) {
                            e.printStackTrace();
                        }
                        return map;
                    }
            );

    @SuppressWarnings("unchecked")
    public static final StandardBundlerParam<Map<String, String>> USER_JVM_OPTIONS =
            new StandardBundlerParam<>(
                    I18N.getString("param.user-jvm-options.name"),
                    I18N.getString("param.user-jvm-options.description"),
                    "userJvmOptions",
                    (Class<Map<String, String>>) (Object) Map.class,
                    null,
                    params -> Collections.emptyMap(),
                    false,
                    (s, params) -> {
                        Map<String, String> map = new HashMap<>();
                        try {
                            Properties p = new Properties();
                            p.load(new StringReader(s));
                            for (Map.Entry<Object, Object> entry : p.entrySet()) {
                                map.put((String)entry.getKey(), (String)entry.getValue());
                            }
                        } catch (IOException e) {
                            e.printStackTrace();
                        }
                        return map;
                    }
            );

    public static final StandardBundlerParam<String> TITLE =
            new StandardBundlerParam<>(
                    I18N.getString("param.title.name"),
                    I18N.getString("param.title.description"), //?? but what does it do?
                    BundleParams.PARAM_TITLE,
                    String.class,
                    new String[] {APP_NAME.getID()},
                    APP_NAME::fetchFrom,
                    false,
                    (s, p) -> s
            );


    // note that each bundler is likely to replace this one with their own converter
    public static final StandardBundlerParam<String> VERSION =
            new StandardBundlerParam<>(
                    I18N.getString("param.version.name"),
                    I18N.getString("param.version.description"),
                    BundleParams.PARAM_VERSION,
                    String.class,
                    null,
                    params -> I18N.getString("param.version.default"),
                    false,
                    (s, p) -> s
            );

    public static final StandardBundlerParam<Boolean> SYSTEM_WIDE =
            new StandardBundlerParam<>(
                    I18N.getString("param.system-wide.name"),
                    I18N.getString("param.system-wide.description"),
                    BundleParams.PARAM_SYSTEM_WIDE,
                    Boolean.class,
                    null,
                    params -> null,
                    false,
                    // valueOf(null) is false, and we actually do want null in some cases
                    (s, p) -> (s == null || "null".equalsIgnoreCase(s))? null : Boolean.valueOf(s)
            );

    public static final StandardBundlerParam<Boolean> SERVICE_HINT  =
            new StandardBundlerParam<>(
                    I18N.getString("param.service-hint.name"),
                    I18N.getString("param.service-hint.description"),
                    BundleParams.PARAM_SERVICE_HINT,
                    Boolean.class,
                    null,
                    params -> false,
                    false,
                    // valueOf(null) is false, and we actually do want null in some cases
                    (s, p) -> (s == null || "null".equalsIgnoreCase(s))? true : Boolean.valueOf(s)
            );

    public static final StandardBundlerParam<Boolean> START_ON_INSTALL  =
            new StandardBundlerParam<>(
                    I18N.getString("param.start-on-install.name"),
                    I18N.getString("param.start-on-install.description"),
                    "startOnInstall",
                    Boolean.class,
                    null,
                    params -> false,
                    false,
                    // valueOf(null) is false, and we actually do want null in some cases
                    (s, p) -> (s == null || "null".equalsIgnoreCase(s))? true : Boolean.valueOf(s)
            );

    public static final StandardBundlerParam<Boolean> STOP_ON_UNINSTALL  =
            new StandardBundlerParam<>(
                    I18N.getString("param.stop-on-uninstall.name"),
                    I18N.getString("param.stop-on-uninstall.description"),
                    "stopOnUninstall",
                    Boolean.class,
                    null,
                    params -> true,
                    false,
                    // valueOf(null) is false, and we actually do want null in some cases
                    (s, p) -> (s == null || "null".equalsIgnoreCase(s))? true : Boolean.valueOf(s)
            );

    public static final StandardBundlerParam<Boolean> RUN_AT_STARTUP  =
            new StandardBundlerParam<>(
                    I18N.getString("param.run-at-startup.name"),
                    I18N.getString("param.run-at-startup.description"),
                    "runAtStartup",
                    Boolean.class,
                    null,
                    params -> false,
                    false,
                    // valueOf(null) is false, and we actually do want null in some cases
                    (s, p) -> (s == null || "null".equalsIgnoreCase(s))? true : Boolean.valueOf(s)
            );

    public static final StandardBundlerParam<Boolean> SHORTCUT_HINT =
            new StandardBundlerParam<>(
                    I18N.getString("param.desktop-shortcut-hint.name"),
                    I18N.getString("param.desktop-shortcut-hint.description"),
                    BundleParams.PARAM_SHORTCUT,
                    Boolean.class,
                    null,
                    params -> false,
                    false,
                    // valueOf(null) is false, and we actually do want null in some cases
                    (s, p) -> (s == null || "null".equalsIgnoreCase(s))? false : Boolean.valueOf(s)
            );

    public static final StandardBundlerParam<Boolean> MENU_HINT =
            new StandardBundlerParam<>(
                    I18N.getString("param.menu-shortcut-hint.name"),
                    I18N.getString("param.menu-shortcut-hint.description"),
                    BundleParams.PARAM_MENU,
                    Boolean.class,
                    null,
                    params -> true,
                    false,
                    // valueOf(null) is false, and we actually do want null in some cases
                    (s, p) -> (s == null || "null".equalsIgnoreCase(s))? true : Boolean.valueOf(s)
            );

    @SuppressWarnings("unchecked")
    public static final StandardBundlerParam<List<String>> LICENSE_FILE =
            new StandardBundlerParam<>(
                    I18N.getString("param.license-file.name"),
                    I18N.getString("param.license-file.description"),
                    BundleParams.PARAM_LICENSE_FILE,
                    (Class<List<String>>)(Object)List.class,
                    null,
                    params -> Collections.<String>emptyList(),
                    false,
                    (s, p) -> Arrays.asList(s.split(","))
            );

    public static final BundlerParamInfo<String> LICENSE_TYPE =
            new StandardBundlerParam<> (
                    I18N.getString("param.license-type.name"),
                    I18N.getString("param.license-type.description"),
                    BundleParams.PARAM_LICENSE_TYPE,
                    String.class, null,
                    params -> I18N.getString("param.license-type.default"),
                    false, (s, p) -> s
            );

    public static final StandardBundlerParam<File> BUILD_ROOT =
            new StandardBundlerParam<>(
                    I18N.getString("param.build-root.name"),
                    I18N.getString("param.build-root.description"),
                    "buildRoot",
                    File.class,
                    null,
                    params -> {
                        try {
                            return Files.createTempDirectory("fxbundler").toFile();
                        } catch (IOException ioe) {
                            return null;
                        }
                    },
                    false,
                    (s, p) -> new File(s)
            );

    public static final StandardBundlerParam<String> IDENTIFIER =
            new StandardBundlerParam<>(
                    I18N.getString("param.identifier.name"),
                    I18N.getString("param.identifier.description"),
                    BundleParams.PARAM_IDENTIFIER,
                    String.class,
                    null,
                    params -> {
                        String s = MAIN_CLASS.fetchFrom(params);
                        if (s == null) return null;

                        int idx = s.lastIndexOf(".");
                        if (idx >= 1) {
                            return s.substring(0, idx);
                        }
                        return s;
                    },
                    false,
                    (s, p) -> s
            );

    public static final StandardBundlerParam<String> PREFERENCES_ID =
            new StandardBundlerParam<>(
                    I18N.getString("param.preferences-id.name"),
                    I18N.getString("param.preferences-id.description"),
                    "preferencesID",
                    String.class,
                    new String[] {IDENTIFIER.getID()},
                    params -> null, // todo take the package of the main app class
                    false,
                    (s, p) -> s
            );

    public static final StandardBundlerParam<Boolean> VERBOSE  =
            new StandardBundlerParam<>(
                    I18N.getString("param.verbose.name"),
                    I18N.getString("param.verbose.description"),
                    "verbose",
                    Boolean.class,
                    null,
                    params -> false,
                    false,
                    // valueOf(null) is false, and we actually do want null in some cases
                    (s, p) -> (s == null || "null".equalsIgnoreCase(s))? true : Boolean.valueOf(s)
            );

    public static void extractParamsFromAppResources(Map<String, ? super Object> params) {
        RelativeFileSet appResources = APP_RESOURCES.fetchFrom(params);

        if (appResources == null) {
            return;
        }
        boolean hasMainClass = params.containsKey(MAIN_CLASS.getID());
        boolean hasMainJar = params.containsKey(MAIN_JAR.getID());
        boolean hasMainJarClassPath = params.containsKey(MAIN_JAR_CLASSPATH.getID());

        if (hasMainClass && hasMainJar && hasMainJarClassPath) {
            return;
        }
        String declaredMainClass = (String) params.get(MAIN_CLASS.getID());

        File srcdir = appResources.getBaseDirectory();
        // presume the set iterates in-order
        for (String fname : appResources.getIncludedFiles()) {
            try {
                File file = new File(srcdir, fname);
                JarFile jf = new JarFile(file);
                Manifest m = jf.getManifest();
                Attributes attrs = (m != null) ? m.getMainAttributes() : null;

                if (attrs != null) {
                    String mainClass = attrs.getValue(Attributes.Name.MAIN_CLASS);
                    String fxMain = attrs.getValue(PackagerLib.MANIFEST_JAVAFX_MAIN);
                    if (hasMainClass) {
                        if (declaredMainClass.equals(fxMain)) {
                            params.put(USE_FX_PACKAGING.getID(), true);
                        } else if (declaredMainClass.equals(mainClass)) {
                            params.put(USE_FX_PACKAGING.getID(), false);
                        } else {
                            if (fxMain != null) {
                                Log.info(MessageFormat.format(I18N.getString("message.fx-app-does-not-match-specified-main"), fname, fxMain, declaredMainClass));
                            }
                            if (mainClass != null) {
                                Log.info(MessageFormat.format(I18N.getString("message.main-class-does-not-match-specified-main"), fname, mainClass, declaredMainClass));
                            }
                            continue;
                        }
                    } else {
                        if (fxMain != null) {
                            params.put(USE_FX_PACKAGING.getID(), true);
                            params.put(MAIN_CLASS.getID(), fxMain);
                        } else if (mainClass != null) {
                            params.put(USE_FX_PACKAGING.getID(), false);
                            params.put(MAIN_CLASS.getID(), mainClass);
                        } else {
                            continue;
                        }
                    }
                    if (!hasMainJar) {
                        params.put(MAIN_JAR.getID(), new RelativeFileSet(appResources.getBaseDirectory(), new LinkedHashSet<>(Arrays.asList(file))));
                    }
                    if (!hasMainJarClassPath) {
                        String cp = attrs.getValue(Attributes.Name.CLASS_PATH);
                        params.put(MAIN_JAR_CLASSPATH.getID(), cp == null ? "" : cp);
                    }
                    break;
                }
            } catch (IOException ignore) {
                ignore.printStackTrace();
            }
        }
    }
}