FZ Guitar module

This is a Filter module. When use the right syntax in the text it shows the neck of guitar, notes on the neck and any other text.

The Syntax
~~~~~~~~~~

[guitar|<parameter>|.....
  |<rgb=<Red>,<Green>,<Blue>|<pitch>=<tone, tone, tone,....>|instr=<name of intrument>|rgb2=<rgb2>|noteaut=1]

<parameter>:   <string>=[<place on neck><Text>] or  [<placeon neck><Text>]

If do not use the <string> the last used string is the right string

<string>: character, e,a,d,g,h,e1 or

<place on neck>: number, the used fret on the neck
<Text>: any other text or nothing

Other parameters
~~~~~~~~~~~~~~~~
<rgb>: The color  of written text with Red, Green, Blue component. Components can decimal numbers or hexa numbers
      in format: decimal: Red, Gree, Blue 
                 hexadecimal #RRGGBB
<rgb2>:  The other color schema for special sign                 
<pitch>: The pitch of strings. For example E,A,D,G,H,E1
<instr>: The name of instrument in the database. The default is GUITAR. 
         It can BASS, BASS5, BASS6, GUITAR, GUITARD
<noteaut>:  calculate the pitch of note and write out

