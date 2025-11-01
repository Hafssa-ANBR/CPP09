
#include "PmergeMe.hpp"
#include <cmath>
#include <algorithm>
#include <cstdlib>
#include <iomanip>
#include <sstream>
#include <vector>
#include <deque>
#include <sys/time.h>

// --- Outils : Gestion du temps ---

static double getTimeInMicroseconds() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec * 1000000.0 + tv.tv_usec;
}

// --- Fonctions d'aide Jacobsthal ---

std::vector<int> PmergeMe::generateJacobsthal(int max_val) {
    std::vector<int> jacobsthal;
    jacobsthal.push_back(0);
    jacobsthal.push_back(1);

    while (true) {
        size_t size = jacobsthal.size();
        long next_j = (long)jacobsthal[size - 1] + 2L * (long)jacobsthal[size - 2];
        if (next_j > max_val) break;
        jacobsthal.push_back(static_cast<int>(next_j));
    }
    return jacobsthal;
}

std::vector<int> PmergeMe::getInsertionOrder(int pend_size) {
    if (pend_size <= 0) return std::vector<int>();
    
    std::vector<int> jacobsthal = generateJacobsthal(pend_size);
    std::vector<int> order;
    std::vector<bool> used(pend_size, false);
    
    // Toujours insérer le premier élément de pend en premier
    order.push_back(0);
    used[0] = true;
    
    // Utiliser les nombres de Jacobsthal pour l'ordre optimal
    for (size_t i = 2; i < jacobsthal.size(); ++i) {
        int jacob_idx = jacobsthal[i];
        int prev_jacob_idx = jacobsthal[i - 1];
        
        // Insérer en ordre décroissant entre prev et current Jacobsthal
        for (int k = jacob_idx - 1; k >= prev_jacob_idx && k < pend_size; --k) {
            if (!used[k]) {
                order.push_back(k);
                used[k] = true;
            }
        }
    }
    
    // Ajouter les éléments restants
    for (int i = 0; i < pend_size; ++i) {
        if (!used[i]) {
            order.push_back(i);
        }
    }
    
    return order;
}

// --- Insertion Binaire Surchargée (pour vector et deque) ---

int PmergeMe::binaryInsert(std::vector<int>& target, int value) {
    std::vector<int>::iterator it = std::upper_bound(target.begin(), target.end(), value);
    target.insert(it, value);
    return 0;
}

int PmergeMe::binaryInsert(std::deque<int>& target, int value) {
    std::deque<int>::iterator it = std::upper_bound(target.begin(), target.end(), value);
    target.insert(it, value);
    return 0;
}

// --- Algorithme Ford-Johnson Surchargé (Template corrigé) ---

template <typename T>
void mergeInsertionSort(T& data, PmergeMe* pmergeMeInstance) {
    int n = static_cast<int>(data.size());
    
    if (n <= 1) return;

    // 1. Gérer l'élément vagabond (stray)
    bool has_stray = (n % 2 != 0);
    int stray_element = -1;
    
    if (has_stray) {
        stray_element = data[n - 1];
        n--; // Réduire n pour travailler avec un nombre pair
    }

    // 2. Créer les paires et séparer en main chain (A) et pend (B)
    std::vector<std::pair<int, int> > pairs;
    
    for (int i = 0; i < n; i += 2) {
        int first = data[i];
        int second = data[i + 1];
        
        // Mettre le plus grand en premier (main), le plus petit en second (pend)
        if (first > second) {
            pairs.push_back(std::make_pair(first, second));
        } else {
            pairs.push_back(std::make_pair(second, first));
        }
    }
    
    // 3. Extraire la main chain (les plus grands de chaque paire)
    T main_chain;
    for (size_t i = 0; i < pairs.size(); ++i) {
        main_chain.push_back(pairs[i].first);
    }
    
    // 4. Trier récursivement la main chain
    if (main_chain.size() > 1) {
        mergeInsertionSort(main_chain, pmergeMeInstance);
    }
    
    // 5. Réorganiser pend (B) selon l'ordre trié de main_chain
    T pend;
    for (size_t i = 0; i < main_chain.size(); ++i) {
        for (size_t j = 0; j < pairs.size(); ++j) {
            if (pairs[j].first == main_chain[i]) {
                pend.push_back(pairs[j].second);
                break;
            }
        }
    }
    
    // 6. Créer la chaîne finale S
    T S;
    
    // Insérer le premier élément de pend au début
    if (!pend.empty()) {
        S.push_back(pend[0]);
    }
    
    // Ajouter toute la main chain
    for (size_t i = 0; i < main_chain.size(); ++i) {
        S.push_back(main_chain[i]);
    }
    
    // 7. Insérer les éléments restants de pend selon l'ordre de Jacobsthal
    if (pend.size() > 1) {
        std::vector<int> insertion_order = pmergeMeInstance->getInsertionOrder(pend.size() - 1);
        
        for (size_t i = 0; i < insertion_order.size(); ++i) {
            int idx = insertion_order[i] + 1; // +1 car on a déjà inséré pend[0]
            if (idx < static_cast<int>(pend.size())) {
                pmergeMeInstance->binaryInsert(S, pend[idx]);
            }
        }
    }
    
    // 8. Insérer l'élément vagabond s'il existe
    if (has_stray) {
        pmergeMeInstance->binaryInsert(S, stray_element);
    }
    
    data = S;
}

// Spécialisations des appels de tri publiques
void PmergeMe::fordJohnsonSort(std::vector<int>& data) {
    mergeInsertionSort(data, this);
}
void PmergeMe::fordJohnsonSort(std::deque<int>& data) {
    mergeInsertionSort(data, this);
}

// --- Implémentation de la Classe PmergeMe (Constructeurs/Méthodes standards) ---

PmergeMe::PmergeMe() {}
PmergeMe::PmergeMe(const PmergeMe& other) : vectorData(other.vectorData), dequeData(other.dequeData) {}
PmergeMe& PmergeMe::operator=(const PmergeMe& other) {
    if (this != &other) {
        vectorData = other.vectorData;
        dequeData = other.dequeData;
    }
    return *this;
}
PmergeMe::~PmergeMe() {}

bool PmergeMe::parseInput(int argc, char** argv) {
    vectorData.clear();
    dequeData.clear();
    for (int i = 1; i < argc; i++) {
        char* endptr;
        long num = std::strtol(argv[i], &endptr, 10);
        
        if (*endptr != '\0' || num < 0 || num > 2147483647) {
            return false;
        }
        vectorData.push_back(static_cast<int>(num));
        dequeData.push_back(static_cast<int>(num));
    }
    return !vectorData.empty();
}

void PmergeMe::displaySequence(const std::vector<int>& arr, const std::string& prefix) const {
    std::cout << prefix;
    size_t limit = arr.size() > 5 ? 4 : arr.size();
    for (size_t i = 0; i < limit; i++) {
        std::cout << arr[i];
        if (i < limit - 1)
            std::cout << " ";
    }
    if (arr.size() > 5)
        std::cout << " [...]";
    std::cout << std::endl;
}

void PmergeMe::sortAndDisplayResults() {
    // 1. Afficher la séquence non triée
    displaySequence(vectorData, "Before: ");
    
    // --- Traitement Vector ---
    std::vector<int> vector_copy = vectorData;
    double startVector = getTimeInMicroseconds();
    fordJohnsonSort(vector_copy);
    double endVector = getTimeInMicroseconds();
    double timeVector = endVector - startVector;
    
    // 2. Afficher la séquence triée
    displaySequence(vector_copy, "After:  ");
    
    // --- Traitement Deque ---
    std::deque<int> deque_copy = dequeData;
    double startDeque = getTimeInMicroseconds();
    fordJohnsonSort(deque_copy);
    double endDeque = getTimeInMicroseconds();
    double timeDeque = endDeque - startDeque;
    
    // 3. Afficher le temps pour le premier conteneur (vector)
    std::cout << std::fixed << std::setprecision(5);
    std::cout << "Time to process a range of " << vectorData.size() 
              << " elements with std::vector : " << timeVector << " us" << std::endl;
    
    // 4. Afficher le temps pour le deuxième conteneur (deque)
    std::cout << "Time to process a range of " << dequeData.size() 
              << " elements with std::deque  : " << timeDeque << " us" << std::endl;
}