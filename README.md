Descriere: 

În această temă trebuie să simulați o stație de încărcare pentru un vehicul electric, folosind mai multe LED-uri și butoane. În cadrul acestui task trebuie să țineți cont de stările butonului și să folosiți debouncing, dar și să coordonați toate componentele ca într-un scenariu din viața reală.

Cerinte:

*Led-ul RGB reprezintă disponibilitatea stației. Dacă stația este liberă led-ul va fi verde, iar dacă stația este ocupată se va face roșu.

*Led-urile simple reprezintă gradul de încărcare al bateriei, pe care îl vom simula printr-un loader progresiv (L1 = 25%, L2 = 50%, L3 = 75%, L4 = 100%). Loader-ul se încărca prin aprinderea succesivă a led-urilor, la un interval fix de 3s. LED-ul care semnifică procentul curent de încărcare va avea starea de clipire, LED-urile din urma lui fiind aprinse continuu, iar celelalte stinse.

*Apăsarea scurtă a butonului de start va porni încărcarea. Apăsarea acestui buton în timpul încărcării nu va face nimic.

*Apăsarea lungă a butonului de stop va opri încărcarea forțat și va reseta stația la starea liberă. Apăsarea acestui buton cat timp stația este liberă nu va face nimic.

Componente utilizate:

4x LED-uri (pentru a simula procentul de încărcare)
1x LED RGB (pentru starea de liber sau ocupat)
2x Butoane (pentru start încărcare și stop încărcare)
9x Rezistoare (7x 100/220/330ohm, 2x 1K)
Breadboard
Linii de legătură

Simulator:
[# Robotica](https://www.tinkercad.com/things/grvWpAq7Jlq/editel)

![29e79b96-8ed2-4c55-b2af-cc974e08443d](https://github.com/user-attachments/assets/a3ac4327-d8bc-4ce3-b4aa-4fa8893322dc)
![3f93a955-26ea-4857-9d61-4d1d161e95f1](https://github.com/user-attachments/assets/343d8b4a-61d3-4777-84a1-7f74ae3b2363)
![f9cb699f-2e0b-4361-8357-a488d5533835](https://github.com/user-attachments/assets/352edf9a-0216-4b5b-90ed-0f5ae47ebb8e)


https://github.com/user-attachments/assets/6df7994f-bb3c-4d75-bc82-7abf1a9e39c4

