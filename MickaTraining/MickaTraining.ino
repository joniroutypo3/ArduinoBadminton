/**
  Projet Bad Gargenville
  Name: GargenvilleBadmintonReflexTime
  Purpose: Dispositif d'entrainement pour le collectif Perf MPG géré par Micka

  @author Jonathan IROULIN
  @version 1.0.1 03/05/18
*/

#include "Timer.h"

#include <Wire.h> //This library allows you to communicate with I2C / TWI devices. https://www.arduino.cc/en/Reference/Wire
#include <LiquidCrystal_I2C.h> //The library allows to control I2C displays with functions extremely similar to LiquidCrystal library. https://www.arduinolibraries.info/libraries/liquid-crystal-i2-c

////////////////////////////////
// Variables de l ecran LCD 16 colonnes x 2 lignes - debut
LiquidCrystal_I2C lcd(0x27,16,2);

// Variables de l ecran LCD 16 colonnes x 2 lignes - fin

////////////////////////////////
// Variables timer - debut
Timer t; // Définition de l'objet timer

int debutTimer = 0;
int tempsDeJeu = 10; //temps de jeu en secondes
int tempsRestant; //temps de jeu restant en secondes

// Variables timer - fin

////////////////////////////////
// Variable Jeu - debut
int debutJeu = 0;
int stopJeu = 0;
long nbrAleatoire;
int nombreBoutons = 2;
long btnInitial;
int btnAllume;
int nombreDePointsJoues = 0;
int nombreDePointsGagnes = 0;
int nombreDePointsPerdus = 0;
int nombreDePointsTentes = 0;
int tableauPointProposes[100];
int tableauPointTentes[100];
int indexTableauPointProposes;
int indexTableauPointTentes ;

// Variable Jeu - fin

////////////////////////////////
// 
int btn1Appuye = 0;
int btn2Appuye = 0;
int btnResetAppuye = 0;

////////////////////////////////
// Définition des entrée/sorties - debut
// Declaration des bornes de led
int led1 = 2; // Voyant Gauche
int led2 = 3; // Voyant Droite
int led3 = 4; // Bouton lumineux Gauche
int led4 = 5; // Bouton lumineux Droite
int ledTemoinJeu = 8;  // Voyant de jeu en cours

// Declaration des bornes de btn
int btn1 = 6; // Bouton de gauche
int btn2 = 7; // Bouton de droite
int btnReset = 9; // Bouton restart

// Définition des entrée/sorties - fin
 
////////////////////////////////
// Init
void setup()
{
  Serial.begin(9600);
  t.every(1000, envoiDecompte); // et celle-ci qui appelle la fonction 'envoi' toutes les secondes
  tempsRestant = tempsDeJeu;
  lcd.init();
  lcd.backlight();
}


void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

}

/**
  Démarre le jeu.
  
*/
void demarrerLeJeu() {
  
}

/**
  Allume les deux voyants.
  
*/
void allumerLesDeuxVoyants() {
  
}

/**
  Eteindre les deux voyants.
  
*/
void eteindreLesDeuxVoyants() {
  
}

/**
  Allume le voyant à jouer.

  @param idVoyant identifiant du voyant à allumer.
  
*/
void allumerLeVoyant(int idVoyant) {
  
}

/**
  Eteindre le voyant à jouer

  @param idVoyant identifiant du voyant à eteindre.
  
*/
void eteindreLeVoyant(int idVoyant) {
  
}

/**
  Allume le bouton lumineux à jouer.

  @param idBoutonLumineux identifiant du bouton lumineux à allumer.
  
*/
void allumerBoutonLumineux(int idBoutonLumineux) {
  
}

/**
  Eteindre le bouton lumineux à jouer.

  @param idBoutonLumineux identifiant du bouton lumineux à eteindre.
  
*/
void eteindreBoutonLumineux(int idBoutonLumineux) {
  
}

/**
  Allume le voyant et le bouton lumineux à jouer.

  @param idVoyantEtBoutonLumineux identifiant du voyant et bouton lumineux à allumer.
  
*/
void allumerLeVoyantEtBouton(int idVoyantEtBoutonLumineux) {
  
}

/**
  Eteindre le voyant et le bouton lumineux à jouer.

  @param idVoyantEtBoutonLumineux identifiant du voyant et bouton lumineux à eteindre.
  
*/
void eteindreLeVoyantEtBouton(int idVoyantEtBoutonLumineux) {
  
}

/**
  Décrémente de un le nombre de point à jouer.
  
*/
void decrementerLesAppuisRestants() {
  
}

/**
  choisi un bouton au hasard.
  
*/
void choisirBoutonHasard() {
  
}

/**
  Attend la temporisation si besoin.
  
*/
void attendreTemporisation() {
  
}

/**
  Démarre le programme.
  
*/
void testerNombreAppuiRestant() {
  
}

/**
  Démarre le temps de jeu.
  
*/
void demarrerLeTempsDeJeu() {
  
}

/**
  Pause le temps de jeu.
  
*/
void pauserLeTempsDeJeu() {
  
}

/**
  Arrête le temps de jeu.
  
*/
void arreterLeTempsDeJeu() {
  
}

/**
  Calculer le temps moyen.
  
*/
void calculerTempsMoyen() {
  
}

/**
  Affiche le temps mis.
  
*/
void afficherLeTemps() {
  
}

/**
  Remet le jeu à zéro avant redemarrage.
  
*/
void remettreAZero() {
  
}
