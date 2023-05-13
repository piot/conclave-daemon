# Conclave Daemon

## Install

Extract the release zip containing the `conclaved` linux executable and `conclave.service` [systemd unit file](https://www.freedesktop.org/software/systemd/man/systemd.unit.html).

```console
sudo chmod a+x conclaved
sudo cp conclaved /usr/local/sbin/
sudo cp conclave.service /usr/local/lib/systemd/system/
```

* Make systemd reload all .service-files

```console
systemctl daemon-reload
```

* Check that systemd has read the .service-file:

```console
systemctl status conclave --output cat
```

It should report something similar to:

```console
○ conclave.service - Conclave UDP Relay
     Loaded: loaded (/usr/local/lib/systemd/system/conclave.service; disabled; preset: disabled)
     Active: inactive (dead)
```

* Make sure it is started, if machine is rebooted in the future:

```console
systemctl enable conclave
```

* Start it now:

```console
systemctl start conclave
```

## Useful commands

### journalctl

[journalctl](https://www.freedesktop.org/software/systemd/man/journalctl.html) outputs the log entries stored in the journal.

```console
journalctl -u conclave -b --output cat -f
```
