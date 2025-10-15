/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hanebaro <hanebaro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 12:22:33 by hanebaro          #+#    #+#             */
/*   Updated: 2025/10/15 20:14:04 by hanebaro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "BitcoinExchange.hpp"

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cerr << "Error: could not open file." << std::endl;
        return (1);
    }
    BitcoinExchange exchange;
    if (!exchange.reaDatabase("data.csv"))
        return (1);
    exchange.InputFile(argv[1]);  
    return (0);
}