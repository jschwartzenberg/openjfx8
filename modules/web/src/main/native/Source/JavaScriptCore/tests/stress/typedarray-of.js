load("./resources/typedarray-constructor-helper-functions.js");
description(
"This test checks the behavior of the TypedArray.of function"
);

shouldBe("Int32Array.of.length", "0");
shouldBe("Int32Array.of.name", "'of'");
debug("");

debug("testPrototypeFunction has the following arg list (name, args, expected)");
debug("");

shouldBeTrue("testConstructorFunction('of', '()', [])");
shouldBeTrue("testConstructorFunction('of', '(1)', [1])");
shouldBeTrue("testConstructorFunction('of', '(1,2,3)', [1,2,3])");
finishJSTest();
