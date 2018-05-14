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
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Variables de l ecran LCD 16 colonnes x 2 lignes - fin

////////////////////////////////
// Variables timer - debut
Timer t; // Définition de l'objet timer

int debutTimer = 0;
int tempsDeJeu = 10; //temps de jeu en secondes
int tempsRestant; //temps de jeu restant en secondes
unsigned int tempoDebutDePartie = 0;
unsigned int tempoFinDePartie = 0;

// Variables timer - fin

////////////////////////////////
// Variable Jeu - debut
int attenteDebutJeu = 1;
int debutJeu = 0;
int stopJeu = 0;
long nbrAleatoire; // nombre qui servira à choisir le bouton de gauche ou de droite
int nombreBoutons = 2; // nombre de boutons pour le jeu
int nombreDePointsAJouer = 5; // nombre de points à jouer dans une partie
int nombreDePointsRestant; // nombre de points à jouer avant fin de partie
long btnInitial;
int btnAllume;
int debugMoniteurSerie = 1;
String messageDebug; // variable de message qui sert pour le debug sur le moniteur série

// Variable Jeu - fin

////////////////////////////////
//
int btn1Appuye = 0;
int btn2Appuye = 0;
int btnResetAppuye = 0;

////////////////////////////////
// Définition des entrée/sorties - debut
// Declaration des bornes de led
int voyantGauche = 2; // Voyant Gauche
int voyantDroite = 3; // Voyant Droite
int BoutonLumineuxGauche = 4; // Bouton lumineux Gauche
int BoutonLumineuxDroite = 5; // Bouton lumineux Droite
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
  lcd.init();
  lcd.backlight();
}


void loop() {
  // put your main code here, to run repeatedly:
  // Si le jeu n'est pas encore commencé
  if (debutJeu == 0) {
    //Si le bouton start est pressé => le bouton start lance le jeu
    Serial.println("Attente du debut de jeu => appuyez sur start");
    lectureDuBoutonStart(debutJeu);
  }
  // Si le jeu est en cours
  if (debutJeu == 1) {
    //Si le bouton start est pressé => relance du jeu
    lectureDuBoutonStart(debutJeu);
  }

}

}

/**
  Allume les deux voyants.

*/
void allumerLesDeuxVoyants() {
  allumerLeVoyant("gauche");
  allumerLeVoyant("droite");
}

/**
  Eteindre les deux voyants.

*/
void eteindreLesDeuxVoyants() {
  eteindreLeVoyant("gauche");
  eteindreLeVoyant("droite");
}

/**
  Allume le voyant à jouer.

  @param idVoyant identifiant du voyant à allumer.

*/
void allumerLeVoyant(String idVoyant) {
  eteindreLesDeuxVoyants();
  if (idVoyant == "gauche") {
    digitalWrite(voyantGauche, HIGH);
  }
  if (idVoyant == "droite") {
    digitalWrite(voyantDroite, HIGH);
  }
}

/**
  Eteindre le voyant à jouer

  @param idVoyant identifiant du voyant à eteindre.

*/
void eteindreLeVoyant(String idVoyant) {
  if (idVoyant == "gauche") {
    digitalWrite(voyantGauche, LOW);
  }

  if (idVoyant == "droite") {
    digitalWrite(voyantDroite, LOW);
  }
}

/**
  Allume le bouton lumineux à jouer.

  @param idBoutonLumineux identifiant du bouton lumineux à allumer.

*/
void allumerBoutonLumineux(String idBoutonLumineux) {
  if (idBoutonLumineux == "gauche") {
    digitalWrite(BoutonLumineuxGauche, HIGH);
  }

  if (idBoutonLumineux == "droite") {
    digitalWrite(BoutonLumineuxDroite, HIGH);
  }
}

/**
  Eteindre le bouton lumineux à jouer.

  @param idBoutonLumineux identifiant du bouton lumineux à eteindre.

*/
void eteindreBoutonLumineux(String idBoutonLumineux) {
  if (idBoutonLumineux == "gauche") {
    digitalWrite(BoutonLumineuxGauche, LOW);
  }

  if (idBoutonLumineux == "droite") {
    digitalWrite(BoutonLumineuxDroite, LOW);
  }
}
/**
  Eteindre le bouton lumineux à jouer.

  @param idBoutonLumineux identifiant du bouton lumineux à eteindre.

*/
void eteindreLesDeuxBoutonsLumineux() {
  eteindreBoutonLumineux("gauche");
  eteindreBoutonLumineux("droite");
}

/**
  Allume le voyant et le bouton lumineux à jouer.

  @param idVoyantEtBoutonLumineux identifiant du voyant et bouton lumineux à allumer.

*/
void allumerLeVoyantEtBouton(String idVoyantEtBoutonLumineux) {
  if (idVoyantEtBoutonLumineux == "gauche") {
    allumerBoutonLumineux("gauche");
    allumerLeVoyant("gauche");
  }
  if (idVoyantEtBoutonLumineux == "droite") {
    allumerBoutonLumineux("droite");
    allumerLeVoyant("droite");
  }

}

/**
  Eteindre le voyant et le bouton lumineux à jouer.

  @param idVoyantEtBoutonLumineux identifiant du voyant et bouton lumineux à eteindre.

*/
void eteindreLeVoyantEtBouton(String idVoyantEtBoutonLumineux) {
  if (idVoyantEtBoutonLumineux == "gauche") {
    eteindreBoutonLumineux("gauche");
    eteindreLeVoyant("gauche");
  }
  if (idVoyantEtBoutonLumineux == "droite") {
    eteindreBoutonLumineux("droite");
    eteindreLeVoyant("droite");
  }

}

/**
  Décrémente de un le nombre de point à jouer.

*/
void decrementerLesAppuisRestants() {

}

/**
  choisi un bouton au hasard.

*/
void choixNouveauBouton() {
  //Initialisation de random
  randomSeed(analogRead(0));
  int btnSuivant = nombreAleatoire();
  Serial.print("Nouveau nombre aleatoire : ");
  Serial.println(btnSuivant);
  //On verifie que le bouton suivant soit différent du précédant
  btnSuivant = verifierBoutonSuivantDifferentDuBoutonPrecedent(btnSuivant);
  allumerLed(btnSuivant);
  btnAllume = btnSuivant;
  incrementePointsJoues();
  stockPointsProposes();
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

  //Réinitialiser le nombre de point
  nombreDePointsRestant = nombreDePointsAJouer;

  //Effacer le temps



}

/**
  Lecture du bouton start qui selon l'état du jeu
  - lance le jeu (Start).
  - stop le jeu (Restart).

*/
void lectureDuBoutonStart(int etatJeu) {

  //Lecture du bouton RESET. La valeur 0 = bouton appuyé
  btnResetAppuye = digitalRead(btnReset);
  if (btnResetAppuye == 0 && etatJeu == 0)
  {
    demarrerPartie();
  }
  if (btnResetAppuye == 0 && etatJeu == 1)
  {
    stoperPartie();
  }

}

/**
  Démarre le jeu.

*/
void demarrerPartie() {
  // On donne la valeur 1 à l'état du jeu
  debutJeu = 1 ;
  // On récupère le temps du début du jeu
  tempoDebutDePartie = millis();
}


/**
  Stop le jeu (Restart).

*/
void stoperPartie() {
  if (debutJeu == 1) {
    //Stop le jeu
    debutJeu = 0 ;
    //réinitialise les variables
    tempoFinDePartie = millis();
    remettreAZero();
  }


  /**
    Fonction qui retourne nombre aleatoire

  */
  long nombreAleatoire() {
    nbrAleatoire = random(1, nombreBoutons + 1);
    return nbrAleatoire;
  }


  /**
    Fonction qui retourne le message transmis sur le moniteur serie

  */

  void debug(String messageDebug) {
    if (1 == debugMoniteurSerie) {
      Serial.println(messageDebug);
    }
  }

