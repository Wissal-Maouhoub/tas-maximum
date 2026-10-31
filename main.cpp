#include <iostream>
#include <vector>
#include <queue>
#include <chrono>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <algorithm>

using namespace std;
using namespace chrono;


// ======================================
// 1. TAS MAX AVEC VECTOR
// ======================================

class TasVector {
private:
    vector<int> elements;

public:

    // Remonter un element
    void remonter(int i) {
        while (i > 0) {
            int parent = (i - 1) / 2;

            if (elements[i] > elements[parent]) {
                swap(elements[i], elements[parent]);
                i = parent;
            } else {
                break;
            }
        }
    }

    // Faire descendre un element
    void descendre(int i) {
        int taille = elements.size();

        while (true) {
            int gauche = 2 * i + 1;
            int droite = 2 * i + 2;
            int maximum = i;

            if (gauche < taille &&
                elements[gauche] > elements[maximum]) {
                maximum = gauche;
            }

            if (droite < taille &&
                elements[droite] > elements[maximum]) {
                maximum = droite;
            }

            if (maximum == i)
                break;

            swap(elements[i], elements[maximum]);
            i = maximum;
        }
    }

    // Insertion
    void inserer(int valeur) {
        elements.push_back(valeur);
        remonter(elements.size() - 1);
    }

    // Lecture du maximum sans suppression
    int obtenirMaximum() {
        return elements[0];
    }

    // Remplacement de la racine par le dernier element
    void supprimerEtRemplacerRacine() {
        if (elements.empty())
            return;

        elements[0] = elements.back();
        elements.pop_back();
    }

    // Ajustement du tas apres le remplacement de la racine
    void ajusterTas() {
        if (!elements.empty()) {
            descendre(0);
        }
    }

    // Suppression complete du maximum
    int extraireMaximum() {
        int maximum = obtenirMaximum();

        supprimerEtRemplacerRacine();
        ajusterTas();

        return maximum;
    }

    bool estVide() {
        return elements.empty();
    }
};


// ======================================
// 2. TAS MAX AVEC PRIORITY_QUEUE
// ======================================

class TasPriorityQueue {
private:
    priority_queue<int> elements;

public:

    // Insertion
    void inserer(int valeur) {
        elements.push(valeur);
    }

    // Lecture du maximum sans suppression
    int obtenirMaximum() {
        return elements.top();
    }

    // Suppression du maximum avec ajustement automatique
    int extraireMaximum() {
        int maximum = elements.top();
        elements.pop();

        return maximum;
    }

    bool estVide() {
        return elements.empty();
    }
};


// ======================================
// 3. PROGRAMME PRINCIPAL
// ======================================

int main() {

    srand(time(0));

    // Tailles a tester
    int tailles[] = {1000, 10000, 100000, 500000, 1000000,10000000,1000000000 };

    const int NB_REPETITIONS = 5;
    const int NB_LECTURES = 1000;

    cout << fixed << setprecision(6);


    // ======================================
    // PRESENTATION DU BENCHMARK
    // ======================================

    cout << "\n";
    cout << "==============================================================================================================================\n";
    cout << "                    COMPARAISON DES DEUX IMPLEMENTATIONS DU TAS MAX\n";
    cout << "==============================================================================================================================\n";

    cout << "\nIMPLEMENTATIONS COMPAREES :\n";
    cout << "  1. Tas max implemente manuellement avec vector.\n";
    cout << "  2. Tas max de la STL avec priority_queue.\n";

    cout << "\nOPERATIONS MESUREES :\n";
    cout << "  1. Insertion : insertion de N elements dans chaque tas.\n";
    cout << "  2. Lecture du maximum : consulter la racine sans la supprimer.\n";
    cout << "  3. Remplacement de la racine : remplacer le maximum par le dernier element.\n";
    cout << "  4. Ajustement du tas : reorganiser le tas apres le remplacement.\n";
    cout << "  5. Suppression et ajustement STL : supprimer le maximum et reorganiser automatiquement le tas.\n";

    cout << "\nUnites de mesure : millisecondes (ms).\n";
    cout << "Nombre de repetitions : " << NB_REPETITIONS << endl;
    cout << "Nombre de lectures du maximum par repetition : "
         << NB_LECTURES << endl;


    // ======================================
    // AFFICHAGE DE L'ENTETE DU TABLEAU
    // ======================================

    cout << "\n";
    cout << "==============================================================================================================================\n";
    cout << "                                                TABLEAU DES RESULTATS\n";
    cout << "==============================================================================================================================\n\n";

    cout << left
         << setw(10) << "N"
         << setw(17) << "Insertion V"
         << setw(17) << "Insertion STL"
         << setw(17) << "Lecture max V"
         << setw(17) << "Lecture max STL"
         << setw(19) << "Remplacement V"
         << setw(17) << "Ajustement V"
         << setw(20) << "Suppression STL"
         << endl;

    cout << left
         << setw(10) << ""
         << setw(17) << "(ms)"
         << setw(17) << "(ms)"
         << setw(17) << "(ms/op)"
         << setw(17) << "(ms/op)"
         << setw(19) << "(ms)"
         << setw(17) << "(ms)"
         << setw(20) << "(ms)"
         << endl;

    cout << "------------------------------------------------------------------------------------------------------------------------------\n";


    // ======================================
    // PARCOURS DES DIFFERENTES TAILLES
    // ======================================

    for (int n : tailles) {

        vector<int> valeurs;

        // Generer les memes valeurs pour les deux tas
        for (int i = 0; i < n; i++) {
            valeurs.push_back(rand() % 1000000 + 1);
        }


        // ======================================
        // 1. INSERTION DE N ELEMENTS
        // ======================================

        double insertionVector = 0;
        double insertionPriority = 0;

        for (int r = 0; r < NB_REPETITIONS; r++) {

            TasVector tasV;
            TasPriorityQueue tasP;

            // Insertion dans le tas vector
            auto debut1 = steady_clock::now();

            for (int valeur : valeurs) {
                tasV.inserer(valeur);
            }

            auto fin1 = steady_clock::now();

            insertionVector +=
                duration<double, milli>(fin1 - debut1).count();


            // Insertion dans priority_queue
            auto debut2 = steady_clock::now();

            for (int valeur : valeurs) {
                tasP.inserer(valeur);
            }

            auto fin2 = steady_clock::now();

            insertionPriority +=
                duration<double, milli>(fin2 - debut2).count();
        }

        insertionVector /= NB_REPETITIONS;
        insertionPriority /= NB_REPETITIONS;


        // ======================================
        // CONSTRUCTION DES DEUX TAS
        // POUR LES TESTS SUIVANTS
        // ======================================

        TasVector tasV;
        TasPriorityQueue tasP;

        for (int valeur : valeurs) {
            tasV.inserer(valeur);
            tasP.inserer(valeur);
        }


        // ======================================
        // 2. LECTURE DU MAXIMUM SANS SUPPRESSION
        // ======================================

        double extractionVector = 0;
        double extractionPriority = 0;

        volatile int resultat = 0;

        for (int r = 0; r < NB_REPETITIONS; r++) {

            // Lecture du maximum avec vector
            auto debut3 = steady_clock::now();

            for (int i = 0; i < NB_LECTURES; i++) {
                resultat = tasV.obtenirMaximum();
            }

            auto fin3 = steady_clock::now();

            extractionVector +=
                duration<double, milli>(fin3 - debut3).count();


            // Lecture du maximum avec priority_queue
            auto debut4 = steady_clock::now();

            for (int i = 0; i < NB_LECTURES; i++) {
                resultat = tasP.obtenirMaximum();
            }

            auto fin4 = steady_clock::now();

            extractionPriority +=
                duration<double, milli>(fin4 - debut4).count();
        }

        // Temps moyen d'une seule lecture
        extractionVector /= (NB_REPETITIONS * NB_LECTURES);
        extractionPriority /= (NB_REPETITIONS * NB_LECTURES);


        // ======================================
        // 3. REMPLACEMENT DE LA RACINE
        // ======================================

        double remplacementVector = 0;

        for (int r = 0; r < NB_REPETITIONS; r++) {

            // Copier le tas avant la mesure
            TasVector copieV = tasV;

            auto debut5 = steady_clock::now();

            copieV.supprimerEtRemplacerRacine();

            auto fin5 = steady_clock::now();

            remplacementVector +=
                duration<double, milli>(fin5 - debut5).count();
        }

        remplacementVector /= NB_REPETITIONS;


        // ======================================
        // 4. AJUSTEMENT DU TAS
        // ======================================

        double ajustementVector = 0;

        for (int r = 0; r < NB_REPETITIONS; r++) {

            // Preparer un tas dont la racine a ete remplacee
            TasVector copieV = tasV;

            copieV.supprimerEtRemplacerRacine();

            // Mesurer uniquement l'ajustement
            auto debut6 = steady_clock::now();

            copieV.ajusterTas();

            auto fin6 = steady_clock::now();

            ajustementVector +=
                duration<double, milli>(fin6 - debut6).count();
        }

        ajustementVector /= NB_REPETITIONS;


        // ======================================
        // 5. SUPPRESSION ET AJUSTEMENT STL
        // ======================================

        double suppressionPriority = 0;

        for (int r = 0; r < NB_REPETITIONS; r++) {

            // Copier le tas avant la mesure
            TasPriorityQueue copieP = tasP;

            auto debut7 = steady_clock::now();

            copieP.extraireMaximum();

            auto fin7 = steady_clock::now();

            suppressionPriority +=
                duration<double, milli>(fin7 - debut7).count();
        }

        suppressionPriority /= NB_REPETITIONS;


        // ======================================
        // 6. VERIFICATION DES RESULTATS
        // ======================================

        if (tasV.obtenirMaximum() != tasP.obtenirMaximum()) {

            cout << "\nErreur : les maximums sont differents pour N = "
                 << n << endl;
        }


        // ======================================
        // AFFICHAGE DES RESULTATS
        // ======================================

        cout << left
             << setw(10) << n
             << setw(17) << insertionVector
             << setw(17) << insertionPriority
             << setw(17) << extractionVector
             << setw(17) << extractionPriority
             << setw(19) << remplacementVector
             << setw(17) << ajustementVector
             << setw(20) << suppressionPriority
             << endl;
    }


    // ======================================
    // FIN DU BENCHMARK
    // ======================================

    cout << "------------------------------------------------------------------------------------------------------------------------------\n";

    cout << "\nFIN DU BENCHMARK\n";
    cout << "Les deux implementations ont ete testees sur les memes donnees.\n";
    cout << "Les resultats permettent de comparer leurs temps d'execution.\n";

    cout << "\nNombre de repetitions : " << NB_REPETITIONS << endl;

    return 0;
}
