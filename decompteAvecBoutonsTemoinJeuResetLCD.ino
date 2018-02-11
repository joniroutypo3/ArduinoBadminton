#include "Timer.h"

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

////////////////////////////////
// Variables de l ecran LCD 16 colonnes x 2 lignes - debut
LiquidCrystal_I2C lcd(0x27,16,2);

// Variables de l ecran LCD 16 colonnes x 2 lignes - fin

////////////////////////////////
// Variables timer - debut
Timer t; // Définition de l'objet timer

int debutTimer = 0;
int tempsDeJeu = 10;
int tempsRestant;
int stopTimer = 0;

// Variables timer - fin

////////////////////////////////
// Variable Jeu - debut
int debutJeu = 0;
int stopJeu = 0;
long nbrAleatoire;
int nombreBoutons = 3;
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
int btn3Appuye = 0;
int btnResetAppuye = 0;

////////////////////////////////
// Définition des entrée/sorties - debut
// Declaration des bornes de led
int led1 = 2;
int led2 = 3;
int led3 = 4;
int ledTemoinJeu = 8;

// Declaration des bornes de btn
int btn1 = 5;
int btn2 = 6;
int btn3 = 7;
int btnReset = 9;

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
 
 
void loop()
{
  t.update();
  if(debutTimer==0){
    Serial.println("Début du decompte");
    debutTimer++;
    //Initialisation de random
    choixNouveauBouton();
    allumerLedTemoinDeJeu();

  }

  // Si il reste du temps
  if(tempsRestant>0){

    lectureDesBoutons();
    
  }
  // Si il ne reste plus de temps
  else{
    if(stopTimer!=1){
    Serial.println("fin du decompte");
    eteindreLedTemoinDeJeu();
    afficherNombreDePointsJoues();
    afficherNombreDePointsGagnes();
    afficherNombreDePointsPerdus();
    afficherNombreDePointsTentes();
    affichierSuitePointsJoues();
    affichierSuitePointsTentes();
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
    //ligneUne.concat("/");
    afficheLCDLigne1(ligneUne);
    Serial.print(ligneUne);
    stopTimer = 1;

    }
    if(stopTimer==1){
      lectureDuBoutonReset();
    }
  }
}

// Lecture des boutons
void lectureDesBoutons(){
  
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

    //Lecture du bouton 3
    btn3Appuye=digitalRead(btn3);
    //Serial.print("valeur du bouton 3: ");
    //Serial.println(btn1Appuye);
 
    if(btn3Appuye==0)
    {
      verifierBouton(3);
    }
    //Serial.print("Bouton allumé global : ");
    //Serial.println(btnAllume);

    //Lecture du bouton RESET
    btnResetAppuye=digitalRead(btnReset);
    //Serial.print("valeur du bouton RESET: ");
    //Serial.println(btnResetAppuye);
 
    if(btnResetAppuye==0)
    {
      RelancePartie();
    }
}

// Lecture du boutons reset
void lectureDuBoutonReset(){
  
    //Lecture du bouton RESET
    btnResetAppuye=digitalRead(btnReset);
    
    if(btnResetAppuye==0)
    {
      RelancePartie();
    }
}



// Choix d'un nouveau bouton à allumer
void choixNouveauBouton(){
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

// Vérification que le bouton suivant soit différent du précédant
int verifierBoutonSuivantDifferentDuBoutonPrecedent(int boutonSuivant){
      indexTableauPointProposes = nombreDePointsJoues -1;
      Serial.print("indexTableauPointProposes : ");
      Serial.println(indexTableauPointProposes);
      int btnPrecedant = tableauPointProposes[indexTableauPointProposes];
      Serial.print("Bouton precedant était : ");
      Serial.println(btnPrecedant);
      
      while (btnPrecedant == boutonSuivant){
        Serial.print("Bouton suivant avant relance : ");
        Serial.println(boutonSuivant);
        randomSeed(analogRead(0));
        boutonSuivant = nombreAleatoire();
        Serial.print("Bouton suivant apres relance : ");
        Serial.println(boutonSuivant);
      }
      Serial.print("Nouveau bouton different du precedant : ");
        Serial.println(boutonSuivant);
      return boutonSuivant; 
}

// Fonction qui allume la LED n°ledAAllumer
void allumerLed(int ledAAllumer){
  if(ledAAllumer==1){
    digitalWrite(led1,HIGH);
  }
  else 
  {digitalWrite(led1,LOW);}
  
  if(ledAAllumer==2){
    digitalWrite(led2,HIGH);
  }
  else 
  {digitalWrite(led2,LOW);}
  
  if(ledAAllumer==3){
    digitalWrite(led3,HIGH);
  }
  else 
  {digitalWrite(led3,LOW);}
}

// Fonction qui eteind la LED n°ledAEteindre
void eteindreLed(int ledAEteindre){
  if(ledAEteindre==1){
    digitalWrite(led1,LOW);
  }
  
  if(ledAEteindre==2){
    digitalWrite(led2,LOW);
  }
  
  if(ledAEteindre==3){
    digitalWrite(led3,LOW);
  }
}

// Fonction qui allume la LED temoin de jeu
void allumerLedTemoinDeJeu(){
  
    digitalWrite(ledTemoinJeu,HIGH);

}

// Fonction qui eteind la LED temoin de jeu
void eteindreLedTemoinDeJeu(){
  
    digitalWrite(ledTemoinJeu,LOW);
  
}


// Fonction Relance la partie
void RelancePartie(){
  
    Serial.print("Relance de la partie demandée ! ");
    afficheLCDLigne2("Relance demandee");
    resetScore();
    delay(1000);
    effaccerEcran();
    delay(100);
    tempsRestant = 0 ;
    debutTimer = 0;
    tempsRestant = tempsDeJeu;
    stopTimer = 0; 
}

// Fonction qui verifie si le bouton appuyé est le bon
void verifierBouton(int btnAVerifier){
  Serial.print("Bouton appuyé = n°");
  Serial.println(btnAVerifier);
  
  // Incrémente le nombre de tentative
  incrementePointsTentes();
  stockPointsTentes(btnAVerifier);

  if(btnAVerifier == btnAllume){ 
    Serial.println("Bouton appuyé = OK");
    incrementePointsGagnes();
    eteindreLed(btnAllume);
    choixNouveauBouton();
  }
  else { 
    Serial.println("Bouton appuyé = KO");
    incrementePointsPerdus();
  }
}


// Fonction qui retourne nombre aleatoire
long nombreAleatoire(){
  //function body
  nbrAleatoire = random(1, nombreBoutons+1);
  return nbrAleatoire;
}


// GESTION TIMER - DEBUT

// Fonction qui incrémente le timer de +1
void incrementeTimer(){
  tempsRestant++;
}
// Fonction qui décrémente le timer de +1
void decrementeTimer(){
  tempsRestant--;
}
 
void envoiDecompte() // fonction qui envoie par liaison série l'entrée du pin 0
{
  decrementeTimer();
  if(tempsRestant>0){
    Serial.print("Temps restant : ");
    Serial.println(tempsRestant);
  }
}

// GESTION TIMER - FIN


// GESTION POINTS - DEBUT

// Fonction qui incrémente de +1 le nombre de points joués
void incrementePointsJoues(){
  Serial.println("Nombre de point joués avant incrementation");
  Serial.println(nombreDePointsJoues);
  nombreDePointsJoues++;
  Serial.println("Nombre de point joués après incrementation");
  Serial.println(nombreDePointsJoues);
}

// Fonction qui incrémente de +1 le nombre de points gagnés
void incrementePointsGagnes(){
  Serial.println("Nombre de point gagnés avant incrementation");
  Serial.println(nombreDePointsGagnes);
  nombreDePointsGagnes++;
  Serial.println("Nombre de point gagnés après incrementation");
  Serial.println(nombreDePointsGagnes);
}

// Fonction qui incrémente de +1 le nombre de points perdus
void incrementePointsPerdus(){
  nombreDePointsPerdus++;
}

// Fonction qui incrémente de +1 le nombre de points perdus
void incrementePointsTentes(){
  nombreDePointsTentes++;
}

// Fonction qui stock les points proposés
void stockPointsProposes(){
  indexTableauPointProposes = nombreDePointsJoues -1;
  tableauPointProposes[indexTableauPointProposes] = btnAllume;
}

// Fonction qui stock les points tentés
void stockPointsTentes(int boutonTente){
  indexTableauPointTentes = nombreDePointsTentes -1;
  tableauPointTentes[indexTableauPointTentes] = boutonTente;
}

// Fonction qui affiche le nombre de points joués
void afficherNombreDePointsJoues(){
  Serial.println("Nombre de point joués");
  Serial.println(nombreDePointsJoues);
}
// Fonction qui affiche le nombre de points gagnés
void afficherNombreDePointsGagnes(){
  Serial.println("Nombre de point gagnés");
  Serial.println(nombreDePointsGagnes);
}

// Fonction qui affiche le nombre de points perdus
void afficherNombreDePointsPerdus(){
  Serial.println("Nombre de point perdus");
  Serial.println(nombreDePointsPerdus);
}

// Fonction qui affiche le nombre de points tentés (perdus + gagnés)
void afficherNombreDePointsTentes(){
  Serial.println("Nombre de point tentes");
  Serial.println(nombreDePointsTentes);
}

// Fonction qui affiche la série de points proposés
void affichierSuitePointsJoues(){
  Serial.println("Suite de points proposés ");
  int i;
  for (i = 0; i < nombreDePointsJoues; i = i + 1) {
    Serial.print(tableauPointProposes[i]);
    Serial.print(" - ");
  }
  Serial.println("");
}

// Fonction qui affiche la série de points proposés
void affichierSuitePointsTentes(){
  Serial.println("Suite de points tentés ");
  int i;
  for (i = 0; i < nombreDePointsTentes; i = i + 1) {
    Serial.print(tableauPointTentes[i]);
    Serial.print(" - ");
  }
  Serial.println("");
}

// GESTION POINTS - FIN

// GESTION DE L'AFFICHAGE LCD 16x2 - DEBUT

void afficheLCDLigne1(String Message){
  lcd.setCursor(0,0); 
  lcd.print(Message);
}
void afficheLCDLigne2(String Message){
  lcd.setCursor(0,1); 
  lcd.print(Message);
}

void effaccerEcran(){
  lcd.clear();
}

// GESTION DE L'AFFICHAGE LCD 16x2 - FIN

void resetScore(){
  
nombreDePointsJoues = 0;
nombreDePointsGagnes = 0;
nombreDePointsPerdus = 0;
nombreDePointsTentes = 0;
memset(tableauPointProposes, 0, sizeof tableauPointProposes);
memset(tableauPointTentes, 0, sizeof tableauPointTentes);

}
