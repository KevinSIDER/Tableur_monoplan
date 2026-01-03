/* ---------------------------------------------------
 * Tests unitaires des formules et du graphe des cellules.
 * ---------------------------------------------------
 * Kevin SIDER & Rayan LADRAIT
 * ---------------------------------------------------
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cell.h"
#include "../Module_liste/liste.h"
#include "Stack/stack.h"

int main() {
    printf("=== Tests Cellules ===\n");

    // --- Création de la feuille ---
    s_feuille_calcul *feuille = creer_feuille("FeuilleTest");

    // --- Test 1 : valeur simple ---
    s_cell cell1 = {0}; // initialisation complète
    cell1.chaine = "= 42";
    analyser_chaine_cell(&cell1, feuille);
    printf("Test 1 : valeur simple = %.2f (attendu 42.00)\n", evaluate_cell(&cell1));

    // --- Test 2 : opérateurs simples ---
    s_cell cell2 = {0};
    cell2.chaine = "= 3 5 +";  // postfix 3+5
    analyser_chaine_cell(&cell2, feuille);
    printf("Test 2a : 3 + 5 = %.2f (attendu 8.00)\n", evaluate_cell(&cell2));

    cell2.chaine = "= 10 4 -";
    analyser_chaine_cell(&cell2, feuille);
    printf("Test 2b : 10 - 4 = %.2f (attendu 6.00)\n", evaluate_cell(&cell2));

    cell2.chaine = "= 6 7 *";
    analyser_chaine_cell(&cell2, feuille);
    printf("Test 2c : 6 * 7 = %.2f (attendu 42.00)\n", evaluate_cell(&cell2));

    cell2.chaine = "= 20 4 /";
    analyser_chaine_cell(&cell2, feuille);
    printf("Test 2d : 20 / 4 = %.2f (attendu 5.00)\n", evaluate_cell(&cell2));

    // --- Test 3 : opération complexe ---
    s_cell cell3 = {0};
    cell3.chaine = "= 3 5 + 2 * "; // (3+5)*2 = 16
    analyser_chaine_cell(&cell3, feuille);
    printf("Test 3 : (3+5)*2 = %.2f (attendu 16.00)\n", evaluate_cell(&cell3));

    // --- Test 4 : référence vers une cellule ---
    s_cell cellA1 = {0};
    cellA1.chaine = "= 7";
    analyser_chaine_cell(&cellA1, feuille);
    feuille->cellules[0][0] = &cellA1; // A1 doit être assignée avant d'évaluer les références
    evaluate_cell(&cellA1);

    s_cell cell4 = {0};
    cell4.chaine = "= A1 3 +"; // 7 + 3 = 10
    analyser_chaine_cell(&cell4, feuille);
    printf("Test 4 : A1 + 3 = %.2f (attendu 10.00)\n", evaluate_cell(&cell4));

    // --- Test 5 : cellule vide ---
    s_cell cell5 = {0};
    cell5.chaine = NULL;
    analyser_chaine_cell(&cell5, feuille);
    printf("Test 5 : cellule vide = %.2f (attendu 0.00)\n", evaluate_cell(&cell5));

    // --- Test 6 : valeur seule 15 ---
    s_cell cell6 = {0};
    cell6.chaine = "= 15";
    analyser_chaine_cell(&cell6, feuille);
    printf("Test 6 : cellule = 15 => %.2f (attendu 15.00)\n", evaluate_cell(&cell6));


    // --- Test 7 : Négatif autorisé ---
    s_cell cell7 = {0};
    cell7.chaine = "= 10 15 -";
    analyser_chaine_cell(&cell7, feuille);
    // Ici on attend bien -5.00
    printf("Test 7 : = 10 15 - => %.2f (attendu -5.00)\n", evaluate_cell(&cell7));


    // --- Test 8 : Erreur syntaxe (Pas de =) ---
    s_cell cell8 = {0};
    cell8.chaine = "3 5 +";
    analyser_chaine_cell(&cell8, feuille);
    double res8 = evaluate_cell(&cell8);
    printf("Résultat : %.2f (attendu 0.00 car refusé)\n", res8);

    // ==========================================================
    // === TESTS MODULE 3 : GRAPHE ET PROPAGATION ===
    // ==========================================================
    printf("\n=== Module 3 : Graphe et Propagation ===\n");
    
    // Scénario de test : Chaîne de dépendance
    // A2 = 10
    // B2 = A2 * 2  (doit valoir 20)
    // C2 = B2 + 5  (doit valoir 25)

    printf("Construction du graphe A2 -> B2 -> C2...\n");

    // 1. Initialisation A2 (Col 0, Lig 1)
    if (feuille->cellules[0][1] == NULL) feuille->cellules[0][1] = calloc(1, sizeof(s_cell));
    s_cell *cA2 = feuille->cellules[0][1];
    cA2->chaine = "= 10";
    analyser_chaine_cell(cA2, feuille);
    evaluate_cell(cA2);
    printf("  [INIT] A2 : %.2f\n", cA2->valeur);

    // 2. Initialisation B2 (Col 1, Lig 1) dépend de A2
    if (feuille->cellules[1][1] == NULL) feuille->cellules[1][1] = calloc(1, sizeof(s_cell));
    s_cell *cB2 = feuille->cellules[1][1];
    cB2->chaine = "= A2 2 *";
    analyser_chaine_cell(cB2, feuille); // Création du lien A2 -> B2
    evaluate_cell(cB2);
    printf("  [INIT] B2 (= A2 2 *) : %.2f\n", cB2->valeur);

    // 3. Initialisation C2 (Col 2, Lig 1) dépend de B2
    if (feuille->cellules[2][1] == NULL) feuille->cellules[2][1] = calloc(1, sizeof(s_cell));
    s_cell *cC2 = feuille->cellules[2][1];
    cC2->chaine = "= B2 5 +";
    analyser_chaine_cell(cC2, feuille); // Création du lien B2 -> C2
    evaluate_cell(cC2);
    printf("  [INIT] C2 (= B2 5 +) : %.2f\n", cC2->valeur);

    // 4. Modification de A2 et Propagation
    printf("\nModification de A2 (10 -> 20) et propagation...\n");
    cA2->chaine = "= 20";
    analyser_chaine_cell(cA2, feuille); // Mise à jour de la formule/valeur de A2
    
    // Appel de la fonction de propagation
    // Cela doit recalculer A2, puis B2 (car dépend de A2), puis C2 (car dépend de B2)
    propager_changements(cA2);

    // 5. Vérification
    printf("  [RESULTAT] A2 : %.2f (Attendu 20.00)\n", cA2->valeur);
    printf("  [RESULTAT] B2 : %.2f (Attendu 40.00)\n", cB2->valeur);
    printf("  [RESULTAT] C2 : %.2f (Attendu 45.00)\n", cC2->valeur);

    if (cA2->valeur == 20.0 && cB2->valeur == 40.0 && cC2->valeur == 45.0) {
        printf(">>> TEST PROPAGATION REUSSI <<<\n");
    } else {
        printf(">>> ECHEC TEST PROPAGATION <<<\n");
    }

    // --- Test Successeurs cleanup ---
    printf("\n=== Test successeurs cleanup (A1 switches ref from B1 to C1) ===\n");
    // Ensure B1 and C1 exist as cells in feuille
    if (!feuille->cellules[1][0]) feuille->cellules[1][0] = calloc(1, sizeof(s_cell));
    if (!feuille->cellules[2][0]) feuille->cellules[2][0] = calloc(1, sizeof(s_cell));
    s_cell *B1 = feuille->cellules[1][0];
    s_cell *C1 = feuille->cellules[2][0];
    B1->chaine = "= 2"; analyser_chaine_cell(B1, feuille); evaluate_cell(B1);
    C1->chaine = "= 3"; analyser_chaine_cell(C1, feuille); evaluate_cell(C1);

    if (!feuille->cellules[0][0]) feuille->cellules[0][0] = calloc(1, sizeof(s_cell));
    s_cell *A1n = feuille->cellules[0][0];
    A1n->chaine = "= B1 1 +"; analyser_chaine_cell(A1n, feuille);
    // Check B1 successeurs contains A1n
    int foundA1inB1 = 0;
    node_t *n = B1->successeurs;
    while (n) { if (list_get_data(n) == (void *)A1n) foundA1inB1 = 1; n = list_next(n); }
    printf("B1 has A1 as successeur: %s\n", foundA1inB1 ? "YES" : "NO");

    // Now change A1 to reference C1
    A1n->chaine = "= C1 1 +"; analyser_chaine_cell(A1n, feuille);
    // Check B1 successeurs doesn't contain A1n anymore
    foundA1inB1 = 0; n = B1->successeurs; while (n) { if (list_get_data(n) == (void *)A1n) foundA1inB1 = 1; n = list_next(n); }
    printf("After change, B1 has A1 as successeur: %s\n", foundA1inB1 ? "YES" : "NO");
    // Check C1 successeurs contains A1n now
    int foundA1inC1 = 0; n = C1->successeurs; while (n) { if (list_get_data(n) == (void *)A1n) foundA1inC1 = 1; n = list_next(n); }
    printf("C1 has A1 as successeur: %s\n", foundA1inC1 ? "YES" : "NO");

    return 0;
}
