#define F_CPU 9600000UL
#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>

// Array der Muster f�r die LED-Animation
uint8_t muster[6] = {
	0b00010000,
	0b00001000,
	0b00011000,
	0b00001010,
	0b00011010,
	0b00001110
};

// Funktionsprototypen
void animation();
void initializeRandomGenerator();

int main(void) {
	// Setze PB1-PB4 als Ausgabepins und PB0 als Eingabepin mit Pull-Up-Widerstand
	DDRB = 0b00011110;
	PORTB |= (1 << PB0); // Pull-Up-Widerstand aktivieren

	// Hochz�hlen der LEDs
	for (int i = 0; i < 6; i++) {
		PORTB = muster[i];
		_delay_ms(200);
	}

	// Runterz�hlen der LEDs
	for (int i = 4; i >= 0; i--) {
		PORTB = muster[i];
		_delay_ms(200);
	}
	PORTB = 0x00;

	// Hauptprogrammschleife
	while (1) {
		// �berpr�fe, ob die Taste gedr�ckt ist
		if (!(PINB & (1 << PB0))) {
			// Initialisiere die Zufallszahlengenerierung mit der Zeit, solange der Button gedr�ckt ist
			initializeRandomGenerator();
			
			// Warte auf Loslassen der Taste (Entprellung)
			_delay_ms(10);
			while (!(PINB & (1 << PB0)));

			// F�hre LED-Animation aus
			animation();

			// Generiere zuf�lliges LED-Muster
			uint8_t random_number = rand() % 6;
			uint8_t number = muster[random_number];

			// Zeige das endg�ltige LED-Muster an
			PORTB = number;
		}
		// Warte, um Tastendruckentprellung zu erm�glichen
		_delay_ms(10);
	}

	return 0;
}

// Funktion f�r die LED-Animation
void animation() {
	for (int i = 0; i < 5; i++) {
		PORTB ^= 0b00011110; // Alle LEDs umschalten
		_delay_ms(100);
	}
}

// Funktion, um die Systemzeit seit dem Einschalten zu erhalten
uint16_t millis() {
	static uint16_t milliseconds = 0;
	milliseconds++;
	return milliseconds;
}

// Funktion zur Initialisierung des Zufallszahlengenerators
void initializeRandomGenerator() {
	uint16_t seed = 0;
	// Solange der Button gedr�ckt ist, f�ge die aktuelle Zeit zum Seed hinzu
	while (!(PINB & (1 << PB0))) {
		seed += millis();
		_delay_ms(1);
	}
	srand(seed);
}