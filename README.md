# Simple CLI for JACK audio server

Small command line interface to connect/disconnect clients to jack server. Useful for scripting connections on kiosk-mode applications.

## Options

    -l  list all ports
    -C  list all connections
    -D  disconnect all connections
    -c  src dest       connect src to dest
    -d  src dest       disconnect src from dest

# Examples

## List available ports

    jackctl -l
    playback: system:playback_1
    playback: system:playback_2
    capture: system:capture_1
    capture: system:capture_2
    capture: aeolus:out.L
    capture: aeolus:out.R
    midi_in: foo-umidi0.0:midi.RX
    midi_in: aeolus:Midi/in
    midi_out: foo-umidi0.0:midi.TX

## Connect audio and midi ports for a client

    jacktl -c system:playback_1 aeolus:out.L
    jacktl -c system:playback_2 aeolus:out.R
    jacktl -c foo:umidi0.0:TX aeolus:Midi/in

# License

GPLV2 or later + MIT
