# ParanoidOS Panic Mode Design

## Default state

Every boot starts in **Sealed Mode**.

## Sealed Mode

- No Wi-Fi
- No Bluetooth
- No cellular modem access
- No USB networking
- No background network stack
- No persistent temporary permissions
- Local console only

## Temporary capability grants

Temporary capability grants must be:

- Explicitly approved by the user
- Narrow in scope
- Time-limited where possible
- Revoked on shutdown by default

## Panic Mode

Panic Mode:

- Immediately tears down communications
- Revokes active temporary capabilities
- Clears session keys and volatile credentials
- Locks sensitive storage where possible
- Reboots into Sealed Mode

## Recovery Reset

Recovery Reset:

- Uses a verified known-good recovery environment
- Restores the Sealed Mode policy
- Can verify/reinstall known-good ParanoidOS images
- May optionally wipe encrypted user data and keys

## Security limitation

Panic Mode reduces risk from compromise within ParanoidOS, but cannot by
itself prove recovery from compromise of lower-level vendor firmware or
hardware subsystems.
