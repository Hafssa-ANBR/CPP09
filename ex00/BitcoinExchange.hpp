/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BitcoinExchange.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hanebaro <hanebaro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 12:22:28 by hanebaro          #+#    #+#             */
/*   Updated: 2025/10/15 20:08:28 by hanebaro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <algorithm>
// #include <sstream>
// #include <cstdlib>

class BitcoinExchange
{
    private:
        std::map<std::string, float> database;
        bool isvaliDate(const std::string &date) const;
        // bool isvalidValue(float value) const;
        float findAmount(const std::string &date) const;
        
    public:
        BitcoinExchange();
        BitcoinExchange(const BitcoinExchange &cpy);
        BitcoinExchange &operator=(const BitcoinExchange &affect);
        bool reaDatabase(const std::string& filename);
        void InputFile(const std::string& filename);
        ~BitcoinExchange();
};