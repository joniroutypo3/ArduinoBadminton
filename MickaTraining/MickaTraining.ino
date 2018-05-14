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
    debug("debutJeu = 0 => Attente du debut de jeu => appuyez sur start", 1);
    lectureDuBoutonStart(debutJeu);
  }
  // Si le jeu est en cours
  if (debutJeu == 1) {
    debug("debutJeu = 1 => On lance le jeu ", 1);
    // Lecture des differents boutons
    // Si le bouton start est pressé => relance du jeu
    lectureDesBoutons();
  }
}

}

/**
  Allume les deux voyants.

*/
void allumerLesDeuxVoyants() {
  debug("Fct allumerLesDeuxVoyants", 1);
  
  allumerLeVoyant("gauche");
  allumerLeVoyant("droite");
}

/**
  Eteindre les deux voyants.

*/
void eteindreLesDeuxVoyants() {
  debug("Fct eteindreLesDeuxVoyants", 1);
  
  eteindreLeVoyant("gauche");
  eteindreLeVoyant("droite");
}

/**
  Allume le voyant à jouer.

  @param idVoyant identifiant du voyant à allumer.

*/
void allumerLeVoyant(String idVoyant) {
  debug("Fct allumerLeVoyant : ", 0);
  debug(idVoyant, 1);
  
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
  debug("Fct eteindreLeVoyant : ", 0);
  debug(idVoyant, 1);
  
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
  debug("Fct allumerBoutonLumineux : ", 0);
  debug(idBoutonLumineux, 1);
  
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
  debug("Fct eteindreBoutonLumineux : ", 0);
  debug(idBoutonLumineux, 1);
  
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
  debug("Fct eteindreLesDeuxBoutonsLumineux", 1);
  
  eteindreBoutonLumineux("gauche");
  eteindreBoutonLumineux("droite");
}

/**
  Allume le voyant et le bouton lumineux à jouer.

  @param idVoyantEtBoutonLumineux identifiant du voyant et bouton lumineux à allumer.

*/
void allumerLeVoyantEtBouton(String idVoyantEtBoutonLumineux) {
  debug("Fct allumerLeVoyantEtBouton : ", 0);
  debug(idVoyantEtBoutonLumineux, 1);
  
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
  debug("Fct eteindreLeVoyantEtBouton : ", 0);
  debug(idVoyantEtBoutonLumineux, 1);
  
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
  debug("Fct decrementerLesAppuisRestants", 1);

}

/**
  choisi un bouton au hasard.

*/
void choixNouveauBouton() {
  debug("Fct choixNouveauBouton", 1);
  
  //Initialisation de random
  randomSeed(analogRead(0));
  int btnSuivant = nombreAleatoire();
  debug("Nouveau nombre aleatoire : ", 0);
  debug(btnSuivant, 1);

  //Si le chiffre  est 1 => gauche 
  if(1 == btnSuivant){
      allumerLeVoyantEtBouton("gauche")
  }
  //Si le chiffre  est 2 => droite
  else if (2 == btnSuivant){
      allumerLeVoyantEtBouton("droite")
  }
 
  allumerLed(btnSuivant);
  btnAllume = btnSuivant;
  incrementePointsJoues();
  stockPointsProposes();
}

/**
  Attend la temporisation si besoin.

*/
void attendreTemporisation() {
  debug("Fct attendreTemporisation", 1);

}

/**
  Démarre le programme.

*/
void testerNombreAppuiRestant() {
  debug("Fct testerNombreAppuiRestant", 1);

}

/**
  Démarre le temps de jeu.

*/
void demarrerLeTempsDeJeu() {
  debug("Fct demarrerLeTempsDeJeu", 1);

}

/**
  Pause le temps de jeu.

*/
void pauserLeTempsDeJeu() {
  debug("Fct pauserLeTempsDeJeu", 1);

}

/**
  Arrête le temps de jeu.

*/
void arreterLeTempsDeJeu() {
  debug("Fct arreterLeTempsDeJeu", 1);

}

/**
  Calculer le temps moyen.

*/
void calculerTempsMoyen() {
  debug("Fct calculerTempsMoyen", 1);

}

/**
  Affiche le temps mis.

*/
void afficherLeTemps() {
  debug("Fct afficherLeTemps", 1);

}

/**
  Remet le jeu à zéro avant redemarrage.

*/
void remettreAZero() {
  debug("Fct remettreAZero", 1);

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
  debug("Fct lectureDuBoutonStart", 1);

  //Lecture du bouton RESET. La valeur 0 = bouton appuyé
  btnResetAppuye = digitalRead(btnReset);
  if (btnResetAppuye == 0 && etatJeu == 0)
  {
    debug("Demmarage de partie dans Fct lectureDuBoutonStart", 1);
    demarrerPartie();
  }
  if (btnResetAppuye == 0 && etatJeu == 1)
  {
    debug("Stop de partie dans Fct lectureDuBoutonStart", 1);
    stoperPartie();
  }

}

/**
  Lecture des bouton gauche et droite

*/
void lectureDesBoutons(){
  debug("Fct lectureDesBoutons", 1);
  
    //Lecture du bouton 1
    btn1Appuye=digitalRead(btn1);
    //Serial.print("valeur du bouton 1: ");
    //Serial.println(btn1Appuye);
    if(btn1Appuye==0)
    {
      verifierBouton(1);
    }
    
    //Lecture du bouton 2
    btn2Appuye=digitalRead(btn2);
    //Serial.print("valeur du bouton 2: ");
    //Serial.println(btn1Appuye);
 
    if(btn2Appuye==0)
    {
      verifierBouton(2);
    }

    //Lecture du bouton start
    lectureDuBoutonStart(debutJeu);
}

/**
  Fonction qui verifie si le bouton appuyé est le bon

*/
void verifierBouton(int btnAVerifier){

  debug("Fct lectureDesBoutons", 1);
  
  debug("Bouton appuyé = n°", 0);
  debug(btnAVerifier, 1);

  if(btnAVerifier == btnAllume){ 
    debug("Bouton appuyé = OK", 1);
    choixNouveauBouton();
  }
}

/**
  Démarre le jeu.

*/
void demarrerPartie() {
  debug("Fct demarrerPartie", 1);

  //
  choixNouveauBouton();
  // On donne la valeur 1 à l'état du jeu
  debutJeu = 1 ;
  // On récupère le temps du début du jeu
  tempoDebutDePartie = millis();
}


/**
  Stop le jeu (Restart).

*/
void stoperPartie() {
  debug("Fct stoperPartie", 1);
  
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
    debug("Fct nombreAleatoire", 1);
    
    nbrAleatoire = random(1, nombreBoutons + 1);
    return nbrAleatoire;
  }


  /**
    Fonction qui retourne le message transmis sur le moniteur serie

  */

  void debug(String messageDebug, int retourLigne) {
    if (1 == debugMoniteurSerie) {
      if (1 == retourLigne) {
        Serial.print(messageDebug);
      }
      else
      {
        Serial.println(messageDebug);
      }
    }
  }

