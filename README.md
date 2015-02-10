# afx-01
AFX-01 Sequencer

The beginnings of an open source hardware sequencer project based on the Teensy 3.1 and an Atmel SAM2695 sound chip.

Hardware (subject to change)
	- Teensy 3.1 Microcontroller
	- Atmel SAM2695 Sound Chip
	- 128x64 OLED Graphic LCD
	- 8x8 RGB LED note indicators
	- 8 vertical buttons
	- 8 horizontal buttons
	- 3x Rotary Encoders
	- 4x mode buttons

Feature Intentions:
	- MIDI sync
	- Live Arpeggiation Mode
	- Live Sequence Transpose Mode
	- Internal sound generation (with Atmel chip)
	- USB Bus / Battery powerable (external USB probably to start)
	- Eurorack quick mount capability? (dependins on final form factor)
	- Scale Quantization
	- Microtonal Scale Capabilities
	- Perlin Noise based generative sequences
	- Fractional Brownian Motion based iterative microtonal sequences
	- Midi Transforms
		- Pre-Quantized Randomizer
		- Cross-sequence influencer
		- Rotational logic
		- Random Step
		- Perlin noise based step
	- MIDI out
	- CV breakout / addon
		- 3x CV out
		- 3x Gate out
		- 3x CC1 out
		- 3x CC2 out
		- 4x Percussion Triggers
