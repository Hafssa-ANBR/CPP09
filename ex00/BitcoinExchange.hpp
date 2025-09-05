/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BitcoinExchange.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hanebaro <hanebaro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 12:22:28 by hanebaro          #+#    #+#             */
/*   Updated: 2025/09/05 13:14:11 by hanebaro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <iostream>

class BitcoinExchange
{
    private:
        
    public:
    BitcoinExchange();
    
    BitcoinExchange(const BitcoinExchange &cpy);
    BitcoinExchange &operator=(const BitcoinExchange &affect);
    ~BitcoinExchange();
};