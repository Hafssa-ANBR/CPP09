/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PmergeMe.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hanebaro <hanebaro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/26 17:49:50 by hanebaro          #+#    #+#             */
/*   Updated: 2025/11/01 16:55:14 by hanebaro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "PmergeMe.hpp"
#include <sys/time.h>
#include <algorithm>
#include <cstdlib>

PmergeMe::PmergeMe() {}

PmergeMe::PmergeMe(const PmergeMe &other) : vectorData(other.vectorData), dequeData(other.dequeData) {}

PmergeMe& PmergeMe::operator=(const PmergeMe &affect)
{
    if (this != &affect)
    {
        vectorData = affect.vectorData;
        dequeData = affect.dequeData;
    }
    return(*this);
}

PmergeMe::~PmergeMe() {}

static double getTimeInMicroseconds()
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (tv.tv_sec * 1000000.0 + tv.tv_usec);
}

std::vector<int> PmergeMe::generateJacobsthal(int max_val)
{
    std::vector<int> jacobsthal;
    jacobsthal.push_back(0);
    jacobsthal.push_back(1);

    while (1)
    {
        size_t size = jacobsthal.size();
        long next_j = (long)jacobsthal[size - 1] + 2L * (long)jacobsthal[size - 2];
        if (next_j > max_val)
            break;
        jacobsthal.push_back(static_cast<int>(next_j));
    }
    return (jacobsthal);
}

int PmergeMe::binaryInsert(std::vector<int>& target, int value, int partner)
{
    std::vector<int>::iterator partner_pos = target.end();
    for (std::vector<int>::iterator it = target.begin(); it != target.end(); ++it)
    {
        if (*it == partner)
        {
            partner_pos = it;
            break;
        }
    }
    std::vector<int>::iterator insert_pos = std::upper_bound(target.begin(), partner_pos, value);
    target.insert(insert_pos, value);
    return (0);
}

int PmergeMe::binaryInsert(std::deque<int>& target, int value, int partner)
{
    std::deque<int>::iterator partner_pos = target.end();
    for (std::deque<int>::iterator it = target.begin(); it != target.end(); ++it)
    {
        if (*it == partner)
        {
            partner_pos = it;
            break;
        }
    }
    std::deque<int>::iterator insert_pos = std::upper_bound(target.begin(), partner_pos, value);
    target.insert(insert_pos, value);
    return (0);
}

std::vector<int> PmergeMe::getInsertionOrder(int pend_size)
{
    if (pend_size <= 0)
        return (std::vector<int>());
    
    std::vector<int> jacobsthal = generateJacobsthal(pend_size);
    std::vector<int> order;
    std::vector<bool> insert(pend_size + 1, false);

    insert[0] = true;
    
    if (pend_size >= 1)
    {
        order.push_back(1);
        insert[1] = true;
    }
    
    // Parcourir les nombres de Jacobsthal
    for (size_t i = 2; i < jacobsthal.size(); i++)
    {
        int idx_jacob = jacobsthal[i];
        if (idx_jacob > pend_size)
            idx_jacob = pend_size;
            
        int prev_jacob = jacobsthal[i - 1];
        for (int j = idx_jacob; j > prev_jacob; j--)
        {
            if (j <= pend_size && !insert[j])
            {
                order.push_back(j);
                insert[j] = true;
            }
        }
    }
    for (int i = 1; i <= pend_size; i++)
    {
        if (!insert[i])
            order.push_back(i);
    }
    
    return (order);
}

template <typename T>
void PmergeMe::ford_johnson_algo(T& data)
{
    int n = static_cast<int>(data.size());
    if(n <= 1)
        return;
    
    int extra_var = -1;
    if(n % 2 == 1)
    {
        extra_var = data[n - 1];
        n--;
    }

    // Créer les paires et les trier
    std::vector<std::pair<int, int> > pairs;
    for(int i = 0; i < n; i += 2)
    {
        int a = data[i];
        int b = data[i + 1];

        if(a > b)
            pairs.push_back(std::make_pair(a, b));
        else
            pairs.push_back(std::make_pair(b, a));
    }

    T main_chain;
    for(size_t i = 0; i < pairs.size(); i++)
        main_chain.push_back(pairs[i].first);

    if(main_chain.size() > 1)
        ford_johnson_algo(main_chain);

    T pend_chain;
    for(size_t i = 0; i < main_chain.size(); i++)
    {
        for(size_t j = 0; j < pairs.size(); j++)
        {
            if(pairs[j].first == main_chain[i])
            {
                pend_chain.push_back(pairs[j].second);
                break;
            }
        }
    }

    if(extra_var != -1)
        pend_chain.push_back(extra_var);

    T final_chain;
    
    // Insérer le premier élément de pend_chain (il est toujours plus petit que main_chain[0])
    if(!pend_chain.empty())
        final_chain.push_back(pend_chain[0]);

    // Ajouter tous les éléments de main_chain
    for(size_t i = 0; i < main_chain.size(); i++)
        final_chain.push_back(main_chain[i]);

    // Insérer les éléments restants de pend_chain selon l'ordre de Jacobsthal
    if(pend_chain.size() > 1)
    {
        std::vector<int> order = this->getInsertionOrder(static_cast<int>(pend_chain.size()) - 1);
        
        for(size_t i = 0; i < order.size(); i++)
        {
            int value_to_insert = pend_chain[order[i]];
            int partner = -1;
            for(size_t j = 0; j < pairs.size(); j++)
            {
                if(pairs[j].second == value_to_insert)
                {
                    partner = pairs[j].first;
                    break;
                }
            }
            binaryInsert(final_chain, value_to_insert, partner);
        }
    }
    
    data = final_chain;
}

// Instanciations explicites
template void PmergeMe::ford_johnson_algo<std::vector<int> >(std::vector<int>& data);
template void PmergeMe::ford_johnson_algo<std::deque<int> >(std::deque<int>& data);

bool PmergeMe::parseInput(int argc, char** argv)
{
    for(int i = 1; i < argc; i++)
    {
        char* endptr;
        long num = strtol(argv[i], &endptr, 10);
        
        if (*endptr != '\0' || num < 0 || num > 2147483647)
        {
            return false;
        }
        vectorData.push_back(static_cast<int>(num));
        dequeData.push_back(static_cast<int>(num));
    }
    return (!vectorData.empty());
}

void PmergeMe::displaySequence(const std::vector<int>& arr, const std::string& prefix) const
{
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
void PmergeMe::sortAndDisplayResults()
{
    displaySequence(vectorData, "Before: ");

    double begin_vec;
    double end_vec;
    double time_vec;
    std::vector<int> vec_cpy = vectorData;
    begin_vec = getTimeInMicroseconds();
    ford_johnson_algo(vec_cpy);
    end_vec = getTimeInMicroseconds();
    time_vec = end_vec - begin_vec;
    displaySequence(vec_cpy, "After:  ");
    
    double begin_deq;
    double end_deq;
    double time_deq;
    std::deque<int> deq_cpy = dequeData;
    begin_deq = getTimeInMicroseconds();
    ford_johnson_algo(deq_cpy);
    end_deq = getTimeInMicroseconds();
    time_deq = end_deq - begin_deq;

    std::cout << std::fixed << std::setprecision(5);
    std::cout << "Time to process a range of " << vectorData.size() << " elements with std::vector : " << time_vec << "us" << std::endl;
    std::cout << "Time to process a range of " << dequeData.size() << " elements with std::deque : " << time_deq << "us" << std::endl;  
}
