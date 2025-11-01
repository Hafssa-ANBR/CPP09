/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PmergeMe.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hanebaro <hanebaro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/26 17:49:47 by hanebaro          #+#    #+#             */
/*   Updated: 2025/10/29 19:32:59 by hanebaro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <vector>
#include <deque>
#include <string>
#include <iomanip>

// class PmergeMe
// {
//     private:
//         std::vector<int> vectorData;
//         std::deque<int> dequeData;
    
//     public:
//         PmergeMe();
//         PmergeMe(const PmergeMe &other);
//         PmergeMe& operator=(const PmergeMe &affect);
//         ~PmergeMe();
//         bool parseInput(int argc, char** argv);
//         void displaySequence(const std::vector<int>& arr, const std::string& prefix) const;
//         void sortAndDisplayResults();
// };


class PmergeMe {
private:
    std::vector<int> vectorData;
    std::deque<int> dequeData;

public:
    // --- Fonctions d'aide spécifiques à Ford-Johnson ---
    std::vector<int> generateJacobsthal(int max_val);
    std::vector<int> getInsertionOrder(int pend_size);
    int binaryInsert(std::vector<int>& target, int value);
    int binaryInsert(std::deque<int>& target, int value);

    // Fonction de tri principale pour les deux conteneurs
    // void fordJohnsonSort(std::vector<int>& data);
    // void fordJohnsonSort(std::deque<int>& data);

    template <typename T>
    void ford_johnson_algo(T& data);
    
    // Constructeurs / Destructeur
    PmergeMe();
    PmergeMe(const PmergeMe& other);
    PmergeMe& operator=(const PmergeMe& other);
    ~PmergeMe();

    // Méthodes publiques
    bool parseInput(int argc, char** argv);
    void displaySequence(const std::vector<int>& arr, const std::string& prefix) const;
    void sortAndDisplayResults(); // Fonction unique pour respecter les exigences de sortie
};
