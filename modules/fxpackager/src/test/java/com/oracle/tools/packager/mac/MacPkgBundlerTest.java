/*
 * Copyright (c) 2014, 2015, Oracle and/or its affiliates. All rights reserved.
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

package com.oracle.tools.packager.mac;

import com.oracle.tools.packager.AbstractBundler;
import com.oracle.tools.packager.Bundler;
import com.oracle.tools.packager.BundlerParamInfo;
import com.oracle.tools.packager.ConfigException;
import com.oracle.tools.packager.IOUtils;
import com.oracle.tools.packager.Log;
import com.oracle.tools.packager.RelativeFileSet;
import com.oracle.tools.packager.UnsupportedPlatformException;
import org.junit.After;
import org.junit.Assume;
import org.junit.Before;
import org.junit.BeforeClass;
import org.junit.Test;

import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.IOException;
import java.io.PrintStream;
import java.nio.file.Files;
import java.util.Arrays;
import java.util.Collection;
import java.util.HashMap;
import java.util.HashSet;
import java.util.Map;
import java.util.Set;
import java.util.TreeMap;

import static com.oracle.tools.packager.StandardBundlerParam.*;
import static com.oracle.tools.packager.mac.MacAppBundler.*;
import static com.oracle.tools.packager.mac.MacBaseInstallerBundler.MAC_APP_IMAGE;
import static com.oracle.tools.packager.mac.MacBaseInstallerBundler.SIGNING_KEYCHAIN;
import static com.oracle.tools.packager.mac.MacPkgBundler.DEVELOPER_ID_INSTALLER_SIGNING_KEY;
import static com.oracle.tools.packager.mac.MacPkgBundler.INSTALLER_SUFFIX;
import static org.junit.Assert.*;

public class MacPkgBundlerTest {

    static final int MIN_SIZE=0x100000; // 1MiB

    static File tmpBase;
    static File workDir;
    static File appResourcesDir;
    static File fakeMainJar;
    static File hdpiIcon;
    static String runtimeJdk;
    static Set<File> appResources;
    static boolean retain = false;
    static boolean signingKeysPresent = false;
    
    static final File FAKE_CERT_ROOT = new File("build/tmp/tests/cert/").getAbsoluteFile();

    @BeforeClass
    public static void prepareApp() {
        // only run on mac
        Assume.assumeTrue(System.getProperty("os.name").toLowerCase().contains("os x"));

        runtimeJdk = System.getenv("PACKAGER_JDK_ROOT");

        // and only if we have the correct JRE settings
        String jre = System.getProperty("java.home").toLowerCase();
        Assume.assumeTrue(runtimeJdk != null || jre.endsWith("/contents/home/jre") || jre.endsWith("/contents/home/jre"));

        Log.setLogger(new Log.Logger(true));
        Log.setDebug(true);

        retain = Boolean.parseBoolean(System.getProperty("RETAIN_PACKAGER_TESTS"));

        workDir = new File("build/tmp/tests", "macpkg");
        hdpiIcon = new File("build/tmp/tests", "GenericAppHiDPI.icns");
        appResourcesDir = new File("build/tmp/tests", "appResources");
        fakeMainJar = new File(appResourcesDir, "mainApp.jar");

        appResources = new HashSet<>(Arrays.asList(fakeMainJar,
                new File(appResourcesDir, "LICENSE"),
                new File(appResourcesDir, "LICENSE2")
        ));

        signingKeysPresent = DEVELOPER_ID_INSTALLER_SIGNING_KEY.fetchFrom(new TreeMap<>()) != null;
    }

    @Before
    public void createTmpDir() throws IOException {
        if (retain) {
            tmpBase = new File("build/tmp/tests/macpkg");
        } else {
            tmpBase = BUILD_ROOT.fetchFrom(new TreeMap<>());
        }
        tmpBase.mkdir();
    }
    
    public String createFakeCerts(Map<String, ? super Object> p) {
        File config = new File(FAKE_CERT_ROOT, "pkg-cert.cfg");
        config.getParentFile().mkdirs();
        try {
            // create the config file holding the key config
            Files.write(config.toPath(), Arrays.<String>asList("[ codesign ]",
                    "keyUsage=critical,digitalSignature",
                    "basicConstraints=critical,CA:false",
                    "extendedKeyUsage=critical,codeSigning",
                    "[ productbuild ]",
                    "basicConstraints=critical,CA:false",
                    "keyUsage=critical,digitalSignature",
                    "extendedKeyUsage=critical,1.2.840.113635.100.4.13",
                    "1.2.840.113635.100.6.1.14=critical,DER:0500"));

            // create the SSL keys
            ProcessBuilder pb = new ProcessBuilder("openssl", "req",
                    "-newkey", "rsa:2048",
                    "-nodes",
                    "-out", FAKE_CERT_ROOT + "/pkg-app.csr",
                    "-keyout", FAKE_CERT_ROOT + "/pkg-app.key",
                    "-subj", "/CN=Developer ID Application: Insecure Test Cert/OU=JavaFX Dev/O=Oracle/C=US");
            IOUtils.exec(pb, VERBOSE.fetchFrom(p));

            // first, for the app
            // create the cert
            pb = new ProcessBuilder("openssl", "x509",
                    "-req",
                    "-days", "10",
                    "-in", FAKE_CERT_ROOT + "/pkg-app.csr",
                    "-signkey", FAKE_CERT_ROOT + "/pkg-app.key",
                    "-out", FAKE_CERT_ROOT + "/pkg-app.crt",
                    "-extfile", FAKE_CERT_ROOT + "/pkg-cert.cfg",
                    "-extensions", "codesign");
            IOUtils.exec(pb, VERBOSE.fetchFrom(p));

            // create and add it to the keychain
            pb = new ProcessBuilder("certtool",
                    "i", FAKE_CERT_ROOT + "/pkg-app.crt",
                    "k=" + FAKE_CERT_ROOT + "/pkg.keychain",
                    "r=" + FAKE_CERT_ROOT + "/pkg-app.key",
                    "c",
                    "p=");
            IOUtils.exec(pb, VERBOSE.fetchFrom(p));
            
            // create the pkg SSL keys
            pb = new ProcessBuilder("openssl", "req",
                    "-newkey", "rsa:2048",
                    "-nodes",
                    "-out", FAKE_CERT_ROOT + "/pkg-pkg.csr",
                    "-keyout", FAKE_CERT_ROOT + "/pkg-pkg.key",
                    "-subj", "/CN=Developer ID Installer: Insecure Test Cert/OU=JavaFX Dev/O=Oracle/C=US");
            IOUtils.exec(pb, VERBOSE.fetchFrom(p));

            // create the pkg cert
            pb = new ProcessBuilder("openssl", "x509",
                        "-req",
                        "-days", "10",
                        "-in", FAKE_CERT_ROOT + "/pkg-pkg.csr",
                        "-signkey", FAKE_CERT_ROOT + "/pkg-pkg.key",
                        "-out", FAKE_CERT_ROOT + "/pkg-pkg.crt",
                        "-extfile",FAKE_CERT_ROOT + "/pkg-cert.cfg",
                        "-extensions", "productbuild");
            IOUtils.exec(pb, VERBOSE.fetchFrom(p));

            // create and add it to the keychain
            pb = new ProcessBuilder("certtool",
                    "i", FAKE_CERT_ROOT + "/pkg-pkg.crt",
                    "k=" + FAKE_CERT_ROOT + "/pkg.keychain",
                    "r=" + FAKE_CERT_ROOT + "/pkg-pkg.key");
            IOUtils.exec(pb, VERBOSE.fetchFrom(p));
            
            return FAKE_CERT_ROOT + "/pkg.keychain";
        } catch (IOException e) {
            e.printStackTrace();
        }
        
        return null;
    }

    @After
    public void maybeCleanupTmpDir() {
        if (!retain) {
            attemptDelete(tmpBase);
        }
        attemptDelete(FAKE_CERT_ROOT);
    }

    private void attemptDelete(File tmpBase) {
        if (tmpBase.isDirectory()) {
            File[] children = tmpBase.listFiles();
            if (children != null) {
                for (File f : children) {
                    attemptDelete(f);
                }
            }
        }
        boolean success;
        try {
            success = !tmpBase.exists() || tmpBase.delete();
        } catch (SecurityException se) {
            success = false;
        }
        if (!success) {
            System.err.println("Could not clean up " + tmpBase.toString());
        }
    }

    /**
     * See if smoke comes out
     */
    @Test
    public void smokeTest() throws IOException, ConfigException, UnsupportedPlatformException {
        AbstractBundler bundler = new MacPkgBundler();

        assertNotNull(bundler.getName());
        assertNotNull(bundler.getID());
        assertNotNull(bundler.getDescription());
        //assertNotNull(bundler.getBundleParameters());

        Map<String, Object> bundleParams = new HashMap<>();

        bundleParams.put(BUILD_ROOT.getID(), tmpBase);

        bundleParams.put(APP_NAME.getID(), "Smoke Test");
        bundleParams.put(MAIN_CLASS.getID(), "hello.HelloRectangle");
        bundleParams.put(PREFERENCES_ID.getID(), "the/really/long/preferences/id");
        bundleParams.put(APP_RESOURCES.getID(), new RelativeFileSet(appResourcesDir, appResources));
        bundleParams.put(MAIN_JAR.getID(),
                new RelativeFileSet(fakeMainJar.getParentFile(),
                        new HashSet<>(Arrays.asList(fakeMainJar)))
        );
        bundleParams.put(CLASSPATH.getID(), "mainApp.jar");
        bundleParams.put(VERBOSE.getID(), true);
        bundleParams.put(LICENSE_FILE.getID(), Arrays.asList("LICENSE", "LICENSE2"));
        bundleParams.put(SIGN_BUNDLE.getID(), false); // force no signing

        if (runtimeJdk != null) {
            bundleParams.put(MAC_RUNTIME.getID(), runtimeJdk);
        }

        boolean valid = bundler.validate(bundleParams);
        assertTrue(valid);

        File result = bundler.execute(bundleParams, new File(workDir, "smoke"));
        System.err.println("Bundle at - " + result);
        assertNotNull(result);
        assertTrue(result.exists());
        assertTrue(result.length() > MIN_SIZE);
    }

    /**
     * Build smoke test and mark it as quarantined, possibly signed
     */
    @Test
    public void quarantinedAppTest() throws IOException, ConfigException, UnsupportedPlatformException {

        AbstractBundler bundler = new MacPkgBundler();

        assertNotNull(bundler.getName());
        assertNotNull(bundler.getID());
        assertNotNull(bundler.getDescription());
        //assertNotNull(bundler.getBundleParameters());

        Map<String, Object> bundleParams = new HashMap<>();

        bundleParams.put(BUILD_ROOT.getID(), tmpBase);

        bundleParams.put(APP_NAME.getID(), "Quarantine App");
        bundleParams.put(MAIN_CLASS.getID(), "hello.HelloRectangle");
        bundleParams.put(PREFERENCES_ID.getID(), "the/really/long/preferences/id");
        bundleParams.put(APP_RESOURCES.getID(), new RelativeFileSet(appResourcesDir, appResources));
        bundleParams.put(VERBOSE.getID(), true);

        if (runtimeJdk != null) {
            bundleParams.put(MAC_RUNTIME.getID(), runtimeJdk);
        }

        if (!signingKeysPresent) {
            String keychain = createFakeCerts(bundleParams);
            Assume.assumeNotNull(keychain);
            bundleParams.put(SIGNING_KEYCHAIN.getID(), keychain);
        }

        boolean valid = bundler.validate(bundleParams);
        assertTrue(valid);

        File result = bundler.execute(bundleParams, new File(workDir, "quarantine"));
        System.err.println("Bundle at - " + result);
        assertNotNull(result);
        assertTrue(result.exists());
        assertTrue(result.length() > MIN_SIZE);
        validateSignatures(result);

        // mark it as though it's been downloaded
        ProcessBuilder pb = new ProcessBuilder(
                "xattr", "-w", "com.apple.quarantine",
                "0000;" + Long.toHexString(System.currentTimeMillis() / 1000L) + ";Java Unit Tests;|com.oracle.jvm.8u",
                result.toString());
        IOUtils.exec(pb, true);
    }

    /**
     * The bare minimum configuration needed to make it work
     * <ul>
     *     <li>Where to build it</li>
     *     <li>The jar containing the application (with a main-class attribute)</li>
     * </ul>
     *
     * All other values will be driven off of those two values.
     */
    @Test
    public void minimumConfig() throws IOException, ConfigException, UnsupportedPlatformException {
        Bundler bundler = new MacPkgBundler();

        Map<String, Object> bundleParams = new HashMap<>();

        bundleParams.put(BUILD_ROOT.getID(), tmpBase);

        bundleParams.put(APP_RESOURCES.getID(), new RelativeFileSet(appResourcesDir, appResources));

        if (runtimeJdk != null) {
            bundleParams.put(MAC_RUNTIME.getID(), runtimeJdk);
        }

        String keychain = null;
        if (!signingKeysPresent) {
            keychain = createFakeCerts(bundleParams);
            if (keychain != null) {
                bundleParams.put(SIGNING_KEYCHAIN.getID(), keychain);
            }
        }

        boolean valid = bundler.validate(bundleParams);
        assertTrue(valid);

        File output = bundler.execute(bundleParams, new File(workDir, "BareMinimum"));
        System.err.println("Bundle at - " + output);
        assertNotNull(output);
        assertTrue(output.exists());
        assertTrue(output.length() > MIN_SIZE);
        if (signingKeysPresent || keychain != null) {
            validateSignatures(output);
        }
    }

    /**
     * Test with unicode in places we expect it to be
     */
    @Test
    public void unicodeConfig() throws IOException, ConfigException, UnsupportedPlatformException {
        Bundler bundler = new MacPkgBundler();

        Map<String, Object> bundleParams = new HashMap<>();

        bundleParams.put(BUILD_ROOT.getID(), tmpBase);

        bundleParams.put(APP_RESOURCES.getID(), new RelativeFileSet(appResourcesDir, appResources));

        bundleParams.put(APP_NAME.getID(), "хелловорлд");
        bundleParams.put(TITLE.getID(), "ХеллоВорлд аппликейшн");
        bundleParams.put(VENDOR.getID(), "Оракл девелопмент");
        bundleParams.put(DESCRIPTION.getID(), "крайне большое описание со странными символами");

        if (runtimeJdk != null) {
            bundleParams.put(MAC_RUNTIME.getID(), runtimeJdk);
        }

        String keychain = null;
        if (!signingKeysPresent) {
            keychain = createFakeCerts(bundleParams);
            if (keychain != null) {
                bundleParams.put(SIGNING_KEYCHAIN.getID(), keychain);
            }
        }

        bundler.validate(bundleParams);

        File output = bundler.execute(bundleParams, new File(workDir, "Unicode"));
        System.err.println("Bundle at - " + output);
        assertNotNull(output);
        assertTrue(output.exists());
        if (signingKeysPresent || keychain != null) {
            validateSignatures(output);
        }
    }

    /**
     * Create a PKG with an external app rather than a self-created one.
     */
    @Test
    public void externalApp() throws IOException, ConfigException, UnsupportedPlatformException {
        // only run with full tests
        Assume.assumeTrue(Boolean.parseBoolean(System.getProperty("FULL_TEST")));

        // first create the external app
        Bundler appBundler = new MacAppBundler();

        Map<String, Object> appBundleParams = new HashMap<>();

        appBundleParams.put(BUILD_ROOT.getID(), tmpBase);

        appBundleParams.put(APP_RESOURCES.getID(), new RelativeFileSet(appResourcesDir, appResources));
        appBundleParams.put(APP_NAME.getID(), "External APP PKG Test");
        appBundleParams.put(IDENTIFIER.getID(), "com.example.pkg.external");
        appBundleParams.put(VERBOSE.getID(), true);

        if (runtimeJdk != null) {
            appBundleParams.put(MAC_RUNTIME.getID(), runtimeJdk);
        }

        String keychain = null;
        if (!signingKeysPresent) {
            keychain = createFakeCerts(appBundleParams);
            if (keychain != null) {
                appBundleParams.put(SIGNING_KEYCHAIN.getID(), keychain);
            }
        }
        
        boolean valid = appBundler.validate(appBundleParams);
        assertTrue(valid);

        File appOutput = appBundler.execute(appBundleParams, new File(workDir, "PKGExternalApp1"));
        System.err.println("App at - " + appOutput);
        assertNotNull(appOutput);
        assertTrue(appOutput.exists());

        // now create the PKG referencing this external app
        Bundler pkgBundler = new MacPkgBundler();

        Map<String, Object> pkgBundleParams = new HashMap<>();

        pkgBundleParams.put(BUILD_ROOT.getID(), tmpBase);

        pkgBundleParams.put(MAC_APP_IMAGE.getID(), appOutput);
        pkgBundleParams.put(APP_NAME.getID(), "External APP PKG Test");
        pkgBundleParams.put(IDENTIFIER.getID(), "com.example.pkg.external");

        pkgBundleParams.put(VERBOSE.getID(), true);

        if (runtimeJdk != null) {
            pkgBundleParams.put(MAC_RUNTIME.getID(), runtimeJdk);
        }
        if (keychain != null) {
            pkgBundleParams.put(SIGNING_KEYCHAIN.getID(), keychain);
        }

        valid = pkgBundler.validate(pkgBundleParams);
        assertTrue(valid);

        File pkgOutput = pkgBundler.execute(pkgBundleParams, new File(workDir, "PKGExternalApp2"));
        System.err.println(".pkg at - " + pkgOutput);
        assertNotNull(pkgOutput);
        assertTrue(pkgOutput.exists());
        assertTrue(pkgOutput.length() > MIN_SIZE);

        if (signingKeysPresent || keychain != null) {
            validateSignatures(pkgOutput);
        }
        
    }

    @Test(expected = ConfigException.class)
    public void externanNoAppName() throws ConfigException, UnsupportedPlatformException {
        Bundler pkgBundler = new MacPkgBundler();

        Map<String, Object> pkgBundleParams = new HashMap<>();

        pkgBundleParams.put(BUILD_ROOT.getID(), tmpBase);

        pkgBundleParams.put(MAC_APP_IMAGE.getID(), ".");
        pkgBundleParams.put(IDENTIFIER.getID(), "net.example.bogus");
        pkgBundleParams.put(VERBOSE.getID(), true);

        pkgBundler.validate(pkgBundleParams);
    }

    @Test(expected = ConfigException.class)
    public void externanNoID() throws ConfigException, UnsupportedPlatformException {
        Bundler pkgBundler = new MacPkgBundler();

        Map<String, Object> pkgBundleParams = new HashMap<>();

        pkgBundleParams.put(BUILD_ROOT.getID(), tmpBase);

        pkgBundleParams.put(MAC_APP_IMAGE.getID(), ".");
        pkgBundleParams.put(APP_NAME.getID(), "Bogus App");
        pkgBundleParams.put(VERBOSE.getID(), true);

        pkgBundler.validate(pkgBundleParams);
    }

    @Test(expected = ConfigException.class)
    public void invalidLicenseFile() throws ConfigException, UnsupportedPlatformException {
        Bundler bundler = new MacPkgBundler();

        Map<String, Object> bundleParams = new HashMap<>();

        bundleParams.put(BUILD_ROOT.getID(), tmpBase);

        bundleParams.put(APP_RESOURCES.getID(), new RelativeFileSet(appResourcesDir, appResources));
        bundleParams.put(LICENSE_FILE.getID(), "BOGUS_LICENSE");

        bundler.validate(bundleParams);
    }

    /**
     * Test a misconfiguration where signature is requested but no key is specified.
     */
    @Test
    public void signButNoCert() throws IOException, ConfigException, UnsupportedPlatformException {
        // only run with full tests
        Assume.assumeTrue(Boolean.parseBoolean(System.getProperty("FULL_TEST")));

        try {
            // first create the external app
            Bundler appBundler = new MacAppBundler();
    
            Map<String, Object> appBundleParams = new HashMap<>();
    
            appBundleParams.put(BUILD_ROOT.getID(), tmpBase);
    
            appBundleParams.put(APP_RESOURCES.getID(), new RelativeFileSet(appResourcesDir, appResources));
            appBundleParams.put(APP_NAME.getID(), "External APP PKG Negative Signature Test");
            appBundleParams.put(IDENTIFIER.getID(), "com.example.pkg.external");
            appBundleParams.put(VERBOSE.getID(), true);
    
            if (runtimeJdk != null) {
                appBundleParams.put(MAC_RUNTIME.getID(), runtimeJdk);
            }
    
            boolean valid = appBundler.validate(appBundleParams);
            assertTrue(valid);
    
            File appOutput = appBundler.execute(appBundleParams, new File(workDir, "PKGExternalAppSignTest"));
            System.err.println("App at - " + appOutput);
            assertNotNull(appOutput);
            assertTrue(appOutput.exists());
    
            // now create the PKG referencing this external app
            Bundler pkgBundler = new MacPkgBundler();
    
            Map<String, Object> pkgBundleParams = new HashMap<>();
    
            pkgBundleParams.put(BUILD_ROOT.getID(), tmpBase);
    
            pkgBundleParams.put(MAC_APP_IMAGE.getID(), appOutput);
            pkgBundleParams.put(APP_NAME.getID(), "Negative Signature Test");
            pkgBundleParams.put(IDENTIFIER.getID(), "com.example.pkg.external");
    
            pkgBundleParams.put(SIGN_BUNDLE.getID(), true);
            pkgBundleParams.put(DEVELOPER_ID_INSTALLER_SIGNING_KEY.getID(), null);
    
            pkgBundler.validate(pkgBundleParams);

            // if we get here we fail
            assertTrue("ConfigException should have been thrown", false);
        } catch (ConfigException ignore) {
            // expected
        }            
    }

    @Test
    public void configureEverything() throws Exception {
        AbstractBundler bundler = new MacPkgBundler();
        Collection<BundlerParamInfo<?>> parameters = bundler.getBundleParameters();

        Map<String, Object> bundleParams = new HashMap<>();

        bundleParams.put(APP_NAME.getID(), "Everything App Name");
        bundleParams.put(APP_RESOURCES.getID(), new RelativeFileSet(appResourcesDir, appResources));
        bundleParams.put(ARGUMENTS.getID(), Arrays.asList("He Said", "She Said"));
        bundleParams.put(BUNDLE_ID_SIGNING_PREFIX.getID(), "everything.signing.prefix.");
        bundleParams.put(CLASSPATH.getID(), "mainApp.jar");
        bundleParams.put(ICON_ICNS.getID(), hdpiIcon);
        bundleParams.put(INSTALLER_SUFFIX.getID(), "-PKG-TEST");
        bundleParams.put(JVM_OPTIONS.getID(), "-Xms128M");
        bundleParams.put(JVM_PROPERTIES.getID(), "everything.jvm.property=everything.jvm.property.value");
        bundleParams.put(MAC_CATEGORY.getID(), "public.app-category.developer-tools");
        bundleParams.put(MAC_CF_BUNDLE_IDENTIFIER.getID(), "com.example.everything.cf-bundle-identifier");
        bundleParams.put(MAC_CF_BUNDLE_NAME.getID(), "Everything CF Bundle Name");
        bundleParams.put(MAC_RUNTIME.getID(), runtimeJdk == null ? System.getProperty("java.home") : runtimeJdk);
        bundleParams.put(MAIN_CLASS.getID(), "hello.HelloRectangle");
        bundleParams.put(MAIN_JAR.getID(), "mainApp.jar");
        bundleParams.put(PREFERENCES_ID.getID(), "everything/preferences/id");
        bundleParams.put(PRELOADER_CLASS.getID(), "hello.HelloPreloader");
        bundleParams.put(SIGNING_KEYCHAIN.getID(), "");
        bundleParams.put(USER_JVM_OPTIONS.getID(), "-Xmx=256M\n");
        bundleParams.put(VERSION.getID(), "1.2.3.4");

        //bundleParams.put(IDENTIFIER.getID(), "com.example.everything.identifier");
        bundleParams.put(DEVELOPER_ID_INSTALLER_SIGNING_KEY.getID(), "Developer ID Installer");
        bundleParams.put(LICENSE_FILE.getID(), "LICENSE");
        //bundleParams.put(SERVICE_HINT.getID(), false);

        // assert they are set
        for (BundlerParamInfo bi :parameters) {
            assertNotNull("Bundle args Contains " + bi.getID(), bundleParams.containsKey(bi.getID()));
        }

        // and only those are set
        bundleParamLoop:
        for (String s :bundleParams.keySet()) {
            for (BundlerParamInfo<?> bpi : parameters) {
                if (s.equals(bpi.getID())) {
                    continue bundleParamLoop;
                }
            }
            fail("Enumerated parameters does not contain " + s);
        }

        // assert they resolve
        for (BundlerParamInfo bi :parameters) {
            bi.fetchFrom(bundleParams);
        }

        // add verbose now that we are done scoping out parameters
        bundleParams.put(BUILD_ROOT.getID(), tmpBase);
        bundleParams.put(VERBOSE.getID(), true);

        // assert it validates
        boolean valid = bundler.validate(bundleParams);
        assertTrue(valid);

        // only run the bundle with full tests
        Assume.assumeTrue(Boolean.parseBoolean(System.getProperty("FULL_TEST")));

        // but first remove signing keys, test servers don't have these...
        bundleParams.remove(DEVELOPER_ID_INSTALLER_SIGNING_KEY.getID());

        File result = bundler.execute(bundleParams, new File(workDir, "everything"));
        System.err.println("Bundle at - " + result);
        assertNotNull(result);
        assertTrue(result.exists());
        assertTrue(result.length() > MIN_SIZE);
    }

    public void validateSignatures(File appLocation) throws IOException {
        // Check the signatures with pkgUtil
        ProcessBuilder pb = new ProcessBuilder(
                "pkgutil", "--check-signature",
                appLocation.getCanonicalPath());
        ByteArrayOutputStream baos = new ByteArrayOutputStream();
        PrintStream ps = new PrintStream(baos);
        try {
            IOUtils.exec(pb, true, false, ps);
        } catch (IOException ioe) {
            if (signingKeysPresent) {
                // these were real keys, failures are real
                throw ioe;
            }
            // failure was for bogus key
            if (ioe.getMessage().contains("Exec failed with code 1 ")) {
                // this is likely because the key is not signed by apple, lets look 
                // ok, look to see if our key is in the output
                if (!baos.toString().contains("1. Developer ID Installer: Insecure Test Cert")) {
                    // didn't list our key as #1, must be some other error
                    throw ioe;
                }
                // ok, this is expected.  Ignore it.
            } else {
                // some other failure, throw the error.
                throw ioe;
            }
        }
    }

}
