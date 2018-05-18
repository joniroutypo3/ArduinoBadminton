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
int temporisationEntreLesPoints = 200;
unsigned int tempoDebutDePartie = 0;
unsigned int tempoFinDePartie = 0;
unsigned int dureeTotalePartie = 0;
unsigned int dureeTotalePartieEnSecondes = 0;
unsigned int dureeTotalePartieEnMiliSecondes = 0;
unsigned int dureeTotaleDeplacementPartie = 0;
unsigned int dureeTotaleDeplacementPartieEnSecondes = 0;
unsigned int dureeTotaleDeplacementPartieEnMiliSecondes = 0;
unsigned int dureeMoyenneDeplacementPartie = 0;
unsigned int dureeMoyenneDeplacementPartieEnSecondes = 0;
unsigned int dureeMoyenneDeplacementPartieEnMiliSecondes = 0;


// Variables timer - fin

////////////////////////////////
// Variable Jeu - debut
int attenteDebutJeu = 1;
int debutJeu = 0;
int stopJeu = 0;
long nbrAleatoire; // nombre qui servira à choisir le bouton de gauche ou de droite
int minAleatoire = 0; // nombre qui servira à definir le minimum aléatoire
int maxAleatoire = 1 ; // nombre qui servira à definir le maximum aléatoire
int nombreBoutons = 2; // nombre de boutons pour le jeu
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

// Définition du tableau des sequences de points car l'aléatoire n'est pas viable pour 2 boutons - debut
const int nombreDeSequence = 5; //nombre de séquence de jeu pré-définies.
const int nombreDePointsParSequence = 20; //nombre de point par sequence
int nombreDePointsAJouer = nombreDePointsParSequence; // nombre de points à jouer dans une partie
int nombreDePointsRestant; // nombre de points à jouer avant fin de partie
int nombreDePointsJouesDansLaSequence; // nombre de points joués avant fin de partie (utile si Micka arrete la séquence avant le nombre de point par séquence)

int sequences[nombreDeSequence][nombreDePointsParSequence] = {{1, 2, 1, 1, 2, 1, 2, 2, 2, 1, 2, 2, 1, 1, 2, 1, 2, 2, 1, 2}, {1, 1, 2, 1, 2, 2, 1, 2, 2, 1, 1, 2, 1, 2, 1, 1, 1, 2, 1, 2}, {2, 2, 1, 2, 1, 1, 1, 2, 1, 1, 2, 1, 2, 1, 1, 1, 2, 1, 1, 2}, {2, 1, 1, 1, 2, 1, 2, 1, 2, 1, 1, 2, 1, 2, 2, 1, 1, 2, 1, 1}, {1, 1, 2, 2, 1, 1, 2, 1, 2, 1, 2, 1, 2, 1, 1, 2, 2, 1, 2, 1}};
int sequenceEnCours [20]; // initialisation du tableau pour sequence en cours.
// Définition du tableau des sequences de points car l'aléatoire n'est pas viable pour 2 boutons  fin

////////////////////////////////
// Init
void setup()
{
  Serial.begin(9600);
  //t.every(1000, envoiDecompte); // et celle-ci qui appelle la fonction 'envoi' toutes les secondes
  lcd.init();
  lcd.backlight();
}


void loop() {
  // put your main code here, to run repeatedly:
  // Si le jeu n'est pas encore commencé
  if (debutJeu == 0) {
    //Si le bouton start est pressé => le bouton start lance le jeu
    //debug("debutJeu = 0 => Attente du debut de jeu => appuyez sur start", 1);
    lectureDuBoutonStart(debutJeu);
  }
  // Si le jeu est en cours
  if (debutJeu == 1) {

    if ( nombreDePointsRestant >= 0 ) {
      //debug("debutJeu = 1 => On lance le jeu ", 1);
      // Lecture des differents boutons
      // Si le bouton start est pressé => relance du jeu
      lectureDesBoutons();
    }
    else {
      stoperPartie();
    }
  }
}

/**
  Allume les deux voyants.

*/
void allumerLesDeuxVoyants() {
  //debug("Fct allumerLesDeuxVoyants", 1);

  allumerLeVoyant("gauche");
  allumerLeVoyant("droite");
}

/**
  Eteindre les deux voyants.

*/
void eteindreLesDeuxVoyants() {
  //debug("Fct eteindreLesDeuxVoyants", 1);

  eteindreLeVoyant("gauche");
  eteindreLeVoyant("droite");
}

/**
  Allume le voyant à jouer.

  @param idVoyant identifiant du voyant à allumer.

*/
void allumerLeVoyant(String idVoyant) {
  //debug("Fct allumerLeVoyant : ", 0);
  //debug(idVoyant, 1);

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
  //debug("Fct eteindreLeVoyant : ", 0);
  //debug(idVoyant, 1);

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
  //debug("Fct allumerBoutonLumineux : ", 0);
  //debug(idBoutonLumineux, 1);

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
  //debug("Fct eteindreBoutonLumineux : ", 0);
  //debug(idBoutonLumineux, 1);

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
  //debug("Fct eteindreLesDeuxBoutonsLumineux", 1);

  eteindreBoutonLumineux("gauche");
  eteindreBoutonLumineux("droite");
}

/**
  Allume le voyant et le bouton lumineux à jouer.

  @param idVoyantEtBoutonLumineux identifiant du voyant et bouton lumineux à allumer.

*/
void allumerLeVoyantEtBouton(String idVoyantEtBoutonLumineux) {
  //debug("Fct allumerLeVoyantEtBouton : ", 0);
  //debug(idVoyantEtBoutonLumineux, 1);

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
  /*
  debug("Fct eteindreLeVoyantEtBouton : ", 0);
  debug(idVoyantEtBoutonLumineux, 1);
  */

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
  //debug("Fct decrementerLesAppuisRestants", 1);

  nombreDePointsRestant = nombreDePointsRestant - 1 ;

  /*
    debug("Nombre d'AppuisRestants = ", 0);
    debug(String(nombreDePointsRestant), 1);
  */

}

/**
  choisi un bouton au hasard.

*/
void choixNouveauBouton() {
  //debug("Fct choixNouveauBouton", 1);

  //testerNombreAppuiRestant();

  // On eteind tous les voyants et boutons
  //@Todo

  // On decremente de 1 le nombre de points à jouer
  //@Todo

  // Si le nombre de points à jouer est supérieur à 0 on continue à choisir un autre bouton.
  //@Todo

  //debug("Nombre de points joué dans la séquence : " + String(nombreDePointsJouesDansLaSequence), 1);

  //int btnSuivant = nombreAleatoire(1, nombreBoutons + 1);
  int btnSuivant = sequenceEnCours[nombreDePointsJouesDansLaSequence];
  btnAllume = btnSuivant ;
  /*
    debug("Nouveau nombre aleatoire : ", 0);
    debug(String(btnSuivant), 1);
  */

  //Si le chiffre  est 1 => gauche
  if (1 == btnSuivant) {
    allumerLeVoyantEtBouton("gauche");
  }
  //Si le chiffre  est 2 => droite
  else if (2 == btnSuivant) {
    allumerLeVoyantEtBouton("droite");
  }

  //décrémente le nombre de points à jouer
  decrementerLesAppuisRestants();
}

/**
  choisi d'une séquence de jeu au hasard.

*/
void choixNouvelleSequence() {
  debug("Fct choixNouvelleSequence", 1);

  randomSeed(analogRead(0));

  //Initialisation de random

  int sequenceAleatoire = nombreAleatoire(0, nombreDeSequence - 1);
  debug("Sequence choisie au hasard : n° ", 0);
  debug(String(sequenceAleatoire), 1);
  debug("Contenu de la sequence choisie au hasard : ", 1);

  //sequenceEnCours = sequences[sequenceAleatoire];
  byte i;
  for (i = 0; i < (sizeof(sequences[sequenceAleatoire]) / sizeof(int)); i++)
  {
    sequenceEnCours[i] = sequences[sequenceAleatoire][i];
    debug(String(sequenceEnCours[i]), 0);
  }

  debug(String(""), 1);

  //Réinitialiser le nombre de point
  nombreDePointsRestant = nombreDePointsAJouer;
  debug("Nombre de points à jouer " + String(nombreDePointsRestant) + " l.362", 1);

  choixNouveauBouton();

}

/**
  Attend la temporisation si besoin.

*/
void attendreTemporisation() {
  //debug("Fct attendreTemporisation", 1);
  eteindreLesDeuxVoyants();
  delay(temporisationEntreLesPoints);
}

/**
  Démarre le programme.

*/
void testerNombreAppuiRestant() {
  debug("Fct testerNombreAppuiRestant", 1);
  debug("NombreAppuiRestant : ", 0);
  debug(String(nombreDePointsRestant), 1);

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
  Calculer le temps total de la partie.
    Temps de réaction
  + Temps de déplacement vers le bouton
  + Temp de pause retour à la base

*/
void calculerTempsTotalPartie() {

  dureeTotalePartie = tempoFinDePartie - tempoDebutDePartie ;
  dureeTotalePartieEnSecondes = dureeTotalePartie / 1000 ;
  dureeTotalePartieEnMiliSecondes = dureeTotalePartie % 1000;

}

/**
  Calculer le temps total de déplacement vers le bouton
  On enleve le temps de temporisation entre chaque point

*/
void calculerTempsTotalDeplacement() {
  calculerTempsTotalPartie();

  //dureeTotaleDeplacementPartie = dureeTotalePartie - ( nombreDePointsParSequence * temporisationEntreLesPoints);

  // Prendre en compte le nombre de point joués et non Toto si Micka ne fait pas les séquences complètes
  dureeTotaleDeplacementPartie = dureeTotalePartie - ( nombreDePointsJouesDansLaSequence * temporisationEntreLesPoints);

  dureeTotaleDeplacementPartieEnSecondes = dureeTotaleDeplacementPartie / 1000 ;
  dureeTotaleDeplacementPartieEnMiliSecondes = dureeTotaleDeplacementPartie % 1000;


}


/**
  Calculer le temps moyen d'un déplacement vers le bouton
  On divise le temps de déplacement par le nombre de points

*/
void calculerTempsMoyenDeplacement() {
  calculerTempsTotalDeplacement();

  //dureeMoyenneDeplacementPartie = dureeTotaleDeplacementPartie / nombreDePointsParSequence;
  dureeMoyenneDeplacementPartie = dureeTotaleDeplacementPartie / nombreDePointsJouesDansLaSequence;
  dureeMoyenneDeplacementPartieEnSecondes = dureeMoyenneDeplacementPartie / 1000 ;
  dureeMoyenneDeplacementPartieEnMiliSecondes = dureeMoyenneDeplacementPartie % 1000;


}


/**
  Affiche le temps mis.

*/
void afficherLesTemps() {
  //debug("Fct afficherLesTemps", 1);

  calculerTempsMoyenDeplacement();
  
    /*
    debug("Début de Sequence à : ", 0);
    debug(String(tempoDebutDePartie), 1);
    debug("Fin de Sequence à : ", 0);
    debug(String(tempoFinDePartie), 1);
    */
  

  debug("temps de déplacement total : ", 0);
  debug(String(dureeTotaleDeplacementPartieEnSecondes), 0);
  debug(",", 0);
  debug(String(dureeTotaleDeplacementPartieEnMiliSecondes), 1);

  debug("temps de déplacement moyen : ", 0);
  debug(String(dureeMoyenneDeplacementPartieEnSecondes), 0);
  debug(",", 0);
  debug(String(dureeMoyenneDeplacementPartieEnMiliSecondes), 1);

  String ligneUne = String(nombreDePointsJouesDansLaSequence) + "pts en " + String(dureeTotaleDeplacementPartieEnSecondes) + "," + String(dureeTotaleDeplacementPartieEnMiliSecondes) + "s" ;
  String ligneDeux = "Moyenne : " + String(dureeMoyenneDeplacementPartieEnSecondes) + "," + String(dureeMoyenneDeplacementPartieEnMiliSecondes) + "s" ;

  /*
    String ligneUne = "J";
    ligneUne.concat(nombreDePointsJoues);
    ligneUne.concat("/");
    ligneUne.concat("G");
    ligneUne.concat(nombreDePointsGagnes);
    ligneUne.concat("/");
    ligneUne.concat("P");
    ligneUne.concat(nombreDePointsPerdus);
    ligneUne.concat("/");
    ligneUne.concat("T");
    ligneUne.concat(nombreDePointsTentes);

  */

  afficheLCDLigne1(ligneUne);
  afficheLCDLigne2(ligneDeux);

}

/**
  Remet le jeu à zéro avant redemarrage.

*/
void remettreAZero() {
  //debug("Fct remettreAZero", 1);

  eteindreLeVoyantEtBouton("gauche");
  eteindreLeVoyantEtBouton("droite");

  // Remise des variables à l'état initial
  tempoDebutDePartie = 0;
  tempoFinDePartie = 0;
  dureeTotalePartie = 0;
  dureeTotalePartieEnSecondes = 0;
  dureeTotalePartieEnMiliSecondes = 0;
  dureeTotaleDeplacementPartie = 0;
  dureeTotaleDeplacementPartieEnSecondes = 0;
  dureeTotaleDeplacementPartieEnMiliSecondes = 0;
  dureeMoyenneDeplacementPartie = 0;
  dureeMoyenneDeplacementPartieEnSecondes = 0;
  dureeMoyenneDeplacementPartieEnMiliSecondes = 0;
  nombreDePointsJouesDansLaSequence = 0;

  //Effacer le temps

  debug("----------------------------------------", 1);

  debug("----------------------------------------", 1);
  debug("----------------------------------------", 1);

}

/**
  Lecture du bouton start qui selon l'état du jeu
  - lance le jeu (Start).
  - stop le jeu (Restart).

*/
void lectureDuBoutonStart(int etatJeu) {
  //debug("Fct lectureDuBoutonStart", 1);

  //Lecture du bouton RESET. La valeur 0 = bouton appuyé
  btnResetAppuye = digitalRead(btnReset);
  if (btnResetAppuye == 1 && etatJeu == 0)
  {
    debug("Demmarage de partie dans Fct lectureDuBoutonStart", 1);
    demarrerPartie();
  }
  if (btnResetAppuye == 1 && etatJeu == 1)
  {
    debug("Stop de partie dans Fct lectureDuBoutonStart", 1);
    stoperPartie();
  }

}

/**
  Lecture des bouton gauche et droite

*/
void lectureDesBoutons() {
  //debug("Fct lectureDesBoutons", 1);

  //Lecture du bouton 1
  btn1Appuye = digitalRead(btn1);
  //Serial.print("valeur du bouton 1: ");
  //Serial.println(btn1Appuye);
  if (btn1Appuye == 1)
  {
    verifierBouton(1);
  }

  //Lecture du bouton 2
  btn2Appuye = digitalRead(btn2);
  //Serial.print("valeur du bouton 2: ");
  //Serial.println(btn1Appuye);

  if (btn2Appuye == 1)
  {
    verifierBouton(2);
  }

  //Lecture du bouton start
  lectureDuBoutonStart(debutJeu);
}

/**
  Fonction qui verifie si le bouton appuyé est le bon

*/
void verifierBouton(int btnAVerifier) {

  //debug("Fct verifierBouton", 1);

  /*
    debug("Bouton appuyé = n°", 0);
    debug(String(btnAVerifier), 1);
    debug("Bouton allumé = ", 0);
    debug(String(btnAllume), 1);
  */

  if (btnAVerifier == btnAllume) {
    //debug("Bouton appuyé = OK", 1);
    nombreDePointsJouesDansLaSequence = nombreDePointsAJouer - nombreDePointsRestant ;
    attendreTemporisation();
    choixNouveauBouton();

  }
}

/**
  Démarre le jeu.

*/
void demarrerPartie() {
  debug("Fct demarrerPartie", 1);

  //Vider l'ecran en début de partie
  lcd.clear();  

  nombreDePointsRestant = nombreDePointsAJouer;

  debug("Nombre de points à jouer " + String(nombreDePointsRestant) + " l.619", 1);

  choixNouvelleSequence();
  //
  //choixNouveauBouton();
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

  debug(" ------ Fin de partie après " +  String(nombreDePointsJouesDansLaSequence) + " points  ------ ", 1);

  if (debutJeu == 1) {
    //Stop le jeu
    debutJeu = 0 ;
    //réinitialise les variables
    tempoFinDePartie = millis();

    //Calcul du temps moyen
    //calculerTempsMoyenDeplacement();
    //Afficher les temps (total de déplacement et temps moyen de déplacement)
    afficherLesTemps();

    remettreAZero();
  }

}
/**
  Fonction qui retourne nombre aleatoire

*/
long nombreAleatoire(int minAleatoire, int maxAleatoire) {

  randomSeed(analogRead(0));
  //nbrAleatoire = random(1, nombreBoutons + 1);
  nbrAleatoire = random(minAleatoire, maxAleatoire);
  debug("Nombre aleatoire = ", 0);
  debug(String(nbrAleatoire), 1);

  /*
     int aR = analogRead(A0) % 2;
    debug("Analog Read = ", 0);
    debug(String(aR), 1);
  */

  // test pour avoir une meilleur répartition avec nombres pairs et impairs
  // Si le nombre aléatoire est pair
  /*
    if ( (nbrAleatoire % 2) == 0) {
    nbrAleatoire = 2 ;
    }
    // Sinon le nombre aléatoire est impair
    else{
    nbrAleatoire = 1 ;
    }
  */
  return nbrAleatoire;
}


/**
  Fonction qui retourne le message transmis sur le moniteur serie

*/

void debug(String messageDebug, int retourLigne) {
  if (1 == debugMoniteurSerie) {
    if (1 == retourLigne) {
      Serial.println(messageDebug);
    }
    else
    {
      Serial.print(messageDebug);
    }
  }
}


// GESTION DE L'AFFICHAGE LCD 16x2 - DEBUT

void afficheLCDLigne1(String Message) {
  lcd.setCursor(0, 0);
  lcd.print(Message);
}
void afficheLCDLigne2(String Message) {
  lcd.setCursor(0, 1);
  lcd.print(Message);
}

void effaccerEcran() {
  lcd.clear();
}

// GESTION DE L'AFFICHAGE LCD 16x2 - FIN
