#!/usr/bin/env python3

import difflib
import glob
import subprocess
import sys

# Seconds
timeout = 1

success = True
for file in glob.glob("test_data/*_input.txt"):
    test_name = file.removeprefix("test_data/").removesuffix("_input.txt")
    print(f"Checking test case {test_name}")
    with open(file) as f:
        input = f.read()
    with open(file.replace("input", "output")) as f:
        output = f.read()

    try:
        res = subprocess.run(
            ["./main"],
            input=input,
            stdout=subprocess.PIPE,
            encoding="utf-8",
            timeout=timeout
        )
    except subprocess.TimeoutExpired:
        print(f"Test {test_name} timed out after {timeout} seconds")
        success = False
    else:
        if res.stdout != output:
            success = False
            print(f"Test {test_name} failed")
            print(
                "".join(
                    difflib.ndiff(
                        res.stdout.splitlines(keepends=True),
                        output.splitlines(keepends=True)
                    )
                )
            )
        else:
            print(f"Test {test_name} successful")

if not success:
    sys.exit(1)
