# List of missing features

## System

- [ ] Create '*sensing_rigs_daemon.log.d*' directory
- [ ] Create *systemd .service* file (if needed)
- [ ] Make the daemon stop if another one is already running

## Logger

- [ ] Internal log handling
- [ ] Add log rotation?
- [ ] Divide log files according to their date
- [ ] Create custom structure to hold last 'N' log entries, instead of using a buffer

## Source files

- [ ] Signal handler
- [ ] Checks:
    - [ ] Under voltage
    - [ ] Cameras integrity
    - [ ] Disk space
- [ ] Functions:
    - [ ] Capture cameras

