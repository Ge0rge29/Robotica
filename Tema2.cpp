#include <Arduino.h>
#include <string.h>

#define BAUD 28800

#define LED_ROSU 11
#define LED_VERDE 10
#define LED_ALBASTRU 9

#define BUTON_START 3
#define BUTON_DIFICULTATE 2


#define BACKSPACE 8 //Valoarea in tabelul ascii pentru backspace;


unsigned long timpTastare[3] = {5000, 4000, 3000}, timpJoc = 33000, momentSelectareCuvant = 0, timpDebounce = 300, timpIncepere = 3000, eroareIncepere = 50;
char cuvant[30], cuvantTinta[30];
char dictionar [10][30] = {"monitor","tastatura","laptop","taste","spray","cub","ecran","telefon","robotica","geam"};
char numeDificultati[3][10] = {"\nUsor\n", "\nMediu\n", "\nGreu\n"};

int index = 0, dificultate = 0, stareLed = 0, clipiri = 0, scor = 0;
volatile unsigned long momentApasareDificultate = 0, momentApasareStart = 0, momentIncepereJoc = 0, momentUltimaClipire = 0;

bool repaus = true, rulare = false;


void setRGB(int valRosu, int valVerde, int valAlbastru) //Functia seteaza culoarea led-ului RGB
{
  analogWrite(LED_ROSU, valRosu);
  analogWrite(LED_VERDE, valVerde);
  analogWrite(LED_ALBASTRU, valAlbastru);
}

//Functia primeste ca parametru un sir de caractere si verifica daca se potriveste cu cuvantul tinta (cel care trebuie scris)
//Functia va returna 1 daca cele 2 cuvinte sunt identice, 0 daca literele corespund, dar sirul de caractere este incomplet,
//si -1 daca literele difera la un anumit punct
int verificaCuvant(const char *cuvantDeVerificat)
{
  int lungime = strlen(cuvantDeVerificat);
  for(int i=0; i<lungime; i++)
  {
    if(cuvantDeVerificat[i] != cuvantTinta[i])
      return -1;
  }
  if(lungime == int(strlen(cuvantTinta)))
    return 1;
  return 0;
}


void finalizare()
{
  Serial.println("\nTerminat!\nScor:"); //Functia afiseaza scorul si reseteaza LED-ul RGB
  Serial.println(scor);
  Serial.println("\n");
  setRGB(100, 100, 100);
}


void verificaStare()
{
  if(rulare && millis() - momentIncepereJoc > timpJoc)
  {
    repaus = true;
    rulare = false;
    finalizare(); //Functia verifica daca timpul de joc (30 s) a exipirat; daca da, atunci se apeleaza functia de finalizare si se trece in starea de repaus
  }
}

//Functia se ocupa de numaratoarea inversa pana la inceperea jocului si de clipitul LED-ului in cele 3 secunde
void incepere()
{
  //Verifica daca a trecut destul timp de la ultima modificare a starii LED-ului pentru a o putea modifica din nou
  if(millis() - momentUltimaClipire > timpIncepere/6)
  {
    momentUltimaClipire = millis();
    setRGB(100 * stareLed, 100 * stareLed, 100 * stareLed);
    stareLed = (stareLed + 1) % 2;

    //Daca LED-ul si-a schimbat starea de un numar par de ori mai mic decat 6, se va afisa un numar, reprezentand numaratoarea inversa pana la inceperea jocului

    if(!(clipiri % 2))
    {
      Serial.println(char(3 - clipiri/2 + 48));
    }

    clipiri++;
  }

  //Cand LED-ul si-a schimbat starea de 6 ori (a clipit de 3 ori) se va selecta primul cuvant de tastat, LED-ul se va face verde, si se reseteaza sirul de caracter "cuvant"
  //in care se stocheaza ce se tasteaza

  if(clipiri == 6)
  {
    index = 0;
    strcpy(cuvant, "");
    strcpy(cuvantTinta,dictionar[(random(10) + momentIncepereJoc) % 10]);
    Serial.println(cuvantTinta);
    momentSelectareCuvant = millis();
    setRGB(0, 100, 0);
    clipiri++;
  }
}

//Functia initializeaza valori necesare pentru inceperea si rularea jocului
void startJoc()
{
  momentIncepereJoc = millis();
  scor = 0;

  //Necesara pentru animatia de inceput
  momentUltimaClipire = millis();
  clipiri = 0;
}

//Functie pentru intrerupere pe butonul de schimbat dificultatea
void verificareButonDificultate()
{
  if(repaus)
  {
    if(millis() - momentApasareDificultate > timpDebounce && digitalRead(BUTON_DIFICULTATE) == LOW)
    {
      momentApasareDificultate = millis();
      dificultate = (dificultate + 1) % 3;
      Serial.println(numeDificultati[dificultate]);
    }
  }
}

//Functie pentru intrerupere pe butonul de start
//Daca starea era de repaus, jocul va incepe; daca starea era de rulare, jocul se va finaliza
void verificareButonStart()
{
  if(millis() - momentApasareStart > timpDebounce && digitalRead(BUTON_START) == LOW)
  {
    momentApasareStart = millis();
    repaus = !repaus;
    rulare = !rulare;
    if(rulare)
    {
      startJoc();
    }else
    {
      finalizare();
    }
  }
}

//Functie pentru citirea unui caracter de pe interfata seriala
//Caracterul citit va fi salavat in sirul de caractere "cuvant"
//Daca se apasa "Backspace" se va sterge ultimul caracter din "cuvant"
void citireLitera()
{
  char litera = Serial.read();
  if(int(litera) == BACKSPACE)
  {
    if(index > 0)
    {
      cuvant[index - 1] = NULL;
      index--;
    }
  }else
  {
    cuvant[index] = litera;
    index++;
    cuvant[index] = NULL;
  }
}

//Functia alege un cuvant nou ca tinta din dictionar si memoreaza momentul cand acesta a fost ales
//Parametrul verdict determina daca cuvantul anterior a fost scris corect in limita de timp
//Daca da atunci creste scorul; altfel trimite un mesaj care anunta ca timpul a expirat

void alegereCuvantNou(int verdict)
{
  if(verdict)  // Verifică dacă cuvântul anterior a fost tastat corect înainte de expirarea timpului
  {
    scor++;  
    Serial.println("\n");  
  }
  else  // Dacă cuvântul nu a fost tastat corect în timp util
  {
    Serial.println("\nTimp expirat!\n"); 
  }

  strcpy(cuvant, " ");  // Resetează sirul `cuvant`, care va stoca ce tastează jucătorul
  index = 0;  // Resetează indexul la 0, astfel încât tastarea să înceapă de la începutul sirului `cuvant`

  // Selectează un nou cuvânt țintă din dicționar în mod aleatoriu
  strcpy(cuvantTinta, dictionar[(random(10) + momentIncepereJoc) % 10]);
  Serial.println(cuvantTinta); 

  momentSelectareCuvant = millis();  // Salvează momentul selectării noului cuvânt pentru a urmări timpul de tastare
}


void setup()
{
    Serial.begin(BAUD);  // Inițializează comunicarea serială la baud rate-ul specificat

    pinMode(LED_ROSU, OUTPUT);     
    pinMode(LED_VERDE, OUTPUT);    
    pinMode(LED_ALBASTRU, OUTPUT); 

    setRGB(100, 100, 100); 

    // Configurează pinui btoanelor, cu rezistență de pull-up internă
    pinMode(BUTON_START, INPUT_PULLUP); 
    pinMode(BUTON_DIFICULTATE, INPUT_PULLUP);  

    // Atașează întreruperi pentru butoane; `verificareButonStart` și `verificareButonDificultate` sunt apelate la apăsare
    attachInterrupt(digitalPinToInterrupt(BUTON_START), verificareButonStart, FALLING);  
    attachInterrupt(digitalPinToInterrupt(BUTON_DIFICULTATE), verificareButonDificultate, FALLING);

    Serial.println("\nSetup complet\n");
}


void loop()
{
  verificaStare();  // Verifică dacă timpul de joc a expirat; dacă da, finalizează jocul și trece în modul de repaus.

  if(rulare) 
  {
    // Verifică dacă a trecut perioada de numaratoare
    if(millis() - momentIncepereJoc <= timpIncepere + eroareIncepere)
    {
      incepere();  // Apelează funcția pt a realiza animația de numaratoare inversa
    }
    else
    {
      // Dacă jucatorul a tastat ceva
      if(Serial.available()) 
      {
          citireLitera();  // actualizează sirul

          
          if(verificaCuvant(cuvant) == 1)  // Dacă cuvântul este complet și corect
          {
            alegereCuvantNou(1);  // Alege un nou cuvânt, crește scorul și afișează noul cuvânt țintă.
          }
          else if(verificaCuvant(cuvant) == -1)  // Dacă s-a tastat un caracter greșit
          {
            setRGB(100, 0, 0);  // Setează LED-ul RGB pe roșu pentru a indica o greșeală.
          }
          else
          {
            setRGB(0, 100, 0);  // Dacă cuvântul este parțial corect, setează LED-ul pe verde.
          }
      }

      // Verifică dacă a expirat timpul pentru tastarea cuvântului curent.
      if(millis() - momentSelectareCuvant > timpTastare[dificultate])
      {
        alegereCuvantNou(0);  // Dacă timpul a expirat, alege un nou cuvânt și se afișează mesajul
      }
    }
  }
}
