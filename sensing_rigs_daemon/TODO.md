# List of missing features

## System

- [ ] Create *systemd .service* file (if needed)

## Logger

- [ ] Optimize internal log error handling
- [ ] Add log rotation?
- [ ] Create custom structure to hold last 'N' log entries, instead of using a buffer

## Signal handler

- [ ] Disable/enable image acquiition when SIGHUP is received

## Source files

- [ ] Checks:
    - [ ] Under voltage
    - [ ] Cameras integrity
    - [ ] Disk space
- [ ] Functions:
    - [ ] Capture cameras

