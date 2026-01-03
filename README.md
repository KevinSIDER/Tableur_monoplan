# Monoplan - Tableur graphique en C

**Monoplan** est une application de tableur minimaliste développée en langage C utilisant la bibliothèque graphique **GTK+ 3**. Ce projet implémente un moteur de calcul basé sur la notation postfixée et gère la propagation automatique des changements via un graphe de dépendances.

**Auteurs :** Kevin SIDER & Rayan LADRAIT

---

## 📋 Fonctionnalités

- **Interface Graphique :** Grille interactive de 26 colonnes (A-Z) et 50 lignes.
- **Évaluation de formules :** Support des opérations arithmétiques de base (`+`, `-`, `*`, `/`) en notation postfixée.
- **Références de cellules :** Possibilité de référencer d'autres cellules (ex: `A1`, `B12`) dans les formules.
- **Propagation dynamique :** Lorsqu'une cellule est modifiée, toutes les cellules qui en dépendent sont automatiquement recalculées grâce à un algorithme de tri topologique sur un graphe de dépendances.
- **Détection d'erreurs :** Gestion de la division par zéro et des références circulaires ou invalides.

---

## 🛠️ Prérequis

Ce projet est conçu pour fonctionner sous **Linux**. Avant de compiler, assurez-vous d'avoir installé les outils de compilation et la bibliothèque de développement GTK+ 3.0.

**Pour Debian / Ubuntu / Mint :**

```bash
sudo apt-get update
sudo apt-get install build-essential pkg-config libgtk-3-dev

```

---

## 🚀 Installation et Compilation

Le projet utilise un **Makefile** situé à la racine pour automatiser la compilation.

1. **Compiler le projet :**

```bash
make

```

Cela générera l'exécutable `monoplan`.

2. **Lancer l'application :**

```bash
make run
# ou directement :
./monoplan

```

**Nettoyer les fichiers de compilation :**

```bash
make clean

```

---

## 📝 Utilisation et Syntaxe

Pour entrer une valeur ou une formule dans une cellule :

1. Cliquez sur une cellule dans la grille.
2. Utilisez la barre de saisie en haut de la fenêtre.
3. Appuyez sur **Entrée** pour valider.

### Règles de syntaxe

Toute saisie doit impérativement commencer par le caractère **`=`**.

Le tableur utilise la **Notation Polonaise Inverse (NPI)**. L'opérateur est placé _après_ ses opérandes.

| Type                   | Syntaxe Classique | Syntaxe Monoplan (NPI) |
| ---------------------- | ----------------- | ---------------------- |
| **Valeur simple**      | `= 42`            | `= 42`                 |
| **Addition**           | `= 10 + 20`       | `= 10 20 +`            |
| **Multiplication**     | `= 5 * 6`         | `= 5 6 *`              |
| **Opération complexe** | `= (3 + 5) * 2`   | `= 3 5 + 2 *`          |
| **Référence**          | `= A1 + 10`       | `= A1 10 +`            |

---

## 📂 Architecture du Projet

Le code est modulaire et séparé en plusieurs responsabilités :

- **Module_liste :**
- Implémentation d'une liste chaînée générique (`liste.c`, `liste.h`).
- Utilisée pour gérer les listes de jetons (tokens) et les listes de successeurs dans le graphe.

- **Module_formule_cellules :**
- `cell.c` / `cell.h` : Structure `s_cell` et `s_feuille_calcul`. Contient l'analyseur lexical (`analyser_chaine_cell`), l'évaluateur (`evaluate_cell`) et la gestion du graphe de dépendance (`propager_changements`).
- `Stack/` : Implémentation d'une pile (`stack.c`) utilisée pour le calcul des expressions postfixées.

- **Module_IHM (Interface) :**
- `main_gtk.c` : Point d'entrée du programme. Gère les événements GTK (clics, saisie), fait le lien entre la vue (`bind.h`) et le modèle (`cell.h`).
- `monoplan.glade` : Fichier XML décrivant l'interface graphique (fenêtre, grille, menus).

---

## 🧠 Détails Techniques (Algorithme de Propagation)

Le système de recalcul automatique repose sur un graphe orienté où chaque cellule est un nœud.

1. Lorsqu'une formule référençant `A1` est saisie dans `B1`, `B1` devient un **successeur** de `A1`.
2. Lors de la modification de `A1`, l'algorithme :

- Identifie le sous-graphe des cellules impactées.
- Calcule le degré entrant (nombre de prédécesseurs) de chaque cellule dans ce sous-graphe.
- Utilise une file pour recalculer les cellules dans l'ordre topologique (dès que tous les antécédents d'une cellule sont à jour).

---
