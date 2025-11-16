# Simple CLI for JACK audio server

Command line interface to connect/disconnect JACK clients. Useful for scripting connections on kiosk-mode applications.

## Options

    -l  list all ports
    -C  list all connections
    -D  disconnect all connections
    -c  src dest       connect src to dest
    -d  src dest       disconnect src from dest

# Examples

## List available ports

```sh
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
```

## Connect audio and midi ports 

```sh
jacktl -c system:playback_1 aeolus:out.L
jacktl -c system:playback_2 aeolus:out.R
jacktl -c foo:umidi0.0:TX aeolus:Midi/in
```

## Using configuration file 

Edit a `ini` style configuration file as follow:

    ;
    ; file: /etc/jackctl.d/aeolus.conf
    ;
    [MIDIMAN MIDI_1]
	    ; First Keyboard from MIDIMAN MIDISPORT
	    source = midiman-umidi0.0:TX
	    destination = aeolus:Midi/in
    
    [MIDIMAN MIDI_2]
	    ; Second Keyboard from MIDIMAN MIDISPORT
	    source = midiman-umidi0.1:TX
	    destination = aeolus:Midi/in
    
    [aeolus AUDIO_L]
            ; Left AUDIO output
	    source = aeolus:out.L
	    destination = system:playback_1

    [aeolus AUDIO_R]
	    ; Right AUDIO output
	    source = aeolus:out.R
	    destination = system:playback_2
     
There is one section per connection. All previous settings will be disconnected before applying configuration.

Then run `jackctl` as follow:
```sh
jackctl -f /etc/jackctl.d/aeolus.ini
```

# License

GPLV2 or later 
