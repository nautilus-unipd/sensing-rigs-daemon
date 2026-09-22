#!/usr/bin/env python3
import os
import glob

path = os.path.expanduser("~/sensing-rigs-daemon/run/captures/lx/*.jpg")
files = sorted(glob.glob(path), key=os.path.getmtime)

if len(files) < 10:
    print("Not enough photos found, please wait a few seconds and try again.")
else:
    last = files[-10:]
    print("Intervals between the last 10 shots (Target 5 FPS: ~0.200 sec):")
    for i in range(1, len(last)):
        delta = os.path.getmtime(last[i]) - os.path.getmtime(last[i-1])
        # Avoid division by zero in case two files have the exact same timestamp
        fps = 1.0 / delta if delta > 0 else 0.0 
        print(f"Delta: {delta:.3f} s | FPS: {fps:.1f} -> {os.path.basename(last[i])}")