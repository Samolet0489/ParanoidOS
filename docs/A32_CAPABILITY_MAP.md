# Samsung A32 5G Hardware Capability Map

## Purpose

This document records read-only findings from the stock Samsung firmware for the
Samsung Galaxy A32 5G SM-A326B/DS.

It is reference material for future ParanoidOS hardware work.
It is not a ParanoidOS configuration file and must not be treated as a flashable
or bootable device-tree source.

## Identity

- Device model: SM-A326B/DS
- Device codename: a32x
- SoC identifier from stock DTB: MediaTek MT6853
- Stock Android version: 13
- Bootloader build: A326BXXSECYB5
- Firmware CSC: BGL / Bulgaria

## CPU and platform

- Eight CPU cores described by the stock DTS:
  - Six ARM Cortex-A55 cores
  - Two ARM Cortex-A76 cores
- CPU control uses PSCI.
- Interrupt controller: ARM GICv3.

## Hardware areas identified

- UART:
  - serial@11002000
  - serial@11003000
- Storage:
  - MediaTek MSDC controllers
  - MediaTek UFS controller
- USB:
  - USB host/controller and USB PHY nodes
- Radio-related:
  - Wi-Fi node
  - Bluetooth node
  - modem-related nodes
  - Wi-Fi and modem reserved-memory / regulator references

## ParanoidOS initial brick-mode policy

Initial ParanoidOS should initialize only the minimum required components:

1. CPU and exception handling
2. MMU and protected memory mappings
3. Interrupt controller
4. Timer
5. One debug UART
6. Optional local storage later

The following remain disabled by default:

- Wi-Fi
- Bluetooth
- Cellular modem access
- USB networking
- Android userspace
- Google services
- Samsung services
- Background network traffic
- DNS, NTP, telemetry, and cloud communication

Future communication must be explicitly enabled and authenticated, beginning with
a controlled encrypted laptop link.

## Safety status

All A32 work so far has been read-only:

- No bootloader unlock
- No flashing
- No rooting
- No partition writes
- No stock firmware modification
