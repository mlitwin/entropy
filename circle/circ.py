#!/usr/bin/python3

import subprocess
import random

for i in range(1, 1000):
    n = random.randint(100, 1000)
    v = random.randint(0,100)
    d = random.randint(n, 100*n)
    subprocess.run(["./circle", f"-n{n}", "-v22", "-d600"]) 