/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BitcoinExchange.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hanebaro <hanebaro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 12:22:25 by hanebaro          #+#    #+#             */
/*   Updated: 2025/10/14 22:49:08 by hanebaro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "BitcoinExchange.hpp"

BitcoinExchange::BitcoinExchange() {}

BitcoinExchange::BitcoinExchange(const BitcoinExchange &cpy)
{
    database = cpy.database;
}

BitcoinExchange &BitcoinExchange::operator=(const BitcoinExchange &affect)
{
    if (this != &affect)
    {
        database = affect.database;
    }
    return(*this);
}

bool BitcoinExchange::isvaliDate(const std::string &date) const
{
    if (date.length() != 10)
        return (false);
    if (date[4] != '-' || date[7] != '-')
        return (false);
    
    // Vérifier que les caractères sont des chiffres
    for (size_t i = 0; i < date.length(); i++) {
        if (i != 4 && i != 7 && !std::isdigit(date[i]))
            return (false);
    }
    // Extraire année, mois, jour
    int year = std::atoi(date.substr(0, 4).c_str());
    int month = std::atoi(date.substr(5, 2).c_str());
    int day = std::atoi(date.substr(8, 2).c_str());
    
    // Validation basique
    if (year < 1000 || year > 9999)
        return (false);
    if (month < 1 || month > 12)
        return (false);
    if (day < 1 || day > 31)
        return (false);
    // Validation des jours par mois (simplifiée)
    if (month == 2 && day > 29)
        return (false);
    if ((month == 4 || month == 6 || month == 9 || month == 11) && day > 30)
        return (false);
    
    return (true);
}

bool BitcoinExchange::isvalidValue(float value) const
{
    return value >= 0 && value <= 1000;
}

float BitcoinExchange::findAmount(const std::string &date) const
{
    
}

bool BitcoinExchange::reaDatabase(const std::string& filename)
{
    
}

void BitcoinExchange::InputFile(const std::string& filename)
{
    
}

BitcoinExchange::~BitcoinExchange()
{
    
}
