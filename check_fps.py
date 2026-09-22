#!/usr/bin/env python3
import os
import glob

path = os.path.expanduser("~/sensing-rigs-daemon/run/captures/lx/*.jpg")
files = sorted(glob.glob(path), key=os.path.getmtime)

if len(files) < 10:
    print("Poche foto trovate, attendi qualche secondo e riprova.")
else:
    last = files[-10:]
    print("Intervalli tra gli ultimi 10 scatti (Obiettivo 5 FPS: ~0.200 sec):")
    for i in range(1, len(last)):
        delta = os.path.getmtime(last[i]) - os.path.getmtime(last[i-1])
        print(f"Delta: {delta:.3f} s -> {os.path.basename(last[i])}")
