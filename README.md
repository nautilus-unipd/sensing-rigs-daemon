# C development container for Sensing-Rigs phototrap

> Software developed for Nautilus-UniPD student organization.

## CONTENUTI

- [Description](#description)
- [Software used](#software-used)
- [Installation](#installation)
- [Authors](#authors)

---

## Description

This custom Docker container provvides an uniform and portable development environment for the **Sensing-Rigs daemon**, written in C.

However, this environment is slightly different from the actual operating system which will host the daemon. It will be executed in the background of a "*Raspberry PI 5*" with "*Raspberry PI OS lite (64bit)*" as operating system.

Since both the container and the O.S. used are based on the same version of Debian ("*Debian Trixie*") the compilation phase should have no differences between the 2 systems. Obviously, in the containerized version the cameras will not be available.

---

## Software used

To build this container "*Docker*" and "*docker-compose*" are required.

---

## Installation

To compile the source code see this [other README.md](./sensing_rigs_daemon/README.md)

To build the container from the base "Dockerfile" and "compose.yaml" and start it on detached mode:

```bash
docker-compose up --build -d
```

To start a stopped container:

```bash
docker start -ai demon_hunter
```

---

## Authors

Software developed by:
- Pasetto Niccolò

---
