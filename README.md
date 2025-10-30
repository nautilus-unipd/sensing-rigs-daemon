# C development container for Sensing-Rigs phototrap

> Software developed for Nautilus-UniPD student organization.

## Table of contents

- [Description](#description)
- [Software used](#software-used)
- [Installation](#installation)
- [Contributing](#contributing)
- [Authors](#authors)

---

## Description

This custom Docker container provides an uniform and portable development environment for the **Sensing-Rigs daemon**, written in C.

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

## Contributing

To contribute in this project:

1. Fork the repository and perform some changes.
2. Create a new feature branch named "**development-new-feature**"
```bash
git checkout -b development-new-feature
```
3. Commit your changes:
```bash
git commit -am"New feature description"
```
4. Push the branch:
```bash
git push origin development-new-feature
```
5. Finally open a new pull request with the branch containing the changes.

To make the comparison/testing easier ensure the changes are well documented and follow the existing coding style.

---

## Authors

Software developed by:
- Pasetto Niccolò

---
