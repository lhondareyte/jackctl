# JACKCTL 1

## SYNOPSIS 

`jackclt` [`-c|-d src dst`] [`-f file`] [`-l`] [`-C`] [`-D`] 

## DESCRIPTION 

`jackctl` is a command line interface to manage ports connections on JACK audio server. It can be used to connect JACK ports in bacth mode.

## OPTIONS

`-l`
  list available ports

`-c source destination`
  connect source to destination

`-d source destination`
  disconnect source from destination

`-f file`
  connect ports from configuration file

`-C`
  list all connections

`-D`
  Disconnect all ports

## CONFIGURATION FILE
The configuration file is in `ini` style file format. Each section corresponds to a connection between two ports. The section name is ignored. Each section must include a source and a destination.

```` 
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
````

## AUTHOR

`jackctl` was written by Luc Hondareyte - https://github.com/lhondareyte
