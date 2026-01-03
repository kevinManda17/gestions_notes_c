#include <stdio.h>
#include <string.h>

#include "utils.h"
#include "student.h"
#include "storage.h"

static void print_menu(void) {
    printf("\n========== MENU ==========\n");
    printf("1. Ajouter un étudiant et ses notes\n");
    printf("2. Supprimer un étudiant\n");
    printf("3. Afficher la liste des étudiants\n");
    printf("4. Afficher les résultats détaillés d’un étudiant\n");
    printf("5. Afficher les statistiques de la classe\n");
    printf("6. Quitter\n");
}

static Student input_student(void) {
    Student s;
    memset(&s, 0, sizeof(Student));

    read_line("Matricule: ", s.id, sizeof(s.id));
    while (strlen(s.id) == 0) {
        printf("Le matricule ne peut pas être vide.\n");
        read_line("Matricule: ", s.id, sizeof(s.id));
    }

    read_line("Nom: ", s.name, sizeof(s.name));
    while (strlen(s.name) == 0) {
        printf("Le nom ne peut pas être vide.\n");
        read_line("Nom: ", s.name, sizeof(s.name));
    }

    s.noteCount = read_int_range("Nombre de notes/matières (1..30): ", 1, MAX_SUBJECTS);
    for (int i = 0; i < s.noteCount; i++) {
        char prompt[64];
        snprintf(prompt, sizeof(prompt), "Note %d (0..20): ", i + 1);
        s.notes[i] = read_float_range(prompt, 0.0f, 20.0f);
    }
    return s;
}

static void menu_add_student(Classroom *c) {
    printf("\n=== Ajouter un étudiant ===\n");
    if (c->count >= MAX_STUDENTS) {
        printf("Capacité maximale atteinte (%d).\n", MAX_STUDENTS);
        return;
    }

    Student s = input_student();

    if (find_student_index_by_id(c, s.id) != -1) {
        printf("Erreur: ce matricule existe déjà.\n");
        return;
    }

    if (!add_student(c, &s)) {
        printf("Erreur: ajout impossible.\n");
        return;
    }
    printf("Étudiant ajouté avec succès.\n");
}

static void menu_remove_student(Classroom *c) {
    printf("\n=== Supprimer un étudiant ===\n");
    char id[MAX_ID];
    read_line("Matricule à supprimer: ", id, sizeof(id));

    if (!remove_student_by_id(c, id)) {
        printf("Matricule introuvable.\n");
        return;
    }
    printf("Étudiant supprimé.\n");
}

static void menu_show_details(const Classroom *c) {
    printf("\n=== Résultats détaillés ===\n");
    char id[MAX_ID];
    read_line("Matricule: ", id, sizeof(id));

    int idx = find_student_index_by_id(c, id);
    if (idx == -1) {
        printf("Matricule introuvable.\n");
        return;
    }
    print_student_details(&c->arr[idx]);
}

static void menu_show_stats(const Classroom *c) {
    printf("\n=== Statistiques de la classe ===\n");
    if (c->count == 0) {
        printf("Aucun étudiant.\n");
        return;
    }

    float avgClass = class_average(c);
    int best = best_student_index(c);
    int worst = worst_student_index(c);

    printf("Moyenne générale de la classe: %.2f\n", avgClass);

    if (best != -1) {
        printf("\n--- Meilleur étudiant ---\n");
        print_student_details(&c->arr[best]);
    }

    if (worst != -1) {
        printf("\n--- Moins bon étudiant ---\n");
        print_student_details(&c->arr[worst]);
    }
}

int main(void) {
    Classroom classroom;
    classroom_init(&classroom);

    const char *FILENAME = "data/students.csv";

    /* Chargement automatique si fichier existe */
    if (!load_classroom_csv(&classroom, FILENAME)) {
        printf("Attention: impossible de charger %s. Démarrage avec données vides.\n", FILENAME);
    } else {
        if (classroom.count > 0) {
            printf("Données chargées: %d étudiant(s).\n", classroom.count);
        }
    }

    while (1) {
        print_menu();
        int choice = read_int_range("Choix (1..6): ", 1, 6);

        switch (choice) {
            case 1:
                menu_add_student(&classroom);
                /* sauvegarde immédiate (sécurise contre crash) */
                if (!save_classroom_csv(&classroom, FILENAME)) {
                    printf("Attention: sauvegarde impossible.\n");
                }
                break;

            case 2:
                menu_remove_student(&classroom);
                if (!save_classroom_csv(&classroom, FILENAME)) {
                    printf("Attention: sauvegarde impossible.\n");
                }
                break;

            case 3:
                print_class_list(&classroom);
                break;

            case 4:
                menu_show_details(&classroom);
                break;

            case 5:
                menu_show_stats(&classroom);
                break;

            case 6:
                /* sauvegarde à la sortie */
                if (!save_classroom_csv(&classroom, FILENAME)) {
                    printf("Erreur: sauvegarde finale impossible.\n");
                }
                printf("Au revoir.\n");
                return 0;
        }
    }
}
