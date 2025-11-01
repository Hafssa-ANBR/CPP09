/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BitcoinExchange.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hanebaro <hanebaro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 12:22:25 by hanebaro          #+#    #+#             */
/*   Updated: 2025/10/29 17:31:03 by hanebaro         ###   ########.fr       */
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

// bool BitcoinExchange::isvalidValue(float value) const
// {
//     return (value >= 0 && value <= 1000);
// }

float BitcoinExchange::findAmount(const std::string &date) const
{
    std::map<std::string, float>::const_iterator it = database.upper_bound(date);
    std::cout << "Debug: upper_bound for date " << date << " found " << (it != database.end() ? it->first : "end") << std::endl;
    if (it == database.begin())
        return (-1);
    --it;
    return it->second;   
}

bool BitcoinExchange::reaDatabase(const std::string& filename)
{
    std::ifstream file(filename.c_str());
    if(!file.is_open())
    {
        std::cerr << "Error: could not open " << filename << " file." << std::endl;
        return(false);
    }
    std::string line;
    std::getline(file, line);
    while(std::getline(file, line))
    {
        size_t pos = line.find(',');
        if(pos == std::string::npos)
            continue;
        std::string date = line.substr(0,pos);
        std::string strVal = line.substr(pos + 1);
        float val = std::atof(strVal.c_str());
        database[date] = val;
        // add parsing
    }
    file.close();
    return(true);
}

void BitcoinExchange::InputFile(const std::string& filename)
{
    std::ifstream file(filename.c_str());

    if(!file.is_open())
    {
        std::cerr << "Error: could not open " << filename << " file" << std::endl;
        return;
    }
    std::string line;
    std::getline(file, line); // Ignorer l'en-tête si présent; je peut la suppr
    while(std::getline(file, line))
    {
        size_t pos = line.find('|');//why size_t
        if(pos == std::string::npos)
        {
            std::cerr << "Error: bad input => " << line << std::endl;
            continue;
        }
        std::string date = line.substr(0, pos - 1);
        std::string strVal = line.substr(pos + 2);// verif pos + 2 and pos -1
        
        if (!isvaliDate(date))
        {
            std::cerr << "Error: bad input => " << date << std::endl;
            continue;
        }
        char *endptr;
        float val = std::strtof(strVal.c_str(), &endptr);
        if(*endptr != '\0' && *endptr != '\n')
        {
            std::cerr << "Error: bad input => " << line << std::endl;
            continue;
        }
        if (val < 0)
        {
            std::cerr << "Error: not a positive number." << std::endl;
            continue;
        }
        if (val > 1000)
        {
            std::cerr << "Error: too large a number." << std::endl;
            continue;
        }
        float amount = findAmount(date);
        if(amount < 0)
        {
            std::cerr << "Error: no data available for this date." << std::endl;
            continue;
        }
        std::cout << date << " => " << val << " = " << (val * amount) << std::endl;
    }
    file.close();
}

BitcoinExchange::~BitcoinExchange() {}
