#include "PmergeMe.hpp"
#include <cmath>
#include <algorithm>
#include <cstdlib>
#include <iomanip>
#include <sstream>

// --- Outils : Gestion du temps et Macros ---

#include <sys/time.h>
static double getTimeInMicroseconds() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec * 1000000.0 + tv.tv_usec;
}

// --- Fonctions d'aide Jacobsthal (Identiques à la version précédente) ---

std::vector<int> PmergeMe::generateJacobsthal(int max_val) {
    std::vector<int> jacobsthal;
    int j0 = 1;
    int j1 = 3;

    if (max_val >= 1) jacobsthal.push_back(1);
    if (max_val >= 3) jacobsthal.push_back(3);

    while (true) {
        long next_j = (long)j1 + 2L * (long)j0;
        if (next_j > max_val || next_j > 2147483647) break;
        jacobsthal.push_back(static_cast<int>(next_j));
        j0 = j1;
        j1 = static_cast<int>(next_j);
    }
    return jacobsthal;
}

std::vector<int> PmergeMe::getInsertionOrder(int pend_size) {
    if (pend_size <= 0) return {};
    std::vector<int> jacobsthal = generateJacobsthal(pend_size);
    std::vector<int> order;
    std::vector<bool> used_indices(pend_size + 1, false);
    
    for (size_t i = jacobsthal.size(); i > 1; --i) {
        int current_j = jacobsthal[i - 1];
        int previous_j = jacobsthal[i - 2]; 

        for (int k = current_j; k > previous_j; --k) {
            if (k <= pend_size && !used_indices[k]) {
                order.push_back(k);
                used_indices[k] = true;
            }
        }
    }
    
    for (int i = 2; i <= pend_size; ++i) {
        if (!used_indices[i]) {
            order.push_back(i);
        }
    }
    return order;
}

// --- Insertion Binaire Surchargée (pour vector et deque) ---

int PmergeMe::binaryInsert(std::vector<int>& target, int value) {
    auto it = std::upper_bound(target.begin(), target.end(), value);
    target.insert(it, value);
    return 0;
}

int PmergeMe::binaryInsert(std::deque<int>& target, int value) {
    auto it = std::upper_bound(target.begin(), target.end(), value);
    target.insert(it, value);
    return 0;
}

// --- Algorithme Ford-Johnson Surchargé (pour vector et deque) ---

template <typename T>
// void mergeInsertionSort(T& data, PmergeMe* pmergeMeInstance) {
//     if (data.size() <= 1) return;

//     int n = data.size();
//     bool has_stray = (n % 2 != 0);
//     int stray_element = -1;

//     if (has_stray) {
//         stray_element = data.back();
//         data.pop_back();
//         n--; 
//     }

//     T A; // Chaîne principale (éléments supérieurs)
//     T B; // Chaîne en attente (éléments inférieurs)
    
//     for (int i = 0; i < n; i += 2) {
//         if (data[i] < data[i + 1]) {
//             A.push_back(data[i + 1]); 
//             B.push_back(data[i]);     
//         } else {
//             A.push_back(data[i]);
//             B.push_back(data[i + 1]);
//         }
//     }
    
//     // Appel récursif (utilisant la même fonction)
//     if (A.size() > 1)
//         mergeInsertionSort(A, pmergeMeInstance); // Tri de A

//     // 4. Insertion du premier élément de B (b1) au début de A
//     if (!B.empty()) {
//         A.insert(A.begin(), B[0]);
//     }

//     // 5. Insertion optimisée de B dans A (Méthode Jacobsthal)
//     int comparisons = 0; // Ignoré ici, gardé pour la conformité de l'exemple précédent
//     int pend_size = B.size(); 
//     std::vector<int> order = pmergeMeInstance->getInsertionOrder(pend_size);

//     for (int index : order) {
//         if (index > 1 && index <= pend_size) {
//             int value_to_insert = B[index - 1]; 
//             pmergeMeInstance->binaryInsert(A, value_to_insert);
//         }
//     }

//     // 6. Insérer l'élément vagabond (stray)
//     if (has_stray) {
//         pmergeMeInstance->binaryInsert(A, stray_element);
//     }
    
//     data = A; // La chaîne A est maintenant la séquence triée.
// }




template <typename T>
void mergeInsertionSort(T& data, PmergeMe* pmergeMeInstance) {
    if (data.size() <= 1) return;

    int n = static_cast<int>(data.size()); // Utiliser static_cast pour size_t -> int si n est utilisé en int
    bool has_stray = (n % 2 != 0);
    int stray_element = -1;
    
    // --- 1. Gérer l'élément vagabond ---
    if (has_stray) {
        stray_element = data.back();
        data.pop_back();
        n--; 
    }

    // --- 2. Pairing et Séparation A/B ---
    T A; // Chaîne principale (éléments supérieurs)
    T B; // Chaîne en attente (éléments inférieurs)
    // std::pair et std::vector<std::pair> sont conformes C++98
    std::vector<std::pair<int, int> > pairs; // Pour maintenir la relation A/B

    for (int i = 0; i < n; i += 2) {
        // std::make_pair est conforme C++98
        if (data[i] < data[i + 1]) {
            pairs.push_back(std::make_pair(data[i + 1], data[i]));
        } else {
            pairs.push_back(std::make_pair(data[i], data[i + 1]));
        }
    }
    
    // Remplir A avec les éléments supérieurs
    for (size_t i = 0; i < pairs.size(); ++i) {
        A.push_back(pairs[i].first);
    }
    
    // --- 3. Tri Récursif de A et Réordonnancement de B (CRITIQUE) ---
    if (A.size() > 1) {
        // Tri récursif de A
        mergeInsertionSort(A, pmergeMeInstance);

        // Réordonnancement de B pour qu'il suive l'ordre trié de A.
        T B_temp;
        typename T::iterator it_A; // Déclaration avant la boucle pour C++98
        
        for (it_A = A.begin(); it_A != A.end(); ++it_A) {
            bool found = false;
            std::vector<std::pair<int, int> >::iterator it_pairs; // Déclaration itérateur
            
            for (it_pairs = pairs.begin(); it_pairs != pairs.end(); ++it_pairs) {
                // Si l'élément de A correspond à la partie supérieure de la paire
                if (*it_A == it_pairs->first) {
                    B_temp.push_back(it_pairs->second);
                    pairs.erase(it_pairs); // Supprimer la paire traitée
                    found = true;
                    break;
                }
            }
            // *NOTE C++98/DOUBLONS*: La gestion des doublons dans A qui auraient
            // la même valeur que `it_A` est très compliquée sans un tri stable
            // ou une structure d'indexation explicite. Nous continuons avec la logique simplifiée.
        }
        B = B_temp;
    }

    // --- 4. Insertion du premier élément de B (b1) ---
    // Utilisation de pop_front() pour deque et pop_back/reverse pour vector, 
    // mais ici B est T (généralisé). pop_front() est sûr pour deque.
    // Pour assurer la compatibilité avec T, B doit supporter front() et pop_front() 
    // ou être traité avec des itérateurs. Nous gardons la simplification :
    if (!B.empty()) {
        int b1 = B.front();
        A.insert(A.begin(), b1);
        
        // Simuler B.pop_front() : utiliser erase sur le début
        B.erase(B.begin()); 
    }

    // --- 5. Insertion optimisée de B restant (Jacobsthal) ---
    int pend_size = static_cast<int>(B.size()); // Nouvelle taille de B après b1
    
    // Obtenir l'ordre Jacobsthal pour les indices initiaux (2 à N+1)
    std::vector<int> order = pmergeMeInstance->getInsertionOrder(pend_size + 1); 

    std::vector<bool> inserted_flags(B.size(), false);
    
    // Insertion des éléments prioritaires (Jacobsthal) - Utilisation de l'itérateur C++98
    std::vector<int>::iterator it_order;
    for (it_order = order.begin(); it_order != order.end(); ++it_order) {
        int index_1_based = *it_order;
        
        // b1 (index 1) est déjà inséré. Nous travaillons sur le B restant (0-indexé).
        // b2 (index 2 initial) -> B[0] (nouveau)
        // b3 (index 3 initial) -> B[1] (nouveau)
        int index_0_based = index_1_based - 2; 
        
        if (index_0_based >= 0 && index_0_based < (int)B.size() && !inserted_flags[index_0_based]) {
            int value_to_insert = B[index_0_based];
            // binaryInsert est conforme si le conteneur A/B supporte upper_bound/insert
            pmergeMeInstance->binaryInsert(A, value_to_insert); 
            inserted_flags[index_0_based] = true;
        }
    }
    
    // Assurer l'insertion de TOUS les éléments restants de B (sécurité/correction d'omission)
    for (size_t i = 0; i < B.size(); ++i) {
        if (!inserted_flags[i]) {
            pmergeMeInstance->binaryInsert(A, B[i]);
        }
    }

    // --- 6. Insérer l'élément vagabond (stray) ---
    if (has_stray) {
        pmergeMeInstance->binaryInsert(A, stray_element);
    }
    
    data = A; 
}









// Spécialisations des appels de tri publiques
void PmergeMe::fordJohnsonSort(std::vector<int>& data) {
    mergeInsertionSort(data, this);
}
void PmergeMe::fordJohnsonSort(std::deque<int>& data) {
    mergeInsertionSort(data, this);
}

// --- Implémentation de la Classe PmergeMe ---

PmergeMe::PmergeMe() {}
PmergeMe::PmergeMe(const PmergeMe& other) : _vectorData(other._vectorData), _dequeData(other._dequeData) {}
PmergeMe& PmergeMe::operator=(const PmergeMe& other) {
    if (this != &other) {
        _vectorData = other._vectorData;
        _dequeData = other._dequeData;
    }
    return *this;
}
PmergeMe::~PmergeMe() {}

bool PmergeMe::parseInput(int argc, char** argv) {
    _vectorData.clear();
    _dequeData.clear();
    for (int i = 1; i < argc; i++) {
        char* endptr;
        long num = std::strtol(argv[i], &endptr, 10);
        
        if (*endptr != '\0' || num < 0 || num > 2147483647) {
            return false;
        }
        _vectorData.push_back(static_cast<int>(num));
        _dequeData.push_back(static_cast<int>(num));
    }
    return !_vectorData.empty();
}

void PmergeMe::displaySequence(const std::vector<int>& arr, const std::string& prefix) const {
    std::cout << prefix;
    size_t limit = arr.size() > 5 ? 5 : arr.size();
    for (size_t i = 0; i < limit; i++) {
        std::cout << arr[i];
        if (i < limit - 1)
            std::cout << " ";
    }
    if (arr.size() > 5)
        std::cout << " [...]";
    std::cout << std::endl;
}

// Fonction principale pour respecter les exigences de sortie
void PmergeMe::sortAndDisplayResults() {
    // 1. Afficher la séquence non triée (basé sur _vectorData)
    displaySequence(_vectorData, "Before: ");
    
    // --- Traitement Vector ---
    std::vector<int> vector_copy = _vectorData;
    double startVector = getTimeInMicroseconds();
    fordJohnsonSort(vector_copy);
    double endVector = getTimeInMicroseconds();
    double timeVector = endVector - startVector;
    
    // 2. Afficher la séquence triée (basé sur le résultat du vector)
    displaySequence(vector_copy, "After: ");
    
    // --- Traitement Deque ---
    std::deque<int> deque_copy = _dequeData;
    double startDeque = getTimeInMicroseconds();
    fordJohnsonSort(deque_copy);
    double endDeque = getTimeInMicroseconds();
    double timeDeque = endDeque - startDeque;
    
    // 3. Afficher le temps pour le premier conteneur (vector)
    std::cout << std::fixed << std::setprecision(5);
    std::cout << "Time to process a range of " << _vectorData.size() 
              << " elements with std::vector : " << timeVector << " us" << std::endl;
    
    // 4. Afficher le temps pour le deuxième conteneur (deque)
    std::cout << "Time to process a range of " << _dequeData.size() 
              << " elements with std::deque : " << timeDeque << " us" << std::endl;
}