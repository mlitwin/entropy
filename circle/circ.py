#!/usr/bin/python3

import subprocess
import random

# ./circle -n200 -v22 -d600
for i in range(1, 1000):
    n = random.randint(100, 10000)
    v = random.randint(0,100)
    d = random.randint(n, 100*n)
    subprocess.run(["./circle", f"-n{n}", "-v22", "-d600"]) 