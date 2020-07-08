/* **************************************************************************
 * Programm dijkstra.c
 *
 * Gruppe		: 05
 * Name 1		: Friedrich Völkers (391529)
 * Name 2		: Alexander Neigum (372950)
 * Eingabe		: Steuerung des Menüs mit Buchstaben
 *				  und Zahlen um die Größe des Feldes,
 *				  den Startpunkt, das Ziel und
 *				  Hindernisse einzugeben
 * Ausgabe		: Ausgegeben wird das vom Benutzer eingegebene
 *				  Rechteck mit Start, Ziel und den Hindernissen,
 *				  das REchteck, in dem man sieht, welche Knoten
 *				  schon untersucht wurden und die Lösung (der
 *				  kürzeste Weg)
 * Beschreibung : Das Programm soll den kürzesten Weg zwischen
 *				  zwei Punkten ermittlen
 *
 ***************************************************************************/

// Einbindung der Bibliotheken
#include <stdio.h>
#include <stdlib.h>

// Definition der Variablen 'unendlich'
#define unendlich 999999.9;

// Definition der Farben
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_GRA   "\x1b[90m"
#define ANSI_COLOR_GRA2   "\x1b[95m"
#define ANSI_COLOR_RESET   "\x1b[0m"
#define ANSI_COLOR_BLUE   "\x1b[34m"
#define ANSI_COLOR_YELLOW   "\x1b[93m"

// Globale Variablen
int breite, hohe, groesse;
int anzahlfreienfelder;
int aktuellegroessewarte;
int startkoordinaten[2];
int zielkoordinaten[2];
int q = 0;

// Erstellung der Struktur für die Warteschlange
struct warteschlange {

	int positionX;
	int positionY;
	int positionVorX;
	int positionVorY;
	double distanz;
	int abgefragt;
	int inhalt;

};

// Erstellung der Struktur für alle freien Felder
struct allefelder {

	int positionX;
	int positionY;
	int inhalt;
	double distanz;
	// 1 = noch nicht abgefragt und 0 = abgefragt
	int abgefragt;
	int positionVorX;
	int positionVorY;

};

// Erstellung der Struktur für den kürzesten Weg
struct kurzerweg {

	int positionX;
	int positionY;

};

// Einbindung der Funktionen
int felderstellen();
void wartestart();
int dijkstra();
int sortierwarte();
int checknachbarn();
int checkentfernen();
int checken();
int kurzerweg();

// Main-Funktion
int main () {

	// lokale Variablen
	int Temp4;
	char antwort1;

	// Terminal leeren vor dem Start
	system("clear");

	// Erfragung der Größe (Breite und Höhe), solange bis sie maximnal 100 x 100 groß sind
	do {

		printf("Bitte geben Sie die Breite des Feldes ein!\n");
		scanf("%d", &breite);

		printf("Bitte geben Sie nun die Höhe des Feldes ein!\n");
		scanf("%d", &hohe);

		q = 0;

		if (hohe > 100 || breite > 100 ) {

			system("clear");

			printf("Das Feld darf maximal eine Größe vonn 100x100 haben!\n");
			q = 1;

		}

	}	while (q == 1);

	// Erstellung des Arrays für das Feld
	int feld[hohe][breite];

	// ermittlung der anzahal aller felder
	groesse = hohe * breite;

	// Erstellt solange Felder nach den Angaben der Benutzer, bis sie zufrieden sind
	do {

		anzahlfreienfelder = felderstellen(feld);

		system("clear");

		printf("So sieht Ihr Feld aus!\n");

		for (int i = 0; i < hohe; ++i)
		{

			for (int j = 0; j < breite; ++j)
			{

				// Färbt die verschiedenen Felder nach Art
				if (feld[i][j] == 48)
				{
					printf("%c\u2589%c", 91, 93);

				}   else if (feld[i][j] == 83) {

					printf(ANSI_COLOR_YELLOW"%c\u2589%c"ANSI_COLOR_RESET, 91, 93);

				}   else if (feld[i][j] == 84) {

					printf(ANSI_COLOR_BLUE"%c\u2589%c"ANSI_COLOR_RESET, 91, 93);

				}   else if (feld[i][j] == 88) {

					printf(ANSI_COLOR_GRA"%c\u2589%c"ANSI_COLOR_RESET, 91, 93);

				}

			}

			printf("\n");

		}


		// Erzeugung der Legende
		printf("\033[%d;%dH", 2, (breite + 3) * 3 );

		printf("Legende: \n");

		printf("\033[%d;%dH", 3, (breite + 3) * 3 );

		printf("Start: "ANSI_COLOR_YELLOW"%c\u2589%c"ANSI_COLOR_RESET, 91, 93);

		printf("\033[%d;%dH", 4, (breite + 3) * 3 );

		printf("Ziel: "ANSI_COLOR_BLUE"%c\u2589%c"ANSI_COLOR_RESET, 91, 93);

		printf("\033[%d;%dH", 5, (breite + 3) * 3 );

		printf("Hindernis: "ANSI_COLOR_GRA"%c\u2589%c"ANSI_COLOR_RESET, 91, 93);

		printf("\033[%d;%dH", 6, (breite + 3) * 3 );

		printf("Freies Feld: "ANSI_COLOR_RESET"%c\u2589%c"ANSI_COLOR_RESET, 91, 93);

		printf("\033[%d;%dH", 7, (breite + 3) * 3 );

		printf("Schon kontrolliert: "ANSI_COLOR_GRA2"%c\u2589%c"ANSI_COLOR_RESET, 91, 93);

		printf("\033[%d;%dH", hohe + 2, 0 * 3 );

		// FRagt den Nutzer, ob er zufrieden ist
		printf("Sind sie zufrieden damit?\n\n");
		printf("Wenn ja, geben Sie bitte 'j' ein, wenn nicht, können sie die\n");
		printf("Hindernisse neu verteilen, dafür müssen Sie bloß ein 'n' eingeben\n");
		scanf("%c", &antwort1);
		scanf("%c", &antwort1);

		if (antwort1 == 'j') {

			Temp4 = 2;

		}	else if (antwort1 == 'n') {

			Temp4 = 1;

		}	else {

			Temp4 = 1;

		}

	}	while (Temp4 == 1);

	// Größenzuweisung für die Strukturen
	struct allefelder a[anzahlfreienfelder];
	struct warteschlange w[anzahlfreienfelder];

	int n = 0;

	// weist jedem Feld die Distanz 'unendlich' zu, die Position, den Inhalt und ob er schon kontrolliert wurde
	for (int i = 0; i < hohe; ++i) {

		for (int j = 0; j < breite; ++j) {

			if (feld[i][j] != 88) {

				a[n].positionX = j + 1;
				a[n].positionY = i + 1;
				a[n].inhalt = feld[i][j];
				a[n].distanz = unendlich;
				a[n].abgefragt = 1;

				++n;

			}

		}

	}

	// weist dem Startknoten die Distanz null zu
	for (int i = 0; i < anzahlfreienfelder; ++i) {

		if (a[i].inhalt == 83) {

			a[i].distanz = 0;
			a[i].positionVorX = 0;
			a[i].positionVorY = 0;

		}

	}

	// Fügt den Startknoten zu der Warteschlange hinzu
	wartestart(w, feld);

	// Führt den Dijkstra-Algorithmus durch
	dijkstra(aktuellegroessewarte, w, feld, a);

	int m[groesse][2];

	int anzahln = kurzerweg(m, a, feld);

	int ergebnisfeld[hohe][breite];

	// 1 = frei; 2 = Start, 3 = Ziel, 4 = Hinderniss, 5 wird bunt
	for (int i = 0; i < hohe; ++i) {

		for (int j = 0; j < breite; ++j) {

			if (feld[i][j] == 48) {

				ergebnisfeld[i][j] = 1;

			}   else if (feld[i][j] == 83) {

				ergebnisfeld[i][j] = 2;

			}   else if (feld[i][j] == 84) {

				ergebnisfeld[i][j] = 3;

			}   else if (feld[i][j] == 88) {

				ergebnisfeld[i][j] = 4;

			}

		}

	}

	for (int i = 0; i < anzahln; ++i) {

		ergebnisfeld[m[i][1] - 1][m[i][0] - 1] = 5;

	}

	for (int i = 0; i < hohe; ++i) {

		for (int j = 0; j < breite; ++j) {

			if (feld[i][j] == 84) {

				ergebnisfeld[i][j] = 3;

			}   else if (feld[i][j] == 83) {

				ergebnisfeld[i][j] = 2;

			}

		}

	}

	printf("\n");

	printf("\033[%d;%dH", (2 * hohe) + 9, 0 * 3 );

	printf("Der kürzeste Weg vom Start (gelb) zum Ziel (blau) wird durch die grüne Route dargestellt\n");

	// 1 = frei; 2 = Start, 3 = Ziel, 4 = Hinderniss, 5 wird bunt

	for (int i = 0; i < hohe; ++i) {

		for (int j = 0; j < breite; ++j) {

			if (ergebnisfeld[i][j] == 1) {

				printf("%c\u2589%c", 91, 93);

			}   else if (ergebnisfeld[i][j] == 2) {

				printf(ANSI_COLOR_YELLOW"%c\u2589%c"ANSI_COLOR_RESET, 91, 93);

			}   else if (ergebnisfeld[i][j] == 3) {

				printf(ANSI_COLOR_BLUE"%c\u2589%c"ANSI_COLOR_RESET, 91, 93);

			}   else if (ergebnisfeld[i][j] == 4) {

				printf(ANSI_COLOR_GRA"%c\u2589%c"ANSI_COLOR_RESET, 91, 93);

			}   else if (ergebnisfeld[i][j] == 5) {

				printf(ANSI_COLOR_GREEN"%c\u2589%c"ANSI_COLOR_RESET, 91, 93);

			}

		}

		printf("\n");
	}

	// printf("\033[%d;%dH", 2 * hohe + 9, 0 * 2);

	// printf("\033[%d;%dH", 2 * hohe + 9 + (startkoordinaten[0] - 1), (startkoordinaten[1] - 1) * 2);


	return 0;

}

int felderstellen(int feld[hohe][breite]) {

	char eingabe1;
	int eingabe2[2];
	char eingabe3;
	char eingabe4;
	char eingabe5;
	int eingabe6;
	int eingabe7[2];
	int Temp1, Temp2, Temp3;

	for (int i = 0; i < hohe; ++i) {

		for (int j = 0; j < breite; ++j) {

			feld[i][j] = 48;
		}

	}

	// Fügt den Startpunkt hinzu
	printf("Bitte geben Sie die Zeile ein, in der der Startpunkt liegen soll!\n");
	scanf("%d", &startkoordinaten[1]);
	printf("Bitte geben Sie die Spalte ein, in der der Startpunkt liegen soll!\n");
	scanf("%d", &startkoordinaten[0]);

	feld[startkoordinaten[1] - 1][startkoordinaten[0] - 1] = 83;

	// Fügt die Zielkoordinaten hinzu
	printf("Bitte geben Sie die Zeile ein, in der das Ziel liegen soll!\n");
	scanf("%d", &zielkoordinaten[1]);
	printf("Bitte geben Sie die Spalte ein, in der das Ziel liegen soll!\n");
	scanf("%d", &zielkoordinaten[0]);

	feld[zielkoordinaten[1] - 1][zielkoordinaten[0] - 1] = 84;

	do {

		// fügt einzelne HIndernisse hinzu
		printf("Wollen Sie noch (einzelne) Hindernisse hinzufügen?\n");
		printf("Tipp: Gleich können Sie noch ganze Reihen voller Hindernisse hinzufügen ;)\n");
		printf("Wenn ja, geben sie bitte 'j' ein und falls nicht ein 'n'\n");
		scanf("%c", &eingabe1);
		scanf("%c", &eingabe1);

		if (eingabe1 == 'j') {

			printf("Bitte geben Sie die Zeile ein, in der das Hindernis liegen soll\n");
			scanf("%d", &eingabe2[1]);
			printf("Bitte geben Sie die Spalte ein, in der das Hindernis liegen soll\n");
			scanf("%d", &eingabe2[0]);

			feld[eingabe2[1] - 1][eingabe2[0] - 1] = 88;

			do {

				printf("Wollen Sie noch weitere Hindernisse hinzufügen?\n");
				printf("Falls ja, geben Sie bitte 'j' ein und falls nicht bitte ein 'n'\n");
				scanf("%c", &eingabe3);
				scanf("%c", &eingabe3);

				if (eingabe3 == 'j') {

					Temp2 = 1;

				}	else if (eingabe3 == 'n') {

					Temp2 = 2;

				}	else {

					Temp2 = 1;

				}

			}	while (Temp2 == 1);

			Temp1 = 1;

		}	else if (eingabe1 == 'n') {

			Temp1 = 1;

		}	else {

			system("clear");
			printf("Ihre Eingabe war falsch, bitte wiederholen Sie ihre Eingabe!\n");

			Temp1 = 2;

		}

	}	while (Temp1 == 2);

	do {

		// Fügt ganze Reihen voller Hindernisse hinzu
		printf("Sie können nun auch noch zwei Punkte in einer Zeile oder in einer Spalte mit Hindernissen verbinden\n");
		printf("Wenn sie das wollen, geben sie 'j' für ja ein und 'n' für nein.\n");
		scanf("%c", &eingabe4);
		scanf("%c", &eingabe4);

		if (eingabe4 == 'j') {

			do {

				printf("Wollen Sie Hindernisse in einer Spalte oder in einer Zeile hinzufügen?\n");
				printf("Um welche in einer Spalte hinzuzufügen, geben Sie bitte ein 's' ein.\n");
				printf("Um welche ein einer Zeile hinzuzufügen, geben Sie bitte ein 'z' ein.\n");
				scanf("%c", &eingabe5);
				scanf("%c", &eingabe5);

				if (eingabe5 == 's') {

					printf("Bitte geben sie an, in welcher Spalte sie Hindernisse hinzufügen wollen.\n");
					scanf("%d", &eingabe6);
					printf("Bitte geben sie an, in welcher Zeile die Hindernisse anfangen sollen.\n");
					printf("Hinweis: Der Anfang ist das obere Ende der Spalte\n");
					scanf("%d", &eingabe7[0]);
					printf("Bitte geben sie an, in welcher Zeile die Hindernisse aufhören sollen.\n");
					scanf("%d", &eingabe7[1]);

					for (int i = eingabe7[0] - 1; i < eingabe7[1]; ++i) {

						feld[i][eingabe6 - 1] = 88;

					}

					do {

						Temp3 = 1;

						printf("Wollen Sie noch mehr Hindernisse in Spalten und Zeilen hinzufügen?\n");
						printf("Wenn ja, geben Sie 'j' ein und wenn nicht geben Sie bitte ein 'n' ein.\n");
						scanf("%c", &eingabe1);
						scanf("%c", &eingabe1);

						if (eingabe1 == 'j') {

							Temp2 = 2;


						} 	else if (eingabe1 == 'n') {

							Temp2 = 1;

						}	else {

							Temp3 = 2;

						}

					}	while (Temp3 == 2);

				}	else if (eingabe5 == 'z') {

					printf("Bitte geben sie an, in welcher Zeile sie Hindernisse hinzufügen wollen.\n");
					scanf("%d", &eingabe6);
					printf("Bitte geben sie an, in welcher Spalte die Hindernisse anfangen sollen.\n");
					printf("Hinweis: Der Anfang ist das linke Ende der Spalte\n");
					scanf("%d", &eingabe7[0]);
					printf("Bitte geben sie an, in welcher Spalte die Hindernisse aufhören sollen.\n");
					scanf("%d", &eingabe7[1]);

					for (int i = eingabe7[0] - 1; i < eingabe7[1]; ++i) {

						feld[eingabe6 - 1][i] = 88;

					}

					do {

						Temp3 = 1;

						printf("Wollen Sie noch mehr Hindernisse in Spalten und Zeilen hinzufügen?\n");
						printf("Wenn ja, geben Sie 'j' ein und wenn nicht geben Sie bitte ein 'n' ein.\n");
						scanf("%c", &eingabe1);
						scanf("%c", &eingabe1);

						if (eingabe1 == 'j') {

							Temp2 = 2;


						} 	else if (eingabe1 == 'n') {

							Temp2 = 1;

						}	else {

							Temp3 = 2;

						}

					}	while (Temp3 == 2);

				}	else {

					Temp2 = 2;

				}

			}	while (Temp2 == 2);


		}	else if (eingabe4 == 'n') {

			Temp1 = 1;

		}	else {

			Temp1 = 2;

		}

	}	while (Temp1 == 2);

	anzahlfreienfelder = 0;

	for (int i = 0; i < hohe; ++i) {

		for (int j = 0; j < breite; ++j) {

			if (feld[i][j] != 88) {

				++anzahlfreienfelder;

			}

		}

	}

	return anzahlfreienfelder;

}

// Fügt den Startknoten zu der Warteschlange hinzu
void wartestart(struct warteschlange w[anzahlfreienfelder], int feld[hohe][breite]) {

	aktuellegroessewarte = 1;

	w[0].positionX = startkoordinaten[0];
	w[0].positionY = startkoordinaten[1];
	w[0].distanz = 0;
	// 1 = noch nicht abgefragt; 0 = schon abgefragt
	w[0].abgefragt = 1;
	w[0].inhalt = feld[startkoordinaten[1] - 1][startkoordinaten[0] - 1];

}

int dijkstra(int aktuellegroessewarte, struct warteschlange w[anzahlfreienfelder], int feld[hohe][breite], struct allefelder a[anzahlfreienfelder]) {

	printf("\033[%d;%dH", hohe + 7, 0 * 2);

	printf("Hier könne Sie verfolgen, wie sich der Algorithmus ausbreitet:\n");

	// Zeigt dem User, welche wie sein FEld aussieht, bei dem er verfolgen kann, wie weit der Algorithmus ist
	for (int i = 0; i < hohe; ++i)
	{

		for (int j = 0; j < breite; ++j)
		{

			if (feld[i][j] == 48)
			{
				printf("%c\u2589%c", 91, 93);

			}   else if (feld[i][j] == 83) {

				printf(ANSI_COLOR_YELLOW"%c\u2589%c"ANSI_COLOR_RESET, 91, 93);

			}   else if (feld[i][j] == 84) {

				printf(ANSI_COLOR_BLUE"%c\u2589%c"ANSI_COLOR_RESET, 91, 93);

			}   else if (feld[i][j] == 88) {

				printf(ANSI_COLOR_GRA"%c\u2589%c"ANSI_COLOR_RESET, 91, 93);

			}

		}

		printf("\n");

	}

	do {

		if (aktuellegroessewarte > 0) {

			if (aktuellegroessewarte > 1) {

				sortierwarte(w, aktuellegroessewarte);

			}

		}

		// checkt alle direkt umliegenden Knoten, fügt sie falls notwenig in die Warteschlange ein und aktualisiert den Wer
		// der aktuellen größe der Warteschlange
		aktuellegroessewarte = checknachbarn(w, a, feld, aktuellegroessewarte, anzahlfreienfelder);

		// Fügt die untersuchten Knoten zu dem Feld hinzu, bei dem der Nutzer den Fortschritt des Algorithmus verfolgen kann
		printf("\033[%d;%dH", hohe + w[0].positionY + 7, (w[0].positionX - 1) * 3 );

		printf(ANSI_COLOR_GRA2"%c\u2589%c"ANSI_COLOR_RESET, 91, 93);

		// Wir haben hier versucht ein Art "DELAY" hinzuzufügen, damit der Nutzer schritt füt Schritt die Entwicklung verfolgen kann
		// das hat leider nicht geklappt
		// mit keiner existierenden Funktion wie z.B. delay();
		// und auch nicht mit einer  For-Schleife, die bis 40.000.000 geht

		printf("\033[%d;%dH", (2 * hohe) + 8, 0 * 3 );

		aktuellegroessewarte = checkentfernen(w, aktuellegroessewarte);

		// wiederholt den Vorgang, bis keine Knoten mehr in der Warteschlange sind
	}	while (aktuellegroessewarte > 0);

	return 0;

}

// sortiert die Warteschlange nach der Distanz 
int sortierwarte(struct warteschlange w[anzahlfreienfelder], int aktuellegroessewarte) {

	int z;

	do {

		for (int i = 0; i < (aktuellegroessewarte - 1); ++i) {

			int j = i + 1;

			if (w[j].distanz < w[i].distanz) {

				int positionXTemp, positionYTemp, positionVorXTemp;
				int positionVorYTemp, abgefragtTemp, inhaltTemp;
				double distanzTemp;

				positionXTemp = w[i].positionX;
				positionYTemp = w[i].positionY;
				positionVorXTemp = w[i].positionVorX;
				positionVorYTemp = w[i].positionVorY;
				distanzTemp = w[i].distanz;
				abgefragtTemp = w[i].abgefragt;
				inhaltTemp = w[i].inhalt;

				w[i].positionX = w[j].positionX;
				w[i].positionY = w[j].positionY;
				w[i].positionVorX = w[j].positionVorX;
				w[i].positionVorY = w[j].positionVorY;
				w[i].distanz = w[j].distanz;
				w[i].abgefragt = w[j].abgefragt;
				w[i].inhalt = w[j].inhalt;

				w[j].positionX = positionXTemp;
				w[j].positionY = positionYTemp;
				w[j].positionVorX = positionVorXTemp;
				w[j].positionVorY = positionVorYTemp;
				w[j].distanz = distanzTemp;
				w[j].abgefragt = abgefragtTemp;
				w[j].inhalt = inhaltTemp;

			}

		}

		z = 0;

		for (int i = 0; i < (aktuellegroessewarte - 1); ++i) {

			int j = i + 1;

			if (w[j].distanz < w[i].distanz) {

				z = 2;

			}

		}

	}	while (z == 2);

	return 0;

}

// untersucht die Nachbarknoten
int checknachbarn(struct warteschlange w[anzahlfreienfelder], struct allefelder a[anzahlfreienfelder], int feld[hohe][breite], int aktuellegroessewarte, int anzahlfreienfelder) {

	int richtung[2];
	int v, z, Temp;

	int o = anzahlfreienfelder;

	// Oben
	v = o;
	richtung[0] = w[0].positionX;
	richtung[1] = w[0].positionY - 1;

	// guckt, ob der Nachbarknoten in dem Feld liegr
	for (int i = 0; i < anzahlfreienfelder; ++i) {

		if (a[i].positionX == richtung[0] && a[i].positionY == richtung[1]) {

			v = i;

		}

	}

	// guckt, ob er schon abgefragt wurde
	if (a[v].abgefragt == 1 && v != o) {

		z = 0;

		for (int i = 0; i < aktuellegroessewarte; ++i) {

			if (w[i].positionX == richtung[0] && w[i].positionY == richtung[1]) {

				z = 2;
				Temp = i;

			}

		}

		// aktualisiert den Wert der Distanz, falls die neue kürzer ist
		if (z == 2 /* Knoten existiert schon */ ) {

			if (w[Temp].distanz > w[0].distanz + 1) {

				w[Temp].distanz = w[0].distanz + 1;

				a[v].distanz = w[0].distanz + 1;

			}

			// fügt ihn zur Warteschlange hinzu, falls er noch nicht drin ist
		}   else { /* Knoten existiert noch nicht */

			aktuellegroessewarte = aktuellegroessewarte + 1;

			// w[m-1].prio
			w[aktuellegroessewarte - 1].positionX = richtung[0];
			w[aktuellegroessewarte - 1].positionY = richtung[1];
			w[aktuellegroessewarte - 1].positionVorX = w[0].positionX;
			w[aktuellegroessewarte - 1].positionVorY = w[0].positionY;
			w[aktuellegroessewarte - 1].distanz = w[0].distanz + 1;
			w[aktuellegroessewarte - 1].abgefragt = 1;
			w[aktuellegroessewarte - 1].inhalt = feld[richtung[1] - 1][richtung[0] - 1];

			a[v].distanz = w[0].distanz + 1;
			a[v].positionVorX = w[0].positionX;
			a[v].positionVorY = w[0].positionY;


		}

	}

	// Links
	v = o;
	richtung[0] = w[0].positionX - 1;
	richtung[1] = w[0].positionY;

	for (int i = 0; i < anzahlfreienfelder; ++i) {

		if (a[i].positionX == richtung[0] && a[i].positionY == richtung[1]) {

			v = i;

		}

	}

	if (a[v].abgefragt == 1 && v != o) {

		z = 0;

		for (int i = 0; i < aktuellegroessewarte; ++i) {

			if (w[i].positionX == richtung[0] && w[i].positionY == richtung[1]) {

				z = 2;
				Temp = i;

			}

		}

		if (z == 2 /* Knoten existiert schon */ ) {

			if (w[Temp].distanz > w[0].distanz + 1) {

				w[Temp].distanz = w[0].distanz + 1;

				a[v].distanz = w[0].distanz + 1;

			}

		}   else { /* Knoten existiert noch nicht */

			aktuellegroessewarte = aktuellegroessewarte + 1;

			// w[m-1].prio
			w[aktuellegroessewarte - 1].positionX = richtung[0];
			w[aktuellegroessewarte - 1].positionY = richtung[1];
			w[aktuellegroessewarte - 1].positionVorX = w[0].positionX;
			w[aktuellegroessewarte - 1].positionVorY = w[0].positionY;
			w[aktuellegroessewarte - 1].distanz = w[0].distanz + 1;
			w[aktuellegroessewarte - 1].abgefragt = 1;
			w[aktuellegroessewarte - 1].inhalt = feld[richtung[1] - 1][richtung[0] - 1];

			a[v].distanz = w[0].distanz + 1;
			a[v].positionVorX = w[0].positionX;
			a[v].positionVorY = w[0].positionY;


		}

	}

	// Unten
	v = o;
	richtung[0] = w[0].positionX;
	richtung[1] = w[0].positionY + 1;

	for (int i = 0; i < anzahlfreienfelder; ++i) {

		if (a[i].positionX == richtung[0] && a[i].positionY == richtung[1]) {

			v = i;

		}

	}

	if (a[v].abgefragt == 1 && v != o) {

		z = 0;

		for (int i = 0; i < aktuellegroessewarte; ++i) {

			if (w[i].positionX == richtung[0] && w[i].positionY == richtung[1]) {

				z = 2;
				Temp = i;

			}

		}

		if (z == 2 /* Knoten existiert schon */ ) {

			if (w[Temp].distanz > w[0].distanz + 1) {

				w[Temp].distanz = w[0].distanz + 1;

				a[v].distanz = w[0].distanz + 1;

			}

		}   else { /* Knoten existiert noch nicht */

			aktuellegroessewarte = aktuellegroessewarte + 1;

			// w[m-1].prio
			w[aktuellegroessewarte - 1].positionX = richtung[0];
			w[aktuellegroessewarte - 1].positionY = richtung[1];
			w[aktuellegroessewarte - 1].positionVorX = w[0].positionX;
			w[aktuellegroessewarte - 1].positionVorY = w[0].positionY;
			w[aktuellegroessewarte - 1].distanz = w[0].distanz + 1;
			w[aktuellegroessewarte - 1].abgefragt = 1;
			w[aktuellegroessewarte - 1].inhalt = feld[richtung[1] - 1][richtung[0] - 1];

			a[v].distanz = w[0].distanz + 1;
			a[v].positionVorX = w[0].positionX;
			a[v].positionVorY = w[0].positionY;


		}

	}

	// Rechts
	v = o;
	richtung[0] = w[0].positionX + 1;
	richtung[1] = w[0].positionY;

	for (int i = 0; i < anzahlfreienfelder; ++i) {

		if (a[i].positionX == richtung[0] && a[i].positionY == richtung[1]) {

			v = i;

		}

	}

	if (a[v].abgefragt == 1 && v != o) {

		z = 0;

		for (int i = 0; i < aktuellegroessewarte; ++i) {

			if (w[i].positionX == richtung[0] && w[i].positionY == richtung[1]) {

				z = 2;
				Temp = i;

			}

		}

		if (z == 2 /* Knoten existiert schon */ ) {

			if (w[Temp].distanz > w[0].distanz + 1) {

				w[Temp].distanz = w[0].distanz + 1;

				a[v].distanz = w[0].distanz + 1;

			}

		}   else { /* Knoten existiert noch nicht */

			aktuellegroessewarte = aktuellegroessewarte + 1;

			// w[m-1].prio
			w[aktuellegroessewarte - 1].positionX = richtung[0];
			w[aktuellegroessewarte - 1].positionY = richtung[1];
			w[aktuellegroessewarte - 1].positionVorX = w[0].positionX;
			w[aktuellegroessewarte - 1].positionVorY = w[0].positionY;
			w[aktuellegroessewarte - 1].distanz = w[0].distanz + 1;
			w[aktuellegroessewarte - 1].abgefragt = 1;
			w[aktuellegroessewarte - 1].inhalt = feld[richtung[1] - 1][richtung[0] - 1];

			a[v].distanz = w[0].distanz + 1;
			a[v].positionVorX = w[0].positionX;
			a[v].positionVorY = w[0].positionY;


		}

	}

	// obenlinks
	v = o;
	richtung[0] = w[0].positionX - 1;
	richtung[1] = w[0].positionY - 1;

	for (int i = 0; i < anzahlfreienfelder; ++i) {

		if (a[i].positionX == richtung[0] && a[i].positionY == richtung[1]) {

			v = i;

		}

	}

	if (a[v].abgefragt == 1 && v != o) {

		z = 0;

		for (int i = 0; i < aktuellegroessewarte; ++i) {

			if (w[i].positionX == richtung[0] && w[i].positionY == richtung[1]) {

				z = 2;
				Temp = i;

			}

		}

		if (z == 2 /* Knoten existiert schon */ ) {

			if (w[Temp].distanz > w[0].distanz + 1.414213562) {

				w[Temp].distanz = w[0].distanz + 1.414213562;

				a[v].distanz = w[0].distanz + 1.414213562;

			}

		}   else { /* Knoten existiert noch nicht */

			aktuellegroessewarte = aktuellegroessewarte + 1;

			// w[m-1].prio
			w[aktuellegroessewarte - 1].positionX = richtung[0];
			w[aktuellegroessewarte - 1].positionY = richtung[1];
			w[aktuellegroessewarte - 1].positionVorX = w[0].positionX;
			w[aktuellegroessewarte - 1].positionVorY = w[0].positionY;
			w[aktuellegroessewarte - 1].distanz = w[0].distanz + 1.414213562;
			w[aktuellegroessewarte - 1].abgefragt = 1;
			w[aktuellegroessewarte - 1].inhalt = feld[richtung[1] - 1][richtung[0] - 1];

			a[v].distanz = w[0].distanz + 1.414213562;
			a[v].positionVorX = w[0].positionX;
			a[v].positionVorY = w[0].positionY;


		}

	}

	// Unten links
	v = o;
	richtung[0] = w[0].positionX - 1;
	richtung[1] = w[0].positionY + 1;

	for (int i = 0; i < anzahlfreienfelder; ++i) {

		if (a[i].positionX == richtung[0] && a[i].positionY == richtung[1]) {

			v = i;

		}

	}

	if (a[v].abgefragt == 1 && v != o) {

		z = 0;

		for (int i = 0; i < aktuellegroessewarte; ++i) {

			if (w[i].positionX == richtung[0] && w[i].positionY == richtung[1]) {

				z = 2;
				Temp = i;

			}

		}

		if (z == 2 /* Knoten existiert schon */ ) {

			if (w[Temp].distanz > w[0].distanz + 1.414213562) {

				w[Temp].distanz = w[0].distanz + 1.414213562;

				a[v].distanz = w[0].distanz + 1.414213562;

			}

		}   else { /* Knoten existiert noch nicht */

			aktuellegroessewarte = aktuellegroessewarte + 1;

			// w[m-1].prio
			w[aktuellegroessewarte - 1].positionX = richtung[0];
			w[aktuellegroessewarte - 1].positionY = richtung[1];
			w[aktuellegroessewarte - 1].positionVorX = w[0].positionX;
			w[aktuellegroessewarte - 1].positionVorY = w[0].positionY;
			w[aktuellegroessewarte - 1].distanz = w[0].distanz + 1.414213562;
			w[aktuellegroessewarte - 1].abgefragt = 1;
			w[aktuellegroessewarte - 1].inhalt = feld[richtung[1] - 1][richtung[0] - 1];

			a[v].distanz = w[0].distanz + 1.414213562;
			a[v].positionVorX = w[0].positionX;
			a[v].positionVorY = w[0].positionY;


		}

	}

	// untenrechts
	v = o;
	richtung[0] = w[0].positionX + 1;
	richtung[1] = w[0].positionY + 1;

	for (int i = 0; i < anzahlfreienfelder; ++i) {

		if (a[i].positionX == richtung[0] && a[i].positionY == richtung[1]) {

			v = i;

		}

	}

	if (a[v].abgefragt == 1 && v != o) {

		z = 0;

		for (int i = 0; i < aktuellegroessewarte; ++i) {

			if (w[i].positionX == richtung[0] && w[i].positionY == richtung[1]) {

				z = 2;
				Temp = i;

			}

		}

		if (z == 2 /* Knoten existiert schon */ ) {

			if (w[Temp].distanz > w[0].distanz + 1.414213562) {

				w[Temp].distanz = w[0].distanz + 1.414213562;

				a[v].distanz = w[0].distanz + 1.414213562;

			}

		}   else { /* Knoten existiert noch nicht */

			aktuellegroessewarte = aktuellegroessewarte + 1;

			// w[m-1].prio
			w[aktuellegroessewarte - 1].positionX = richtung[0];
			w[aktuellegroessewarte - 1].positionY = richtung[1];
			w[aktuellegroessewarte - 1].positionVorX = w[0].positionX;
			w[aktuellegroessewarte - 1].positionVorY = w[0].positionY;
			w[aktuellegroessewarte - 1].distanz = w[0].distanz + 1.414213562;
			w[aktuellegroessewarte - 1].abgefragt = 1;
			w[aktuellegroessewarte - 1].inhalt = feld[richtung[1] - 1][richtung[0] - 1];

			a[v].distanz = w[0].distanz + 1.414213562;
			a[v].positionVorX = w[0].positionX;
			a[v].positionVorY = w[0].positionY;


		}

	}

	// obenrechts
	v = o;
	richtung[0] = w[0].positionX + 1;
	richtung[1] = w[0].positionY - 1;

	for (int i = 0; i < anzahlfreienfelder; ++i) {

		if (a[i].positionX == richtung[0] && a[i].positionY == richtung[1]) {

			v = i;

		}

	}

	if (a[v].abgefragt == 1 && v != o) {

		z = 0;

		for (int i = 0; i < aktuellegroessewarte; ++i) {

			if (w[i].positionX == richtung[0] && w[i].positionY == richtung[1]) {

				z = 2;
				Temp = i;

			}

		}

		if (z == 2 /* Knoten existiert schon */ ) {

			if (w[Temp].distanz > w[0].distanz + 1.414213562) {

				w[Temp].distanz = w[0].distanz + 1.414213562;

				a[v].distanz = w[0].distanz + 1.414213562;

			}

		}   else { /* Knoten existiert noch nicht */

			aktuellegroessewarte = aktuellegroessewarte + 1;

			// w[m-1].prio
			w[aktuellegroessewarte - 1].positionX = richtung[0];
			w[aktuellegroessewarte - 1].positionY = richtung[1];
			w[aktuellegroessewarte - 1].positionVorX = w[0].positionX;
			w[aktuellegroessewarte - 1].positionVorY = w[0].positionY;
			w[aktuellegroessewarte - 1].distanz = w[0].distanz + 1.414213562;
			w[aktuellegroessewarte - 1].abgefragt = 1;
			w[aktuellegroessewarte - 1].inhalt = feld[richtung[1] - 1][richtung[0] - 1];

			a[v].distanz = w[0].distanz + 1.414213562;
			a[v].positionVorX = w[0].positionX;
			a[v].positionVorY = w[0].positionY;


		}

	}


	// weist dem untersuchten Knoten den Zustand 'untersucht' zu
	for (int i = 0; i < anzahlfreienfelder; ++i) {

		if (a[i].positionX == w[0].positionX && a[i].positionY == w[0].positionY) {

			a[i].abgefragt = 0;
			// a[i].distanz = w[i].distanz;

		}

	}

	return aktuellegroessewarte;

}

// entfernt den untersuchten KNoten aus der Warteschlange
int checkentfernen(struct warteschlange w[anzahlfreienfelder], int aktuellegroessewarte) {

	for (int i = 0; i < (aktuellegroessewarte - 1); ++i) {

		w[i].positionX = w[i + 1].positionX;
		w[i].positionY = w[i + 1].positionY;
		w[i].positionVorX = w[i + 1].positionVorX;
		w[i].positionVorY = w[i + 1].positionVorY;
		w[i].distanz = w[i + 1].distanz;
		w[i].abgefragt = w[i + 1].abgefragt;
		w[i].inhalt = w[i + 1].inhalt;

	}

	aktuellegroessewarte = aktuellegroessewarte - 1;

	return aktuellegroessewarte;

}

// ermittelt den kürzesten Weg
int kurzerweg(int m[groesse][2], struct allefelder a[anzahlfreienfelder], int feld[hohe][breite], struct kurzerweg wa[groesse]) {

	int n = 0;
	int z = 0;
	int Temp6, Temp7;

	// sucht den Zielknoten
	for (int i = 0; i < anzahlfreienfelder; ++i) {

		if (a[i].inhalt == 84) {

			m[n][0] = a[i].positionX;
			m[n][1] = a[i].positionY;

			++n;

		}

	}

	// speichert in dem Array m immer den Vorgängerknoten, bis er beim Startknoten ist
	do {

		Temp6 = m[n - 1][0];
		Temp7 = m[n - 1][1];

		for (int i = 0; i < anzahlfreienfelder; ++i) {

			if (a[i].positionX == Temp6 && a[i].positionY == Temp7) {

				if (a[i].positionVorX == 0 && a[i].positionVorY == 0) {

					z = 1;

				}   else {

					m[n][0] = a[i].positionVorX;
					m[n][1] = a[i].positionVorY;

					++n;

				}

			}

		}

	}   while ( z != 1);

	return n;

}