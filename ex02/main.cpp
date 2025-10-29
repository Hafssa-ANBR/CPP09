/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hanebaro <hanebaro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/26 17:49:52 by hanebaro          #+#    #+#             */
/*   Updated: 2025/10/29 13:12:16 by hanebaro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "PmergeMe.hpp"

int main(int argc, char** argv) {
    PmergeMe sorter;
    
    if (argc < 2) {
        std::cerr << "Error: no input provided" << std::endl;
        return 1;
    }
    
    if (!sorter.parseInput(argc, argv)) {
        std::cerr << "Error" << std::endl;
        return 1;
    }

    sorter.sortAndDisplayResults();

    return 0;
}