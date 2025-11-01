/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PmergeMe.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hanebaro <hanebaro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/26 17:49:47 by hanebaro          #+#    #+#             */
/*   Updated: 2025/11/01 16:56:55 by hanebaro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <vector>
#include <deque>
#include <string>
#include <iomanip>

class PmergeMe
{
    private:
        std::vector<int> vectorData;
        std::deque<int> dequeData;

    public:
        std::vector<int> generateJacobsthal(int max_val);
        std::vector<int> getInsertionOrder(int pend_size);
        int binaryInsert(std::vector<int>& target, int value, int partner);
        int binaryInsert(std::deque<int>& target, int value, int partner);

        template <typename T>
        void ford_johnson_algo(T& data);
        
        PmergeMe();
        PmergeMe(const PmergeMe& other);
        PmergeMe& operator=(const PmergeMe& other);
        ~PmergeMe();

        // Méthodes publiques
        bool parseInput(int argc, char** argv);
        void displaySequence(const std::vector<int>& arr, const std::string& prefix) const;
        void sortAndDisplayResults();
};
 