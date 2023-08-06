<h1 align="center">  Smart blind stick </h1>
<h3 align="center"> Pinout at Features </h3>


<h2 align="center"> ------------- PINOUT ng mga Hardware------------- </h2>

### Vibration ata to
- VibRight - Digital pin 13
- VibLeft - Digital pin 12
- VibMid  - Digital pin 11

### SONAR NAMAN
- RIGHT SONAR TRIG - 34
- RIGHT SONAR ECHO - 35
- LEFT  SONAR TRIG - 36
- LEFT  SONAR ECHO - 37
- MID   SONAR TRIG - 38
- MID   SONAR TRIG - 39

### BUTTONS NAMAN
- Up Button Blue (Day)    - 42
- Mid Button Blue (Night) - 43
- Down Button Blue (Rain) - 44
- SOS Button              - 45
- Standby 			      - 46

### GSM ata to
- GSM RX - 0
- GSM TX - 1
- GSM RST- 2

### DFPlayer Mini mp3 at Speaker
- VCC - 5v
- RX - 1k resistor - 11
- TX - 10
- SPK_1 - Negative sa Speaker
- GND - GND
- SPK_2 - Positive sa Speaker

### MGA DI PA NAKAKABIT
- LDR Sensor


<h2 align="center"> ------------- Features ng Smart Stick ------------- </h2>


## Pa lagyan nalang boss AJ ng check (/) para organize din:
-------------------------------------------------------

### Main Menu Features:
- Gumagana mga modes pag sinelect 	( / )

- May sound kapag Sinelect yung mode for ex. "Rain mode Selected" ( / )

- Emboss sa mga buttons			( / )

-------------------------------------------------------
### Daymode Features:
-------------------------------------------------------

- LDR Sensor can detect light/Cover 
area and have 2 outputs in speaker
"Dark Area detected" 
&
"Light Area detected"  Only 1 time 
once detected				( / )


- Sonar at Vibration Motor function ( / )  
Pag sa Middle Sonar naka detect ng
malapit mag obstacle detected.
for ex. ganto yung range:
	       50 (Vibrate)
sonar left     )))))
(Trig speaker) 50     100 (Vibrate)
sonar mid      ))))) )))))
               50 (Vibrate)
sonar right    )))))

- Natatrack yung location*matik check to  ( / )

- Voice output na "Daymode is activated"  ( / )

--------------------------------------------------------
### Night Mode:
--------------------------------------------------------
- Hazzard na LED on kasi gabi 		( / )

- LDR sensor off				( / )

- SMS notiffication 			(  )  
"Night mode was triggered" 
"Better to assist the user night time
is coming"


- Send also link to the website or 	( )
application


- Voice output na "Nightmode is activated"
and "Dont forget user that it is evening"   ( / )

- Naka on parin function ng sonar ang vib motor	( )


--------------------------------------------------------
### Rain Mode:
--------------------------------------------------------
- All sensor are off			( / )

- SMS notiffication 			( )  
"Rain mode was triggered" 
"Need Relative/Guardian Assistance"

- Hazzard na LED on kasi umuulan		( )


- Send also link to the website or 	( )  
application

- Voice output na "RainMode is activated" ( / )
and "Please assist me to a covered area" 

--------------------------------------------------------
### SOS Mode:  
--------------------------------------------------------
- All sensor are off			( / )

- SMS urgent notif "SOS mode was triggered" 		( / )  
"Emergency! Please help!" 3x

- Send also link to the website or 	( )
application

- Voice output na "SOS mode is activated" ( / )
and "Please help me! need assistance!"

- ~Lagyan ng wang wang kasi emergency (-)~ 

- Hazzard na LED on kasi emergency	( / )

--------------------------------------------------------
### Standby Mode:
--------------------------------------------------------
- Low powersaving Mode			( / )


